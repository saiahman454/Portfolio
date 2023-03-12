//------------------------------------------------------------------------------
//
// File Name:	ComponentWindow.h
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
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class ComponentWindow : public ImGuiWindow
  {
  public:
    ComponentWindow(LevelEditor& parent);

    void Init()override;

    void Render()override;

     ~ComponentWindow()override;

     inline const std::string& GetWindowName(){ return name; };
  private:
    LevelEditor& my_parent;
    Engine& my_engine;
    std::string name = "ComponentWindow";
    bool open = true;
    bool delete_open = false;
    bool new_open = false;
    std::string del_name;
    std::string comp_new;
    std::string obj_name;
    std::string obj_flag;

    int curr = -1;
    int curr_new = -1;
    std::vector<const char*> list;
  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


