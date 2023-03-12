//------------------------------------------------------------------------------
//
// File Name:	GameObjectListBox.cpp
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "GameObjectListBox.h"
#include "Utility.h"
#include "imgui_stdlib.h"
#include "Window.h"
#include "TransformManager.h"
#include "PhysicsManager.h"
#include "ColliderManager.h"
#include <assert.h>
#include "Renderer.h"
#include <iostream>
#include "Tile.h"
#include "SaveMenu.h"
#include "SceneManager.h"
// Dapper Namespace!
namespace Dapper {
  /*--------------------------------------
  Description: List box constructor
  Params: parent - the parent level editor
  ---------------------------------------*/
  GameObjectListBox::GameObjectListBox(LevelEditor& parent) :
    my_parent(parent),
    my_engine(parent.GetEngine()),
    obj_manager(GetSystemFromEngine<GameObjectManager*>(my_engine, "GameObjectManager")),
    name_confirm(false)
  {

  }
  /*--------------------------------------
  Description: List box destuctor
  ---------------------------------------*/
  GameObjectListBox::~GameObjectListBox()
  {

  }
  /*--------------------------------------
  Description: List box init function
  ---------------------------------------*/
  void GameObjectListBox::Init()
  {
    //average stacked objects is 8
    selected.reserve(8);
  }
  /*--------------------------------------
  Description: List box display/render function
  ---------------------------------------*/
  void GameObjectListBox::Render()
  {


    //check the see if any game object have been added
    DidListUpdate();
    //put names of all gameobjects into list
    for (int i = 0; i < (int)list.size(); ++i) {
      list[i] = (*obj_manager).GetObjectAtIndex(i)->GetName().c_str();
      obj_IDs[i] = (*obj_manager).GetObjectAtIndex(i)->GetID();
    }



    //start of main window
    float scale = my_parent.GetScaleFactor();
    if (scale) {
      //ImGui::SetNextWindowPos({ 700 * scale, 0 });
      ImGui::Begin("All Game Objects");
      ImGui::SetWindowFontScale(1.60f * scale);
      //ImGui::SetWindowSize({ 400 * scale,250 * scale }, 0);
      //start of things in window
      ImGui::Combo(" ", &current_item_index, list.data(), (int)list.size(), 4);
      //buttons

      ImGui::SameLine();
      if (ImGui::Button("Delete", { 100 * scale,45 * scale }))
      {
        if (current_item_index > -1)
        {
          SceneManager& scene_manager = *GetSystemFromEngine<SceneManager*>(my_parent.GetEngine(), "SceneManager");
          SaveMenu* save_menu = dynamic_cast<SaveMenu*>(my_parent.GetWindow("Save Window"));
          std::string_view scene_name = save_menu->GetCurrentScene();
          Scene& scene = scene_manager.GetScene(scene_name);
          scene.Remove(obj_IDs[current_item_index]);
          obj_manager->DeleteGameObject(obj_IDs[current_item_index]);
          current_item_index = -1;
        }
      }
      ImGui::Dummy({ 5.0f, 2.0f });

      if (tile_snapping)
      {
        ImGui::PushStyleColor(0, { 0,255,0,255 });
        if (ImGui::Button("Tile Snapping", { 220 * scale,45 * scale }))
        {
          tile_snapping = false;
        }
        ImGui::PopStyleColor();
        ImGui::SameLine();
      }
      else
      {
        ImGui::PushStyleColor(0, { 255,0,0,255 });
        if (ImGui::Button("Tile Snapping", { 220 * scale,45 * scale }))
        {
          tile_snapping = true;
        }
        ImGui::PopStyleColor();
        ImGui::SameLine();
      }
      if (ImGui::Button("Debug Drawing", { 220 * scale,45 * scale }))
      {
        GetSystemFromEngine<TransformManager*>(my_engine, "TransformManager")->ToggleDebugDraw();
        GetSystemFromEngine<PhysicsManager*>(my_engine, "PhysicsManager")->ToggleDebugDraw();
        GetSystemFromEngine<ColliderManager*>(my_engine, "ColliderManager")->ToggleDebugDraw();
      }
      ImGui::End();
    }

    LeftClickDrag();
    GameObjectStretch();
    //right click window
    RightClickObject();
    if (rightclicked_bool)
    {
      //Input& input = *GetSystemFromEngine<Input*>(my_engine, "Input");
      //glm::vec2 mousepos = input.GetMousePos();
      //ImGui::SetNextWindowPos({ mousepos.x,mousepos.y });
      ImGui::SetNextWindowPos(last_right_click);
      ImGui::Begin("Selected", &rightclicked_bool);
      ImGui::SetWindowSize({ 400 * scale,300 * scale });
      ImGui::SetWindowFontScale(2.0f * scale);
      if (ImGui::ListBox(" ###right click", &current_select, selected.data(), (int)selected.size(), 6))
      {

        auto iter = std::find(obj_IDs.begin(), obj_IDs.end(), selected_IDs[current_select]);
        //couldn't find current_item
        assert(iter != obj_IDs.end());
        current_item_index = iter - obj_IDs.begin();
        rightclicked_bool = false;
        selected.clear();
        selected_IDs.clear();
      }
      ImGui::End();
    }
    if (current_item_index != -1)
    {
      Renderer* drawer = GetSystemFromEngine<Renderer*>(my_engine, "Renderer");
      if (drawer)
      {
        GameObject* obj = (*obj_manager).GetObjectAtIndex(current_item_index);
        if (obj)
        {
          TransformManager& trans_man = *GetSystemFromEngine<TransformManager*>(my_engine, "TransformManager");
          Transform& trans = trans_man[obj->GetComponentID("Transform")];
          drawer->DebugSquare(trans.GetTranslation(), trans.GetScale(), glm::vec4(1, 0.729, 0.141, 1));

        }
      }
    }



  }


