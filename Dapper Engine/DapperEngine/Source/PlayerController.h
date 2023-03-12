//------------------------------------------------------------------------------
//
// File Name:	PlayerController.h
// Author(s):	Gavin Lu
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "IStateMachine.h"
#include "Input.h"
#include "Physics.h"
#include "Transform.h"
#include "Audio.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  // enum classes aren't implicitly convertible to ints
  enum class PlayerStates {PlayerInvalid, PlayerIdle, PlayerMove, PlayerDash, PlayerCutScene, PlayerDead};

  class PlayerController : public IStateMachine<PlayerStates>
  {
  public:

    PlayerController(int id, Engine & engineParam);

    ~PlayerController() override;

    void OnExit() override;

    void OnInit() override;

    void OnUpdate(float dt) override;

    void Init() override;

    inline int GetParentID() const override;

    bool GetInvincible();

    bool GetInvincibleFlag();

    float GetPlayerVel();

    inline void SetParentID(int ID) override;

    inline const std::string& GetName() const override;

    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;

    void Write(IJsonWriter& writer) const override;

    void ImGuiDisplay(float scale)override;

    inline void SetID(int ID)override;

    inline const int GetID() const override;

    void Decelerate(float dt);

    inline void UpdateRotation(float dt);
   
    void ResolutionFunction(IComponent* object);

    void SpriteFlip(Physics& physics, Transform& transform);

    void RegisterMessage(std::function<void()> function);

  private:
    inline bool UpdateWhilePaused()const;

    //void RegisterFunction(Messenger<MeshManager&>::func function);

    static inline const std::string name = "PlayerController";
    int my_ID;
    int parent_ID = -1;
    Engine& engine;
    Messenger<> on_collection{}; // Send a message to the UI when a collectible is collected

    // Initialized at read
    float acceleration_multiplier_per_sec = 0;
    float speed = 0;
    float max_speed = 0;
    float deceleration_multiplier = 0;
    float drift_speed = 0;
    float dash_drift_speed = 0;
    float dash_max_speed = 0;

    float max_dash_multiplier = 4;
    float dash_base_lasting_time = 0.25f;
    float dash_force = 25;

    float invoked_timer = 0;
    float death_timer = 1.0f;

    float level_end_trigger_x = 0.0f; // The threshold Squample has to pass to complete the level

    // Non const
    float acc = 1;
    glm::vec2 input_direction = { 0,0 };
    glm::vec2 move_direction = { 0,0 };
    bool dashing = false;
    glm::vec2 new_move_direction = { 1,0 };
    float vel = 0;

    float held_dash_time = 0;
    float invoke_timer = 0;

    bool can_dash = true;

    float invin_timer;
    float invin_duration = 1.0f;
    bool invincibility_flag = false;
    bool invincible = false;

    bool testbool = true;

    std::string fileName = "";
    float volume = 0.5;

    std::string lose_fileName = "";
    float lose_volume = 0.5;

    std::string dash_fileName = "";
    float dash_volume = 0.5;

    std::string tile_fileName = "";
    float tile_volume = 0.5;

    std::string move_fileName = "";
    float move_volume = 0.5;

    std::string collect_fileName = "";
    float collect_volume = 0.5;

    bool stop_moving = false;

    bool breachplayed = false;

    std::string current_scene = "";

    std::shared_ptr<Sound> movesound;

  };
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------
