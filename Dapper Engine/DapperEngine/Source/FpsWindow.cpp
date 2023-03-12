//------------------------------------------------------------------------------
//
// File Name:	FpsWindow.cpp
// Author(s):	
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "FpsWindow.h"
#include "Time.h"
#include "Utility.h"

// Dapper Namespace!
namespace Dapper {

    FpsWindow::FpsWindow(LevelEditor& parent_) 
        : parent(parent_)
    {
    }

    FpsWindow::~FpsWindow()
    {
    }

  void FpsWindow::Init()
  {
  }

  void FpsWindow::Render()
  {
      Time& time = *GetSystemFromEngine<Time*>(parent.GetEngine(), "Time");
      float scale = parent.GetScaleFactor();

      ImGui::SetNextWindowPos({ 1200 * scale, 0 * scale });
      ImGui::Begin("Diagnostics");
      ImGui::SetWindowFontScale(1.5f);
      //ImGui::SetWindowSize({ 400 * scale, 100 * scale }, 0);
      ImGui::Text("FPS: %.1f", time.GetFPS());
      ImGui::Text("Time per frame %.3f milliseconds", 1000.0f / time.GetFPS());


      ImGui::End();
  }

  const std::string& FpsWindow::GetWindowName()
  {
    return my_name;
  }

} // End Of Dapper Namespace