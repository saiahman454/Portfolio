//------------------------------------------------------------------------------
//
// File Name:	FloatingHeartBehavior.h
// Author(s):	Adam Tackett
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

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class FloatingHeartBehavior : public IComponent
  {
  public:

     FloatingHeartBehavior(int ID_, Engine* engine_);

    ~FloatingHeartBehavior() override;

    void Init() override;

    inline int GetParentID() const override;

    inline void SetParentID(int ID) override;

    inline const std::string& GetName() const override;

    void Update(float dt) override;

    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;

    void Write(IJsonWriter& writer) const override;

    void ImGuiDisplay(float scale)override;

    inline  void SetID(int ID)override;

    inline const int GetID() const override;

    void SetDespawnTimer(float despawnTimer);
    inline void SetPause(bool Condition)override { paused = Condition; }
    inline bool GetPause() const override { return paused; }

  private:

    static inline const std::string name = "FloatingHeartBehavior";

    int my_ID; // The ID of the component in the behavior manager

    int parent_ID = -1; // The ID of the parent game object

    Engine* engine; // The engine

    float timer = 0.0f; // The speed of the heart
    void OnCollide(IComponent* comp);

    std::string fileName = "/SFX/Player/Player_PickupHealth";
    float volume = 0.5;
    bool paused = false;
  };
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


