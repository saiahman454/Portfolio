//------------------------------------------------------------------------------
//
// File Name:	RestartBehavior.cpp
// Author(s):	Gavin Lu
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "RestartBehavior.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Utility.h"
#include "Input.h"
#include "SceneManager.h"
#include "Engine.h"

// Dapper Namespace!
namespace Dapper {
  RestartBehavior::RestartBehavior(int ID_, Engine& engine_)
    : my_ID(ID_)
    , engine(engine_)
  {
  }

  RestartBehavior::~RestartBehavior()
  {
  }


  void RestartBehavior::Init()
  {

  }

  int RestartBehavior::GetParentID() const
  {
    return parent_ID;
  }

  void RestartBehavior::SetParentID(int ID)
  {
    parent_ID = ID;
  }

  const std::string& RestartBehavior::GetName() const
  {
    return name;
  }

  void RestartBehavior::Update(float dt)
  {
    dt;
    Input& input = *dynamic_cast<Input*>(engine.Get("Input"));
    const std::vector<keyboard_key> keys = input.GetAllKeys();
    SceneManager& state =
      *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");

     if (state.GetSceneID("Lose") != -1)
    {
      if (keys[GLFW_KEY_SPACE] == keyboard_key::KEY_TRIGGERED)
      {
        state.UnloadAllScenes();
        state.LoadScene("Sandbox");//needs to be changed ADAM
      }
    }

    if (state.GetSceneID("Win") != -1)
    {
      if (keys[GLFW_KEY_SPACE] == keyboard_key::KEY_TRIGGERED)
      {
        state.UnloadAllScenes();
        state.LoadScene("Sandbox");//needs to be changed ADAM
      }
    }
  }

  void RestartBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial, jsonVal;

    auto obj = jsonVal.GetObject();


  }

  void RestartBehavior::Write(IJsonWriter& writer) const
  {
    // Write all of the values to the json file.
    writer.WriteKey("RestartBehavior");
    writer.StartObject();
    writer.EndObject();
  }

  void RestartBehavior::ImGuiDisplay(float scale)
  {
    // Display editable values as float inputs.
    scale;

  }

  void RestartBehavior::SetID(int ID)
  {
    my_ID = ID;
  }

  const int RestartBehavior::GetID() const
  {
    return my_ID;
  }

} // End Of Dapper Namespace