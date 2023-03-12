//------------------------------------------------------------------------------
//
// File Name:	SaveMenu.h
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
//------------------------------------------------------------------------------
#include "IimguiWindow.h"
#include "LevelEditor.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  enum class Save_Action {invalid, save_as, load, new_scene, Unload };
  class SaveMenu : public ImGuiWindow
  {
  public:
    SaveMenu(LevelEditor& parent);
    ~SaveMenu();
    // Inherited via ImGuiWindow
    void Init() override;
    void Render() override;
    inline const std::string& GetWindowName() override{ return my_name; };

    void CalcSceneNames();
    std::string_view GetCurrentScene();
    void SaveActionsDisplay();
  private:
    static inline const std::string my_name = "Save Window";
    LevelEditor& my_parent;
    char filepath[50] = {};
    char gamestate[50] = {};
    bool auto_save = false;
    float save_timer;

    //new things for scenes
    std::vector<const char*> scene_names;
    int current_scene;
    void Save(const char* filepath);
    Save_Action action = Save_Action::invalid;
    std::string input_scene;
  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


