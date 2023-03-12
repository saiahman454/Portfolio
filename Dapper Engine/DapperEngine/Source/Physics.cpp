//------------------------------------------------------------------------------
//
// File Name:	Physics.cpp
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Physics.h"
#include "Utility.h"
#include "Transform.h"
#include "glm/vec3.hpp"
#include "imgui.h"

namespace Dapper
{
  /*--------------------------------------
  Description: Physics constructor
  Params:
  engine - the parent engine for the tile
  ---------------------------------------*/
  Physics::Physics(Engine* engine) :
    my_engine(engine),
      paused(false)
  {
  }

  /*--------------------------------------
  Description: Physics destructor
  ---------------------------------------*/
  Physics::~Physics()
  {
  }
  /*--------------------------------------
  Description: Initalize function
  ---------------------------------------*/
  void Physics::Init()
  {
  }
  /*--------------------------------------
  Description: Update the physics component 
  using basic kinematic functions
  Params: dt - Delta time
  ---------------------------------------*/
  void Physics::Update(float dt)
  {

    Transform& transform = *GetComponentFromParent<Transform>(*my_engine, my_parent_ID, "Transform");
    glm::vec3 result = transform.GetTranslation();
    old_pos = result;
    glm::vec2 newTranslation;
    float drag_co = glm::clamp((1 - (drag * dt / 100.0f)), 0.0f, 1.0f);
    acceleration *= drag_co;

    velocity = drag_co *((acceleration * dt) + velocity);
    newTranslation = (velocity * dt) + glm::vec2(result);
    transform.setRotation(transform.GetRotation() + (rotation_velocity * dt));

    transform.setTranslation(glm::vec3(newTranslation, result.z));

  }


  /*--------------------------------------
  Description: Reads component from file
  Params:
  deseial - unused
  jsonVal - the json object
  ---------------------------------------*/
  void Physics::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial;
    auto object = jsonVal.GetObject();
    auto vel = object["Velocity"].GetArray();
    velocity = { vel[0].GetFloat(), vel[1].GetFloat() };
    auto acc = object["Acceleration"].GetArray();
    acceleration = { acc[0].GetFloat(), acc[1].GetFloat() };
    rotation_velocity = object["Rotation"].GetFloat();
    restitution = object["Restitution"].GetFloat();
    mass = object["Mass"].GetFloat();

    // Friendly Reminder that if mass is 0, it's infinite mass.

  }

  /*--------------------------------------
  Description: writes object to a file
  Params:
  writer - custom writer class
  ---------------------------------------*/
  void Physics::Write(IJsonWriter& writer) const
  {
    writer.WriteKey("Physics");
    writer.StartObject();
    writer.WriteVec2("Velocity", velocity.x, velocity.y);
    writer.WriteVec2("Acceleration", acceleration.x, acceleration.y);
    writer.WriteFloat("Rotation", rotation_velocity);
    writer.WriteFloat("Restitution", restitution);
    writer.WriteFloat("Mass", mass);
    writer.WriteInt("Drag", drag);
    writer.EndObject();
  }

  /*--------------------------------------
  Description: level editor display
  Params:
  scale - screen resolution
  ---------------------------------------*/
  void Physics::ImGuiDisplay(float scale)
  {
    scale;
    ImVec2 cursor = ImGui::GetCursorPos();
    ImVec2 size(148 * scale, 35 * scale);
    if (ImGui::Button("+##X", size))
    {
      velocity.x += 50;
    }
    ImGui::SameLine(size.x+cursor.x+(4*scale));
    if (ImGui::Button("+##Y", size))
    {
      velocity.y += 50;
    }
    ImGui::PushItemWidth(300*scale);
    ImGui::InputFloat2("Velocity", &velocity[0]);
    ImGui::PopItemWidth();
    if (ImGui::Button("-##X", size))
    {
      velocity.x -= 50;
    }
    cursor = ImGui::GetCursorPos();
    ImGui::SameLine(size.x+cursor.x+(4*scale));
    if (ImGui::Button("-##Y", size))
    {
      velocity.y -= 50;
    }


    ImGui::NewLine();

    if (ImGui::Button("+##X", size))
    {
      velocity.x += 50;
    }
    cursor = ImGui::GetCursorPos();
    ImGui::SameLine(size.x+cursor.x+(4*scale));
    if (ImGui::Button("+##Y", size))
    {
      velocity.y += 50;
    }
    ImGui::PushItemWidth(300 * scale);
    ImGui::InputFloat2("Acceleration", &acceleration[0]);
    ImGui::PopItemWidth();
    if (ImGui::Button("-##X", size))
    {
      velocity.x -= 50;
    }
    cursor = ImGui::GetCursorPos();
    ImGui::SameLine(size.x+cursor.x+(4*scale));
    if (ImGui::Button("-##Y", size))
    {
      velocity.y -= 50;
    }
    ImGui::NewLine();
    ImGui::PushItemWidth(300 * scale);
    ImGui::InputFloat("Mass", &mass, 50.0f);
    ImGui::InputFloat("Bouncyness", &restitution, 0.1f);
    ImGui::InputInt("Drag", &drag, 10);
    ImGui::PopItemWidth();

  }

  /*--------------------------------------
  Description: Gets the inverse mass
  ---------------------------------------*/
  const float Physics::GetInverseMass()
  {
    if (mass == 0)
      return 0;
    else
      return (1 / mass);
  }

}