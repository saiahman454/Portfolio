//------------------------------------------------------------------------------
//
// File Name:	MassChangeWindow.h
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
  class MassChangeWindow : public ImGuiWindow
  {
  public:

    MassChangeWindow(LevelEditor& parent_);
    ~MassChangeWindow();
    // Inherited via ImGuiWindow
    void Init() override;
    void Render() override;
    const std::string& GetWindowName() override;
  private:
    static inline const std::string my_name = "MassChangeWindow";
    LevelEditor& parent;
    std::vector<std::string> components_string;
    std::vector<const char*> components;
    int current_item = -1;
    int last_item = -1;
  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


