//------------------------------------------------------------------------------
//
// File Name:	GameObjectListBox.h
// Author(s):	Isaiah Dickison
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
#include "IimguiWindow.h"
#include "LevelEditor.h"
#include "GameObjectManager.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class GameObjectListBox : public ImGuiWindow
  {
  public :
    GameObjectListBox(LevelEditor& parent);

    //inherrited for IMGuiWindow
    ~GameObjectListBox()override;

    void Init()override;

    void Render()override;
    //end of inherrited functions

    int GetCurrentObject();

    inline const std::string& GetWindowName() { return name; }

    void DidListUpdate();



  private:
    inline static const std::string name = "GameObjectListBox";
    int current_item_index = -1;
    std::vector<const char*> list;
    std::vector<int> obj_IDs;
    LevelEditor& my_parent;
    Engine& my_engine;
    GameObjectManager* obj_manager;

    bool name_confirm;
    ImVec2 mouse_pos = { 0,0 };
    bool tile_snapping = false;
    bool stretch = false;

    int current_select = -1;
    std::vector<const char*> selected;
    std::vector<int> selected_IDs;
    ImVec2 last_right_click;
    bool rightclicked_bool;
    glm::vec2 relative_pos;
    glm::vec2 stretch_pos;
    bool drag_bool;
    void RightClickObject();
    void LeftClickDrag();
    void GameObjectStretch();


    float x_val;
    float y_val;
    glm::vec2 clicked_loc;

  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


