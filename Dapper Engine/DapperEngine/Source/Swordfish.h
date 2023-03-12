//------------------------------------------------------------------------------
//
// File Name:	Swordfish.h
// Author(s):	Isaiah
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
#include "GameObject.h"
#include "Transform.h"
#include "Physics.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper
{
  enum class swordfish_state { Invalid, Idle, Attack };
  class Swordfish : public IStateMachine<swordfish_state>
  {
  public:
    Swordfish(int ID, Engine& engine);

    void Init();
    inline int GetParentID() const override { return parent_ID; }
    inline void SetParentID(int ID) override { parent_ID = ID; }
    inline const std::string& GetName() const override { return name; }
    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal);
    inline void SetID(int ID) override { my_ID = ID; }
    void Write(IJsonWriter& writer) const override;
    void ImGuiDisplay(float scale) override;
    inline const int GetID() const override { return my_ID; }
    void OnExit();
    void OnInit();
    void OnUpdate(float dt);
    inline bool UpdateWhilePaused() const{ return false; }

    void ResolutionFunction(IComponent* object);

  private:
    static inline const std::string name = "Swordfish";
    int my_ID;
    int parent_ID = -1;
    Engine& my_engine;

    swordfish_state state;
    float engage_radius = 500;
    float stop_radius = 20;
    float speed = 0;

    void StopFollow();
    void FollowTarget();
    bool LookforTarget();
    const GameObject* GetTarget();
    void SpriteFlip(Physics& physics, Transform& transform);

    std::string fileName = "";
    float volume = 0.5;
    //const GameObject* player = nullptr;
  };


} // End Of Dapper Namespace

//------------------------------------------------------------------------------