  /*--------------------------------------
  Description: Gets current selected gameobject
  ---------------------------------------*/
  int GameObjectListBox::GetCurrentObject()
  {
    if (current_item_index != -1 && obj_IDs.size() > current_item_index)
    {
      return obj_IDs[current_item_index];
    }
    else
    {
      return -1;
    }
  }

  /*--------------------------------------
  Description: checks if list changed size
  ---------------------------------------*/
  void GameObjectListBox::DidListUpdate()
  {
    list.resize(obj_manager->Size());
    obj_IDs.resize(obj_manager->Size());
  }

  /*--------------------------------------
  Description: allows you to right click a gameobject
  ---------------------------------------*/
  void GameObjectListBox::RightClickObject()
  {
    //grab the input class for the mouse
    Input& input = *GetSystemFromEngine<Input*>(my_engine, "Input");
    const std::vector<mouse_button>& mouse_button = my_parent.GetGuiMouseInput();
    if (mouse_button[GLFW_MOUSE_BUTTON_RIGHT] == mouse_button::MOUSE_TRIGGERED)
    {
      selected.clear();
      selected_IDs.clear();
      //grab mouse pos
      glm::vec2 mousepos = input.GetMousePosinWorldCoords();
      //get all gameobjects
      const std::vector<GameObject>& objs = obj_manager->GetAllGameObjects();
      //grab the registry
      ComponentRegistry& reg = *GetSystemFromEngine<ComponentRegistry*>(my_engine, "ComponentRegistry");

      const GameObject* selected_obj = nullptr;
      for (const GameObject& current : objs)
      {
        //grab the current gameobjects pos and size
        int ID = current.GetComponentID("Transform");
        Transform& trans = *dynamic_cast<Transform*>(reg.GetComponent("Transform", ID));
        glm::vec3 pos = trans.GetTranslation();
        glm::vec2 size = trans.GetScale();

        //box collison check
        if (mousepos.x > (pos.x - (glm::abs(size.x * .5))) &&
          mousepos.x < (pos.x + (glm::abs(size.x * .5))) &&
          mousepos.y >(pos.y - (glm::abs(size.y * .5))) &&
          mousepos.y < (pos.y + (glm::abs(size.y * .5))))
        {
          selected.push_back(current.GetName().c_str());
          selected_IDs.push_back(current.GetID());
          //if first object selcted

        }

      }
      if (selected_obj == nullptr)
      {
        current_item_index = -1;
      }
      rightclicked_bool = true;
      glm::vec2 screenpos = input.GetMousePos();
      last_right_click = { screenpos.x, screenpos.y };
      std::cout << "mousepos" << mousepos.x << " " << mousepos.y << std::endl;

    }
  }


