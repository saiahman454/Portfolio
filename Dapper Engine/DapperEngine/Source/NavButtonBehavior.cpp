//------------------------------------------------------------------------------
//
// File Name:	NavButtonBehavior.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#include "NavButtonBehavior.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Utility.h"
#include "Input.h"
#include "SceneManager.h"
#include "Engine.h"
#include "Transform.h"
#include "BGMController.h"
#include "Ambience.h"

// Dapper Namespace!
namespace Dapper {
    NavButtonBehavior::NavButtonBehavior(int ID_, Engine& engine_)
        : Button(ID_, engine_), engine(engine_)
    {
    }

    NavButtonBehavior::~NavButtonBehavior()
    {
    }


    void NavButtonBehavior::Init()
    {
        //Bind connects a function and a given this pointer / parameters(std::placeholder)
        Button::onReleased.RegisterMessage
        (std::bind(&NavButtonBehavior::ChangeGameState, this));
    }

    const std::string& NavButtonBehavior::GetName() const
    {
        return name;
    }

    void NavButtonBehavior::Update(float dt)
    {
        dt;
        Button::Update(dt);

    }

    void NavButtonBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
    {
        deserial, jsonVal;

        auto obj = jsonVal.GetObject();
        gameState = jsonVal["GameState"].GetString();
        Button::Read(deserial, jsonVal);

    }

    void NavButtonBehavior::Write(IJsonWriter& writer) const
    {
        // Write all of the values to the json file.
        writer.WriteKey("NavButtonBehavior");
        writer.StartObject();
        writer.WriteString("GameState", gameState.c_str());
        //Button::Write() //Adam add for sprites
        Button::Write(writer);
        writer.EndObject();
    }

    void NavButtonBehavior::ImGuiDisplay(float scale)
    {
        // Display editable values as float inputs.
        scale;
        ImGui::InputText("GameState", &gameState);

    }

    void NavButtonBehavior::ChangeGameState() const
    {
        SceneManager& state =
            *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");

        // if in pause and are going to credits fade to that
        int current_scene = state.SceneInCharge(GetParentID());
        std::string current_scene_name = state.GetScene(current_scene).GetName();

        // Fade when going to credits or gameplay scene
        if (gameState == "Credits" || gameState == "CreditsPause")
        {
            state.FadeToScene(current_scene_name, gameState, ConditionCheck::push);
        }
        else if (gameState == "Level1" || gameState == "Level2" || gameState == "Level3")
        {
            state.FadeToScene(current_scene_name, gameState, ConditionCheck::pop);
        }
        else
        {
            state.UnloadAllScenes();
            state.LoadScene(gameState);
            Ambience& ambience = *(GetSystemFromEngine<Ambience*>(engine, "Ambience"));
            ambience.AmbienceSetParameter(gameState);

            BGMController& bgmcontroller = *(GetSystemFromEngine<BGMController*>(engine, "BGMController"));
            bgmcontroller.BGMSetParameter(gameState);

            bgmcontroller.SetPaused(0);
            ambience.SetPaused(0);

        }
    }

} // End Of Dapper Namespace