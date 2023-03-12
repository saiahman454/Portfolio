//------------------------------------------------------------------------------
//
// File Name:	MassChangeWindow.cpp
// Author(s):	
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "MassChangeWindow.h"
#include "GameObjectListBox.h"
#include "GameObjectManager.h"
#include "Utility.h"


// Dapper Namespace!
namespace Dapper {



  MassChangeWindow::MassChangeWindow(LevelEditor& parent_) :
    parent(parent_)
  {
  }

  MassChangeWindow::~MassChangeWindow()
  {
  }

  void MassChangeWindow::Init()
  {
  }

  void MassChangeWindow::Render()
  {
    float scale = parent.GetScaleFactor();
    if (scale) {
      GameObjectListBox& list_box = dynamic_cast<GameObjectListBox&>(*parent.GetWindow("GameObjectListBox"));
      int current_obj = list_box.GetCurrentObject();
      GameObjectManager& the_man = *GetSystemFromEngine<GameObjectManager*>(parent.GetEngine(), "GameObjectManager");
      ComponentRegistry& reg = *GetSystemFromEngine<ComponentRegistry*>(parent.GetEngine(), "ComponentRegistry");
      if (current_obj != -1)
      {
        if (last_item != current_obj)
        {
          GameObject& obj = the_man[current_obj];
          const auto& map = obj.GetMap();
          for (const auto& pair : map)
          {
            components_string.push_back(pair.first.data());
          }
          last_item = current_obj;
          for (int i = 0; i < components_string.size(); ++i)
          {
            components.push_back(components_string[i].c_str());
          }
        }
      }
      else
      {
        components_string.clear();
        components.clear();
      }
      ImGui::Begin("Mass Change");
      ImGui::Combo("comp", &current_item, components.data(), components.size(), 4);

      ImGui::End();
    }
  }

  const std::string& MassChangeWindow::GetWindowName()
  {
    return my_name;
  }

} // End Of Dapper Namespace