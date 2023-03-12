//------------------------------------------------------------------------------
//
// File Name:	SpriteWindow.cpp
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "SpriteWindow.h"
#include "LevelEditor.h"
#include "imgui.h"
#include "GameObjectListBox.h"
// Dapper Namespace!
namespace Dapper {



  SpriteWindow::SpriteWindow(LevelEditor& parent) :
    my_parent(parent)
  {

  }

  void SpriteWindow::Init()
  {

  }

  void SpriteWindow::Render()
  {
    GameObjectListBox& list_box = dynamic_cast<GameObjectListBox&>(*my_parent.GetWindow("GameObjectListBox"));
    if (list_box.GetCurrentItem() == 1)
    {
    ImGui::SetNextWindowPos({ 300,300 });
    ImGui::Begin("Sprite Window");
    ImGui::SetWindowFontScale(1.5f);
    ImGui::SetWindowSize({ 300,150 }, 0);
    ImGui::CollapsingHeader("blah");

    ImGui::End();
    }

  }

  SpriteWindow::~SpriteWindow()
  {
  }

  std::string SpriteWindow::GetWindowName()
  {
    return name;
  }

} // End Of Dapper Namespace