//------------------------------------------------------------------------------
//
// File Name:	FpsWindow.h
// Author(s):	
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "LevelEditor.h"
#include "IimguiWindow.h"
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class FpsWindow : public ImGuiWindow
  {
  public:

    FpsWindow(LevelEditor& parent_);
    ~FpsWindow();
    // Inherited via ImGuiWindow
    void Init() override;
    void Render() override;
    const std::string& GetWindowName() override;
  private:
    static inline const std::string my_name = "FpsWindow";
    LevelEditor& parent;
  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


