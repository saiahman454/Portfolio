//------------------------------------------------------------------------------
//
// File Name:	SaveMenu.cpp
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "SaveMenu.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "GameObjectManager.h"
#include "Utility.h"
#include "JsonWriter.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Time.h"
#include "GameStateManager.h"

// Dapper Namespace!
namespace Dapper {


  /*--------------------------------------
  Description: constructor for SaveMenu
  Params:
  parent - the parent level editor
  ---------------------------------------*/
  SaveMenu::SaveMenu(LevelEditor& parent) :
    my_parent(parent)
  {
  }

  /*--------------------------------------
  Description: destructor for SaveMenu
  ---------------------------------------*/
  SaveMenu::~SaveMenu()
  {
  }

  /*--------------------------------------
  Description: Init for the window
  ---------------------------------------*/
  void SaveMenu::Init()
  {
    save_timer = 30;
    SceneManager& scene_man = *GetSystemFromEngine< SceneManager*>(my_parent.GetEngine(), "SceneManager");
    std::function<void(void)> calc_names = std::bind(&SaveMenu::CalcSceneNames, this);
    scene_man.RegisterOnChange(calc_names);

    scene_names.clear();
    for (unsigned i = 0; i < scene_man.Size(); ++i)
    {
      scene_names.push_back(scene_man.GetSceneAtIndex(i).GetName().c_str());
    }

  }

  /*--------------------------------------
  Description: Render function
  ---------------------------------------*/
  void SaveMenu::Render()
  {
    float scale = my_parent.GetScaleFactor();
    ImVec2 size(100 * scale, 50 * scale);
    ImGui::Begin("Scene Controller");
    //ImGui::SetNextWindowPos({ 0,650*scale });
    //ImGui::SetNextWindowSize({ 475 * scale,210 * scale });
    ImGui::SetWindowFontScale(1.60 * scale);
    ImGui::PushItemWidth(420 * scale);
    ImGui::Combo("Scenes", &current_scene, scene_names.data(), scene_names.size(), 4);
    ImGui::PopItemWidth();
    if (ImGui::Button("Save", size))
    {
      SceneManager& scene_man = *GetSystemFromEngine< SceneManager*>(my_parent.GetEngine(), "SceneManager");
      Scene& the_scene = scene_man.GetScene(scene_names[current_scene]);
      the_scene.Save();
    }
    ImGui::SameLine();
    if (ImGui::Button("Save As", size))
    {
      action = Save_Action::save_as;
    }
    ImGui::SameLine();
    if (ImGui::Button("Load", size))
    {
      action = Save_Action::load;
    }
    ImGui::SameLine();
    if (ImGui::Button("Unload", size))
    {
      action = Save_Action::Unload;
    }
    ImGui::SameLine();
    if (ImGui::Button("New", size))
    {
      action = Save_Action::new_scene;
    }
    ImGui::Checkbox("Auto Save", &auto_save);
    SaveActionsDisplay();


    if (auto_save)
    {
      Time& time = *GetSystemFromEngine<Time*>(my_parent.GetEngine(), "Time");
      const float& dt = time.GetRenderdt();
      if (save_timer <= 0)
      {
        if (current_scene != -1)
        {
          SceneManager& scene_man = *GetSystemFromEngine< SceneManager*>(my_parent.GetEngine(), "SceneManager");
          Scene& the_scene = scene_man.GetScene(scene_names[current_scene]);
          the_scene.Save();
        }
        save_timer = 30;
      }
      save_timer -= dt;
    }

    ImGui::End();
  }

  void SaveMenu::CalcSceneNames()
  {
    scene_names.clear();
    SceneManager& scene_man = *GetSystemFromEngine< SceneManager*>(my_parent.GetEngine(), "SceneManager");
    for (unsigned i = 0; i < scene_man.Size(); ++i)
    {
      scene_names.push_back(scene_man.GetSceneAtIndex(i).GetName().c_str());
    }
  }

  std::string_view SaveMenu::GetCurrentScene()
  {
    return scene_names[current_scene];
  }

  void SaveMenu::SaveActionsDisplay()
  {
    float scale = my_parent.GetScaleFactor();
    switch (action)
    {
    case Dapper::Save_Action::invalid:
    {

      break;
    }
    case Dapper::Save_Action::save_as:
    {
      ImGui::Begin("Save As");
      ImGui::SetWindowFontScale(1.60 * scale);
      ImGui::InputText("Name", &input_scene);
      if (ImGui::Button("Confirm"))
      {
        if (!input_scene.empty())
        {
          SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(my_parent.GetEngine(), "SceneManager");
          scene_man.GetScene(scene_names[current_scene]).Save(input_scene);
          action = Save_Action::invalid;
          input_scene.clear();
        }
      }
      ImGui::End();
      break;
    }
    case Dapper::Save_Action::load:
    {
      ImGui::Begin("Load");
      ImGui::SetWindowFontScale(1.60 * scale);
      ImGui::InputText("Name", &input_scene);
      if (ImGui::Button("Confirm"))
      {
        if (!input_scene.empty())
        {
          SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(my_parent.GetEngine(), "SceneManager");
          if (scene_man.GetSceneID(input_scene) == -1)
          {
            scene_man.LoadScene(input_scene);
          }
          action = Save_Action::invalid;
          input_scene.clear();
        }
      }
      ImGui::End();
      break;
    }
    case Dapper::Save_Action::Unload:
    {
      SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(my_parent.GetEngine(), "SceneManager");
      scene_man.UnloadScene(scene_names[current_scene]);
      action = Save_Action::invalid;
      break;
    }
    case Dapper::Save_Action::new_scene:
    {
      ImGui::Begin("New");
      ImGui::SetWindowFontScale(1.60 * scale);
      ImGui::InputText("Name", &input_scene);
      if (ImGui::Button("Confirm"))
      {
        if (!input_scene.empty())
        {
          SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(my_parent.GetEngine(), "SceneManager");
          scene_man.GetNew(input_scene);
          action = Save_Action::invalid;
          input_scene.clear();
        }
      }
      ImGui::End();
      break;
    }
    }
  }

  void SaveMenu::Save(const char* file_path)
  {
    GameObjectManager& the_man = *GetSystemFromEngine<GameObjectManager*>(my_parent.GetEngine(), "GameObjectManager");
    Audio& the_audio = *GetSystemFromEngine<Audio*>(my_parent.GetEngine(), "Audio");
    JsonWriter writer;
    writer.StartObject();
    the_man.WriteAllGameObjects(writer);
    the_audio.WriteAllAudioObjects(writer);
    writer.EndObject();
    writer.WriteToFile(file_path);
  }



} // End Of Dapper Namespace