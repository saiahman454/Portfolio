//------------------------------------------------------------------------------
//
// File Name:	Physics.h
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "IComponent.h"
#include "Engine.h"
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

namespace Dapper
{
  using string = std::string;
  using vec2 = glm::vec2;


  class Physics : public IComponent
  {
  public:

    Physics(Engine* engine);
    ~Physics();
    Physics& operator=(const Physics& copy) = default;

    void Init() override;
    void Update(float dt) override;
    inline int GetParentID() const override { return my_parent_ID; };
    inline void SetParentID(int ID)override{ my_parent_ID = ID; };
    inline const std::string& GetName() const override { return name; };
    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;

    //dont implement until level editor is up
    void Write(IJsonWriter& writer) const override;
    inline void SetID(int ID)override { my_ID = ID; };
    void ImGuiDisplay(float scale)override;
    inline const int GetID() const override { return my_ID; };

    inline void SetPause(bool Condition)override { paused = Condition; }
    inline bool GetPause() const override { return paused; }

    inline const vec2 GetVelocity(){ return velocity; };
    inline const vec2 GetAcceleration(){ return acceleration; };
    inline const float GetRotationalVelocity(){ return rotation_velocity; };
    inline const float GetRestitution(){ return restitution; };
    inline const float GetMass() const { return mass; };
    inline const glm::vec2 GetOldPos() { return old_pos; };
    const float GetInverseMass();

    inline void SetMass(float mass_param){ mass = mass_param; };
    inline void SetRestitution(float rest){ restitution = rest; };
    inline void SetVelocity(vec2 new_vel){ velocity = new_vel; };
    inline void SetAccelration(vec2 new_acc){ acceleration = new_acc; };
    inline void SetRotaionalVelocity(float new_rot){ rotation_velocity = new_rot; };
    //might not need after collision gut
    inline void AddVelocity(vec2 vel){ velocity += vel; };
    inline void AddAcceleration(vec2 acc){ acceleration += acc; };

    inline void AddRotationalVelocity(float num){ rotation_velocity += num; };

  private:
    inline static const string name = "Physics";
    Engine* my_engine;

    vec2 old_pos{ 0,0 };
    vec2 velocity{ 0,0 };
    vec2 acceleration{ 0,0 };
    float rotation_velocity = 0;
    int my_parent_ID = -1;
    int my_ID = -1;
    float restitution;
    float mass;
    int drag = 0;
    bool paused = false;
  };

}

