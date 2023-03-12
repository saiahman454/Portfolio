//------------------------------------------------------------------------------
//
// File Name:	SwordfishMissle.h
// Author(s):	
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
#include "IStateMachine.h"
#include "glm/vec3.hpp"
#include "Audio.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  enum class swordfishMissle_state { Invalid, Idle,Warning,Attack };
  class SwordfishMissle : public IStateMachine<swordfishMissle_state>
  {
  public:
    SwordfishMissle(int ID, Engine& engine);

    // Inherited via IStateMachine
    void Init() override;
    int GetParentID() const override;
    void SetParentID(int ID) override;
    const std::string& GetName() const override;
    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;
    void SetID(int ID) override;
    void Write(IJsonWriter& writer) const override;
    void ImGuiDisplay(float scale) override;
    const int GetID() const override;

    // Player Collision Resolution
    void ResolutionFunction(IComponent* object);

    void OnExit() override;
    void OnInit() override;
    void OnUpdate(float dt) override;

    bool UpdateWhilePaused() const override;
    const int GetTarget();
  private:
    const std::string name = "SwordfishMissle";
    int my_ID;
    int parent_ID;
    Engine& my_engine;

    swordfishMissle_state state;

    bool triggerd = false;
    int Target_ID = -1;

    int warning_ID = -1;
    glm::vec3 last_warning_pos;
    float self_destruct_timer = 5;
    //Imgui errors

    std::string fileName = "";
    float volume = 0.5;

    std::shared_ptr<Sound> sound;
    std::shared_ptr<Sound> warningSound;
    std::shared_ptr<Sound> dashSound;

    float trigger = 0;
    float moving_timer = 2;
    float stay_timer = 1;
    float warning_lerp = 500;
    float speed = 1000;

  };

} // End Of Dapper Namespace

//------------------------------------------------------------------------------


