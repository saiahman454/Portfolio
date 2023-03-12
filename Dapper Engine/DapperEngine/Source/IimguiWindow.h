//------------------------------------------------------------------------------
//
// File Name:	IimguiComponent.h
// Author(s):	Isaiah Dickison
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include <string>
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class ImGuiWindow
  {
  public:

    virtual void Init() = 0;

    virtual void Render() = 0;

    virtual ~ImGuiWindow() = 0 {};

    virtual const std::string& GetWindowName() = 0;
  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


