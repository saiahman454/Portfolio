//------------------------------------------------------------------------------
//
// File Name:	LoadSceneButtonBehavior.cpp
// Author(s):	Natalie Bouley
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#include "LoadSceneButtonBehavior.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Utility.h"
#include "Input.h"
#include "SceneManager.h"
#include "Engine.h"
#include "Transform.h"

// Dapper Namespace!
namespace Dapper {
    LoadSceneButtonBehavior::LoadSceneButtonBehavior(int ID_, Engine& engine_)
        : Button(ID_, engine_), engine(engine_)
    {
    }

    LoadSceneButtonBehavior::~LoadSceneButtonBehavior()
    {
    }


    void LoadSceneButtonBehavior::Init()
    {
        //Bind connects a function and a given this pointer / parameters(std::placeholder)
        Button::onReleased.RegisterMessage
        (std::bind(&LoadSceneButtonBehavior::ChangeGameState, this));
    }

    const std::string& LoadSceneButtonBehavior::GetName() const
    {
        return name;
    }

    void LoadSceneButtonBehavior::Update(float dt)
    {
        dt;
        Button::Update(dt);

    }

    void LoadSceneButtonBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
    {
        deserial, jsonVal;

        auto obj = jsonVal.GetObject();
        gameState = jsonVal["GameState"].GetString();
        Button::Read(deserial, jsonVal);

    }

    void LoadSceneButtonBehavior::Write(IJsonWriter& writer) const
    {
        // Write all of the values to the json file.
        writer.WriteKey("ControlsButtonBehavior");
        writer.StartObject();
        writer.WriteString("GameState", gameState.c_str());
        //Adam add for sprites
        Button::Write(writer);
        writer.EndObject();
    }

    void LoadSceneButtonBehavior::ImGuiDisplay(float scale)
    {
        // Display editable values as float inputs.
        scale;
        ImGui::InputText("GameState", &gameState);

    }

    void LoadSceneButtonBehavior::OnSceneLoad() const
    {
        // Push scene states, pause all scenes, load the gamestate
        SceneManager& state =
            *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
        state.PushSceneStates();
        state.PauseAllScenes();
        state.LoadScene(gameState);
    }

    void LoadSceneButtonBehavior::ChangeGameState() const
    {
        OnSceneLoad();
    }

} // End Of Dapper Namespace