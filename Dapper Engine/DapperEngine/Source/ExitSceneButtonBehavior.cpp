//------------------------------------------------------------------------------
//
// File Name:	ExitSceneButtonBehavior.cpp
// Author(s):	Natalie Bouley
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#include "ExitSceneButtonBehavior.h"
#include "BGMController.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Utility.h"
#include "Input.h"
#include "SceneManager.h"
#include "Engine.h"
#include "Transform.h"

// Dapper Namespace!
namespace Dapper {
    ExitSceneButtonBehavior::ExitSceneButtonBehavior(int ID_, Engine& engine_)
        : Button(ID_, engine_), engine(engine_)
    {
    }

    ExitSceneButtonBehavior::~ExitSceneButtonBehavior()
    {
    }


    void ExitSceneButtonBehavior::Init()
    {
        //Bind connects a function and a given this pointer / parameters(std::placeholder)
        Button::onReleased.RegisterMessage
        (std::bind(&ExitSceneButtonBehavior::ChangeGameState, this));
    }

    const std::string& ExitSceneButtonBehavior::GetName() const
    {
        return name;
    }

    void ExitSceneButtonBehavior::Update(float dt)
    {
        dt;
        Button::Update(dt);

    }

    void ExitSceneButtonBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
    {
        deserial, jsonVal;

        auto obj = jsonVal.GetObject();
        Button::Read(deserial, jsonVal);

    }

    void ExitSceneButtonBehavior::Write(IJsonWriter& writer) const
    {
        // Write all of the values to the json file.
        writer.WriteKey("ControlsButtonBehavior");
        writer.StartObject();
        //Adam add for sprites
        Button::Write(writer);
        writer.EndObject();
    }

    void ExitSceneButtonBehavior::ImGuiDisplay(float scale)
    {
        // Display editable values as float inputs.
        scale;

    }

    void ExitSceneButtonBehavior::OnSceneExit() const
    {
        // Exit scene, pop scene states
        SceneManager& state =
            *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
        int toUnload = state.SceneInCharge(GetParentID());

        state.UnloadScene(toUnload);
        state.PopSceneStates();

        BGMController& bgm = *GetSystemFromEngine<BGMController*>(engine, "BGMController");
        bgm.SetPaused(0);

        Input& input = *GetSystemFromEngine<Input*>(engine, "Input");

        input.setPause(true);
    }

    void ExitSceneButtonBehavior::ChangeGameState() const
    {
        OnSceneExit();
    }

} // End Of Dapper Namespace