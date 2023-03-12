//------------------------------------------------------------------------------
//
// File Name:	LevelEditor.h
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
#include "ISystem.h"
#include "Engine.h"
#include "imgui.h"
#include "IimguiWindow.h"
#include "Input.h"
//------------------------------------------------------------------------------


// Dapper Namespace!
namespace Dapper
{
  class LevelEditor : public ISystem
  {
  public:
    LevelEditor(Engine& engine);
    ~LevelEditor()override;

    LevelEditor(const LevelEditor& copy) = delete;

    inline const std::string& Name()const override{ return name; };

    void Initialize()override;

    void Update(float dt) override;

    void Render() override;

    void AddWindow(ImGuiWindow* new_window);

    ImGuiWindow* GetWindow(std::string window_name);

    inline Engine& GetEngine(){ return my_engine; };

    inline float GetScaleFactor(){ return window_scale; };



    inline const std::vector<mouse_button>& GetGuiMouseInput()const{ return gui_mouse; };

  private:
    void OnResizeCalcScale(int width, int height);
    void OnRelease(int button);
    void OnTrigger(int button);
    const std::string name = "LevelEditor";
    Engine& my_engine;
    bool on_off = true;
    std::vector<ImGuiWindow*> windows;
    glm::vec2 scroll_zoom = {0,0};
    float window_scale = 1;
    
    std::vector<mouse_button> gui_mouse;

    // Inherited via ISystem
    inline virtual bool UpdateWhilePaused() const override{ return false; };
  };//End of LevelEditor class

} // End Of Dapper Namespace