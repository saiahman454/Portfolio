//------------------------------------------------------------------------------
//
// File Name:	QuitButtonBehavior.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#include "QuitButtonBehavior.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Utility.h"
#include "Input.h"
#include "GameStateManager.h"
#include "Engine.h"
#include "Transform.h"

// Dapper Namespace!
namespace Dapper {
  QuitButtonBehavior::QuitButtonBehavior(int ID_, Engine& engine_)
    : Button(ID_, engine_), engine(engine_)
  {
  }

  QuitButtonBehavior::~QuitButtonBehavior()
  {
  }


  void QuitButtonBehavior::Init()
  {
      //Bind connects a function and a given this pointer / parameters(std::placeholder)
      Button::onReleased.RegisterMessage
      (std::bind(&QuitButtonBehavior::QuitGame, this));
  }

  const std::string& QuitButtonBehavior::GetName() const
  {
    return name;
  }

  void QuitButtonBehavior::Update(float dt)
  {
    dt;
    Button::Update(dt);

  }

  void QuitButtonBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial, jsonVal;

    auto obj = jsonVal.GetObject();
    Button::Read(deserial, jsonVal);
  }

  void QuitButtonBehavior::Write(IJsonWriter& writer) const
  {
    // Write all of the values to the json file.
    writer.WriteKey("ExitButtonBehavior");
    writer.StartObject();
    Button::Write(writer);
    writer.EndObject();
  }

  void QuitButtonBehavior::ImGuiDisplay(float scale)
  {
    // Display editable values as float inputs.
    scale;

  }

  void QuitButtonBehavior::QuitGame() const
  {
      engine.Stop();
  }

} // End Of Dapper Namespace