  void GameObjectListBox::LeftClickDrag()
  {
    if (current_item_index != -1)
    {
      Input& input = *GetSystemFromEngine<Input*>(my_engine, "Input");
      const std::vector<mouse_button>& mouse_buttons = my_parent.GetGuiMouseInput();
      GameObject* obj = (*obj_manager).GetObjectAtIndex(current_item_index);
      if (obj)
      {
        TransformManager& trans_man = *GetSystemFromEngine<TransformManager*>(my_engine, "TransformManager");
        Transform& trans = trans_man[obj->GetComponentID("Transform")];
        glm::vec2 mousepos = input.GetMousePosinWorldCoords();

        if (mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] == mouse_button::MOUSE_TRIGGERED)
        {
          glm::vec3 transpos = trans.GetTranslation();
          glm::vec2 size = trans.GetScale();
          if (mousepos.x > (transpos.x - (size.x * .5)) &&
            mousepos.x < (transpos.x + (size.x * .5)) &&
            mousepos.y >(transpos.y - (size.y * .5)) &&
            mousepos.y < (transpos.y + (size.y * .5)))
          {
            glm::vec2 dist = { (trans.GetScale().x / 2) - 25, (trans.GetScale().y / 2) - 25 };
            dist.x = glm::trunc((transpos.x - mousepos.x) / dist.x);
            dist.y = glm::trunc((transpos.y - mousepos.y) / dist.y);
            if (dist.x == 0 && dist.y == 0)
            {
              relative_pos = mousepos - (glm::vec2)transpos;
              drag_bool = true;
            }

          }
        }


        if (drag_bool)
        {
          if (mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] == mouse_button::MOUSE_DOWN)
          {
            glm::vec3 pos = trans.GetTranslation();
            if (tile_snapping)
            {
              glm::vec2 rel_pos = mousepos - relative_pos;
              glm::vec2 scale = trans.GetScale();
              glm::vec3 test(floor((rel_pos.x - (scale.x / 2)) / 64.0f) * 64, floor((rel_pos.y - (scale.y / 2)) / 64) * 64, pos.z);
              glm::vec3 snapped = { test.x + (scale.x / 2),test.y + (scale.y / 2), pos.z };
              trans.setTranslation(snapped);
            }
            else
            {
              glm::vec2 move_distance = mousepos - relative_pos;
              trans.setTranslation({ move_distance.x,move_distance.y,pos.z });
            }
          }
        }
      }//obj check
      if (mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] == mouse_button::MOUSE_RELEASE)
      {
        drag_bool = false;
      }
    }
  }

  void GameObjectListBox::GameObjectStretch()
  {
    if (current_item_index != -1)
    {
      Input& input = *GetSystemFromEngine<Input*>(my_engine, "Input");
      const std::vector<mouse_button>& mouse_buttons = my_parent.GetGuiMouseInput();
      GameObject* obj = (*obj_manager).GetObjectAtIndex(current_item_index);
      if (obj)
      {
        glm::vec2 mousepos = input.GetMousePosinWorldCoords();
        TransformManager& trans_man = *GetSystemFromEngine<TransformManager*>(my_engine, "TransformManager");
        Transform& trans = trans_man[obj->GetComponentID("Transform")];
        if (mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] == mouse_button::MOUSE_TRIGGERED)
        {
          glm::vec2 size = trans.GetScale();
          glm::vec3 pos = trans.GetTranslation();
          if (mousepos.x > (pos.x - (size.x * .5)) &&
            mousepos.x < (pos.x + (size.x * .5)) &&
            mousepos.y >(pos.y - (size.y * .5)) &&
            mousepos.y < (pos.y + (size.y * .5)))
          {
            glm::vec2 dist = { (trans.GetScale().x / 2) - 10, (trans.GetScale().y / 2) - 10 };
            x_val = glm::trunc((mousepos.x - pos.x) / dist.x);
            y_val = glm::trunc((mousepos.y - pos.y) / dist.y);
            if (x_val != 0 || y_val != 0)
            {
              stretch = true;
              stretch_pos = mousepos;
              clicked_loc = mousepos;
            }
          }
        }
        if (stretch)
        {
          if (mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] == mouse_button::MOUSE_DOWN)
          {
            glm::vec2 dist = mousepos - stretch_pos;
            stretch_pos = mousepos;
            float max = 0;
            dist = { dist.x * x_val, dist.y * y_val };
            if (glm::abs(dist.x) > glm::abs(dist.y))
            {
              max = dist.x;
            }
            else
            {
              max = dist.y;
            }
            glm::vec2 scale = trans.GetScale();
            if (obj->GetComponentID("Tile") == -1)
            {
              trans.setScale(scale + max * glm::abs(glm::vec2(x_val, y_val)));
            }
            else
            {
              Tile& tile = *GetComponentFromParent<Tile>(my_engine, obj->GetID(), "Tile");
              glm::vec2 tile_dist = mousepos - clicked_loc;
              if (glm::abs(tile_dist.x) > 64 || glm::abs(tile_dist.y) > 64)
              {
                if (glm::abs(tile_dist.x) > glm::abs(tile_dist.y))
                {
                  if (tile_dist.x > 0)
                  {
                    tile.IncreaseX();
                    clicked_loc = mousepos;
                  }
                  else
                  {
                    tile.DecreaseX();
                    clicked_loc = mousepos;
                  }
                }
                else
                {
                  if (tile_dist.y > 0)
                  {
                    tile.DecreaseY();
                    clicked_loc = mousepos;
                  }
                  else
                  {
                    tile.IncreaseY();
                    clicked_loc = mousepos;
                  }
                }
              }
            }

          }
        }
        if (mouse_buttons[GLFW_MOUSE_BUTTON_LEFT] == mouse_button::MOUSE_RELEASE)
        {
          stretch = false;
        }

      }
    }
  }

} // End Of Dapper Namespace