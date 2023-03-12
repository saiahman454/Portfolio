//------------------------------------------------------------------------------
//
// File Name:	ComponentWindow.cpp
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "ComponentWindow.h"
#include "LevelEditor.h"
#include "GameObjectListBox.h"
#include "Utility.h"
#include "TransformManager.h"
#include "imgui_stdlib.h"
#include "ComponentRegistry.h"
// Dapper Namespace!
namespace Dapper {
  /*--------------------------------------
  Description: constructor for ComponentWindow
  Params:
  parent - the parent level editor
  ---------------------------------------*/
  ComponentWindow::ComponentWindow(LevelEditor& parent) :
    my_parent(parent), my_engine(parent.GetEngine())
  {

  }

  /*--------------------------------------
  Description: Init for the window
  ---------------------------------------*/
  void ComponentWindow::Init()
  {
    ComponentRegistry& reg = *GetSystemFromEngine<ComponentRegistry*>(my_engine, "ComponentRegistry");
    auto& comp_list = reg.GetCompList();

    for (int i = 0; i < (int)comp_list.size(); ++i)
    {
      list.push_back(comp_list[i].data());
    }

  }

  /*--------------------------------------
  Description: Render function
  ---------------------------------------*/
  void ComponentWindow::Render()
  {
    float scale = my_parent.GetScaleFactor();
    if (scale) {
      GameObjectListBox& list_box = dynamic_cast<GameObjectListBox&>(*my_parent.GetWindow("GameObjectListBox"));
      int current_obj = list_box.GetCurrentObject();
      GameObjectManager& the_man = *GetSystemFromEngine<GameObjectManager*>(my_engine, "GameObjectManager");
      ComponentRegistry& reg = *GetSystemFromEngine<ComponentRegistry*>(my_engine, "ComponentRegistry");

      if (current_obj != -1)
      {
        GameObject& obj = the_man[current_obj];
        auto map = obj.GetMap();
        //start of main window
        //ImGui::SetNextWindowPos({ 0,0 });
        //ImGui::SetWindowSize({ 0 * scale,0 * scale }, 0);
        ImGui::Begin(obj.GetName().c_str());
        ImGui::SetWindowFontScale(1.70 * scale);

        ImGui::InputText("Name", &obj_name);
        if (ImGui::Button("Set Name"))
        {
          obj.SetName(obj_name);
          obj_name.clear();
        }

        ImGui::Text("Current flag :");
        ImGui::Text(obj.GetFlag().c_str());
        ImGui::InputText("Flag", &obj_flag);
        if (ImGui::Button("Set Flag"))
        {
          obj.SetFlag(obj_flag.c_str());
          obj_flag.clear();
        }

        bool is_enabled = obj.isEnabled();
        if (ImGui::Checkbox("Enabled", &is_enabled))
        {
            obj.setEnabled(is_enabled);
        }

        if (ImGui::Button("New Component", { 220 * scale,45 * scale }))
        {
          new_open = true;
          delete_open = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Delete Component", { 220 * scale,45 * scale }))
        {
          delete_open = true;
          new_open = false;
        }
        //drop down menu
        //ImGui::SetWindowSize({ 550 * scale,400 * scale }, 0);
        if (ImGui::CollapsingHeader("Components"))
        {

          for (const auto& pair : map)
          {
            if (ImGui::TreeNode(pair.first.data()))
            {
              IComponent* comp = reg.GetComponent(pair.first, pair.second);
              if (comp)
              {
                comp->ImGuiDisplay(scale);
              }
              ImGui::TreePop();
            }
          }
        }
        ImGui::End();
        //end of main window

        //Start of the delete pop up
        if (delete_open)
        {

          //settings 
          ImGui::Begin("Are you sure?", &delete_open);
          ImGui::SetWindowFontScale(2.0f * scale);
          //ImGui::SetWindowSize({ 400,150 }, 0);
          ImGui::Text("Component Name");
          ImGui::Combo("Things", &curr, list.data(), list.size(), 5);
          //deletion of components
          if (ImGui::Button("Comfirm##del"))
          {
            if (curr > -1)
            {
              auto iter = map.find(list[curr]);
              if (iter != map.end())
              {
                if (iter->first != "Transform")
                {
                  obj.RemoveComponent(iter->first);
                  reg.Destroy(iter->first, iter->second);
                }
              }
            }
          }
          ImGui::End();
        }
        //end of delete pop up
        if (new_open)
        {

          ImGui::Begin("New Component!", &new_open);
          ImGui::SetWindowFontScale(2.0f * scale);
          //ImGui::SetWindowSize({ 400 * scale,150 * scale }, 0);
          //ImGui::InputText(" ", &comp_new);
          ImGui::Combo("things", &curr_new, list.data(), list.size(), 5);
          if (ImGui::Button("Comfrim##new"))
          {
            if (curr_new != -1)
            {
              auto iter = map.find(list[curr_new]);
              if (iter == map.end())
              {
                IComponent* comp = reg.Construct(list[curr_new]);
                if (comp)
                {
                  obj.AddComponent(list[curr_new], comp->GetID());
                  comp->SetParentID(obj.GetID());
                  comp->Init();
                }
              }
            }
          }



          ImGui::End();
        }
      }
    }
  }

  /*--------------------------------------
  Description: Destructor
  ---------------------------------------*/
  ComponentWindow::~ComponentWindow()
  {
  }




} // End Of Dapper Namespace