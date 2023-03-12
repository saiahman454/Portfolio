//------------------------------------------------------------------------------
//
// File Name:	SpriteWindow.h
// Author(s):	
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
  class SpriteWindow : public ImGuiWindow
  {
  public:
    SpriteWindow(LevelEditor& parent);

    void Init()override;

    void Render()override;

     ~SpriteWindow()override;

     std::string GetWindowName();
  private:
    LevelEditor& my_parent;
    std::string name = "SpriteWindow";
    bool open = true;
  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


