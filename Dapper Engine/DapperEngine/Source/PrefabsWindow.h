//------------------------------------------------------------------------------
//
// File Name:	PrefabsWindow.h
// Author(s):	Isaiah Dickison
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "IimguiWindow.h"
#include "LevelEditor.h"
#include "GameObjectManager.h"
#include <string.h>
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class PrefabsWindow : public ImGuiWindow
  {
  public:
    PrefabsWindow(LevelEditor& _parent);

    void Init()override;

    void Render()override;

    ~PrefabsWindow()override;

     inline const std::string& GetWindowName(){ return name; };
  private:
    LevelEditor& parent;
    std::string name = "PrefabsWindow";
    Engine& engine;
    GameObjectManager* object_manager;

    // Create a new game object from a prefab
    int CreateNewObjectFromPrefab(std::string const& prefab_name);

  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


