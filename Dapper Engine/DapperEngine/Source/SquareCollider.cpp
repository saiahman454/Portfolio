//------------------------------------------------------------------------------
//
// File Name:	SquareCollider.cpp
// Author(s): Gavin Lu
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "SquareCollider.h"
#include "Physics.h"
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Transform.h"
#include "Utility.h"
#include "glm/vec3.hpp"
#include "CircleCollider.h"
#include "imgui.h"
// Dapper Namespace!
namespace Dapper
{

  /*--------------------------------------
  Description: SquareCollider Constructor
  Return: Void
  ---------------------------------------*/
  SquareCollider::SquareCollider(Engine* eng)
    : ICollider(eng)
  {
  }

  /*--------------------------------------
  Description: Init function for square collider
  Return: Void
  ---------------------------------------*/
  void SquareCollider::Init()
  {

    SetPhysicsPointer(GetComponentFromParent<Physics>(*GetEngine(), GetParentID(), "Physics"));
    SetTransformPointer(GetComponentFromParent<Transform>(*GetEngine(), GetParentID(), "Transform"));

  }

  /*--------------------------------------
  Description: SquareCollider Update
  Params: float dt ¡V Time derivative!
  Return: Void
  ---------------------------------------*/
  void SquareCollider::Update(float dt)
  {
    dt;
  }

  /*--------------------------------------
  Description: Gets Component name, aka "Square Collider"
  Return: const std::string&
  ---------------------------------------*/
  const std::string& SquareCollider::GetName() const
  {
    return name_;
  }

  /*--------------------------------------
  Description: SquareCollider Deserialization/read
  Params:
    Engine& engine - engine for getting audio system.
    rapidjson::Value& val - value gotten from json
  Return: Void
  ---------------------------------------*/
  void SquareCollider::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {

    deserial;

    auto obj = jsonVal.GetObject();
    auto siz = obj["Size"].GetArray();
    this->size = glm::vec2(siz[0].GetFloat(), siz[1].GetFloat());
    auto ghost = obj["GhostCollider"].GetBool();
    this->ghost_collider = ghost;

  }

  /*--------------------------------------
  Description: SquareCollider writes to json for deserializing.
  Params:
    IJsonWriter& writer - Writer for writing
  Return: Void
  ---------------------------------------*/
  void SquareCollider::Write(IJsonWriter& writer) const
  {
    writer.WriteKey("SquareCollider");
    writer.StartObject();
    writer.WriteVec2("Size", size.x, size.y);
    writer.WriteBool("GhostCollider", ghost_collider);
    writer.EndObject();
  }

  /*--------------------------------------
  Description: SquareCollider gui display for editor.
  Params:
    float scale - scale
  Return: Void
  ---------------------------------------*/
  void SquareCollider::ImGuiDisplay(float scale)
  {
    scale;
    ImVec2 cursor = ImGui::GetCursorPos();
    ImVec2 button_size(148 * scale, 35 * scale);
    if (ImGui::Button("+##X", button_size))
    {
      size.x += 50;
    }
    ImGui::SameLine(button_size.x + cursor.x + (4 * scale));
    if (ImGui::Button("+##Y", button_size))
    {
      size.y += 50;
    }
    ImGui::PushItemWidth(300 * scale);
    ImGui::InputFloat2("Size", &size[0]);
    ImGui::PopItemWidth();
    if (ImGui::Button("-##X", button_size))
    {
      size.x -= 50;
    }
    cursor = ImGui::GetCursorPos();
    ImGui::SameLine(button_size.x + cursor.x + (4 * scale));
    if (ImGui::Button("-##Y", button_size))
    {
      size.y -= 50;
    }

    ImGui::Checkbox("Ghost Collider", &ghost_collider);

  }

  /*--------------------------------------
  Description: Gets component ID
  Return: const int
  ---------------------------------------*/
  const int SquareCollider::GetID() const
  {
    return my_id;
  }

  /*--------------------------------------
  Description: Sets component ID
  Params:
    int ID - ID you're setting
  Return: Void
  ---------------------------------------*/
  void SquareCollider::SetID(int ID)
  {
    my_id = ID;
  }

  /*--------------------------------------
  Description: Gets parent id of component
  Return: int
  ---------------------------------------*/
  int SquareCollider::GetParentID() const
  {
    return parent_id;
  }

  /*--------------------------------------
  Description: Returns if the collider is a ghost collider
  Return: bool
  ---------------------------------------*/
  bool SquareCollider::IsGhostCollider() const
  {
    return ghost_collider;
  }

  /*--------------------------------------
  Description: Sets if collider is ghost
  Params:
    bool is_ghost - if it's ghost
  Return: Void
  ---------------------------------------*/
  void SquareCollider::SetGhostCollider(bool is_ghost)
  {
    ghost_collider = is_ghost;
  }

  /*--------------------------------------
  Description: Gets the size of collider
  Return: glm::vec2
  ---------------------------------------*/
  glm::vec2 SquareCollider::GetSize() const
  {
    return size;
  }

  /*--------------------------------------
  Description: Sets size of collider
  Params:
    glm::vec2 new_size - new size of collider
  Return: Void
  ---------------------------------------*/
  void SquareCollider::SetSize(glm::vec2 new_size)
  {
    size = new_size;
  }

  /*--------------------------------------
  Description: Sets parent ID
  Params:
    int ID - ID you're setting
  Return: Void
  ---------------------------------------*/
  void SquareCollider::SetParentID(int ID)
  {
    parent_id = ID;
  }

  bool IsOverlapping(const glm::vec2* (a_points), const glm::vec2* b_points, glm::vec2 axis)
  {
    glm::vec2 a_vals(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
    glm::vec2 b_vals(std::numeric_limits<float>::max(), std::numeric_limits<float>::lowest());
    float dotproduct;
    for (int i = 0; i < 4; ++i)
    {
      dotproduct = glm::dot(a_points[i], axis);
      a_vals.x = glm::min(a_vals.x, dotproduct);
      a_vals.y = glm::max(a_vals.y, dotproduct);
      dotproduct = glm::dot(b_points[i], axis);
      b_vals.x = glm::min(b_vals.x, dotproduct);
      b_vals.y = glm::max(b_vals.y, dotproduct);
    }
    if (a_vals.x < b_vals.y && a_vals.y > b_vals.x)
    {
      return true;
    }


    return false;
  }


  /*--------------------------------------
  Description: Collider check when both colliders are square
  Params:
    SquareCollider& squarecolliderA - First component/collider
    SquareCollider& squarecolliderB - Second component/collider
    Engine& engine - Engine
    Physics& Physics1 - Physics Component of first collider
    Physics& Physics2 - Physics Component of second collider
    Transform& transformA - Transform Component of first collider
    Transform& transformB - Transform Component of second collider.
  Return: Void
  ---------------------------------------*/
  void SimpleAABBvsAABB(SquareCollider& squarecolliderA, SquareCollider& squarecolliderB, Engine& engine, Physics& Physics1,
    Physics& Physics2, Transform& transformA, Transform& transformB)
  {

    // Well if both object's mass is 0, it means they're both static, so we dont do the check.

    if (Physics1.GetMass() == 0 && Physics2.GetMass() == 0)
    {
      return;
    }

    // SETUP

    glm::vec2 translationA = transformA.GetTranslation();
    glm::vec2 translationB = transformB.GetTranslation();
    glm::vec2 sizeA = squarecolliderA.GetSize();
    glm::vec2 sizeB = squarecolliderB.GetSize();

    // Calculation normal
    glm::vec2 normal = translationB - translationA;

    // normal for resolution
    glm::vec2 resolutionNorm;


    /*glm::vec2 nRVa = glm::normalize(glm::rotate(glm::vec2(1, 0), glm::radians(transformA.GetRotation())));

    glm::vec2 rRVa(nRVa.y, -nRVa.x);

    glm::vec2 nRVb = glm::normalize(glm::rotate(glm::vec2(1, 0), glm::radians(transformB.GetRotation())));
    glm::vec2 rRVb(nRVb.y, -nRVb.x);

    glm::vec2 a_points[4] = {
    translationA + (nRVa + rRVa) * sizeA / 2.0f,
    translationA - (nRVa + rRVa) * sizeA / 2.0f,
    translationA + (nRVa - rRVa) * sizeA / 2.0f,
    translationA - (nRVa - rRVa) * sizeA / 2.0f,
    };
    glm::vec2 b_points[4] = {
    translationB + (nRVb + rRVb) * sizeB / 2.0f,
    translationB - (nRVb + rRVb) * sizeB / 2.0f,
    translationB + (nRVb - rRVb) * sizeB / 2.0f,
    translationB - (nRVb - rRVb) * sizeB / 2.0f,
    };
    bool gamer = false;
    if (IsOverlapping(a_points, b_points, nRVa)&& 
        IsOverlapping(a_points, b_points, rRVa)&&
        IsOverlapping(a_points, b_points, nRVb)&&
        IsOverlapping(a_points, b_points, rRVb)
      )
    {



      gamer = true;
    }*/

    // Calculate half extents along x axis for each object
    float a_extent = sizeA.x / 2;
    float b_extent = sizeB.x / 2;


    // Calculate overlap on x axis
    float x_overlap = a_extent + b_extent - abs(normal.x);

    // CALCULATION TIME!

    // SAT test on x axis
    if (x_overlap > 0)
    {
      // Calculate half extents along y axis for each object
      a_extent = sizeA.y / 2;
      b_extent = sizeB.y / 2;



      // Calculate overlap on y axis
      float y_overlap = a_extent + b_extent - abs(normal.y);

      // SAT test on y axis
      if (y_overlap > 0)
      {

        // Find out which axis is axis of least pen
        if (x_overlap < y_overlap)
        {

          // Point towards B knowing that n points from A to B
          if (normal.x < 0)
            resolutionNorm = glm::vec2(-1, 0);
          else
            resolutionNorm = glm::vec2(1, 0);
          squarecolliderB.ColliderSendMessage(&squarecolliderA);
          squarecolliderA.ColliderSendMessage(&squarecolliderB);
          if (!(squarecolliderA.IsGhostCollider() || squarecolliderB.IsGhostCollider()))
          {

            ImpulseResolution(&squarecolliderA, &squarecolliderB, engine, resolutionNorm, Physics1, Physics2);
            PositionalCorrection(&squarecolliderA, &squarecolliderB, engine, x_overlap, resolutionNorm, Physics1, Physics2, transformA, transformB);

          }

          // X and Y overlap on positional Correction is in the hundreds..why?
          return;

        }
        else
        {

          // Other way around!
          if (normal.y < 0)
            resolutionNorm = glm::vec2(0, -1);
          else
            resolutionNorm = glm::vec2(0, 1);
          squarecolliderB.ColliderSendMessage(&squarecolliderA);
          squarecolliderA.ColliderSendMessage(&squarecolliderB);

          if (!(squarecolliderA.IsGhostCollider() || squarecolliderB.IsGhostCollider()))
          {

            ImpulseResolution(&squarecolliderA, &squarecolliderB, engine, resolutionNorm, Physics1, Physics2);
            PositionalCorrection(&squarecolliderA, &squarecolliderB, engine, y_overlap, resolutionNorm, Physics1, Physics2, transformA, transformB);

          }

          return;

        }

      }

    }

  }


  /*--------------------------------------
  Description: Collider check when one is circle and one is square
    Params:
    SquareCollider& squareCollider - First component/collider
    CircleCollider& circleCollider - Second component/collider
    Engine& engine - Engine
    Physics& Physics1 - Physics Component of first collider
    Physics& Physics2 - Physics Component of second collider
    Transform& transformA - Transform Component of first collider
    Transform& transformB - Transform Component of second collider.
  Return: Void
  ---------------------------------------*/
  void AABBvsCircle(SquareCollider& squareCollider, CircleCollider& circleCollider, Engine& engine, Physics& Physics1,
    Physics& Physics2, Transform& transformA, Transform& transformB)
  {

    // Well if both object's mass is 0, it means they're both static, so we dont do the check.

    if (Physics1.GetMass() == 0 && Physics2.GetMass() == 0)
    {
      return;
    }

    // SETUP

    glm::vec2 translationA = transformA.GetTranslation();
    glm::vec2 translationB = transformB.GetTranslation();
    glm::vec2 sizeA = squareCollider.GetSize();

    // Calculation normal
    glm::vec2 normal = translationB - translationA;

    // Clamp point to edges of AABB
    glm::vec2 Closest = normal;

    // normal for resolution
    glm::vec2 normalTwo;

    float r = circleCollider.GetRadius();

    // Calculate half extends along each axis.
    float x_extent = sizeA.x / 2;
    float y_extent = sizeA.y / 2;

    Closest.x = glm::clamp(Closest.x, (-x_extent), x_extent);
    Closest.y = glm::clamp(Closest.y, (-y_extent), y_extent);

    bool inside = false;

    if (normal == Closest)
    {

      inside = true;
      if (abs(normal.x) > abs(normal.y))
      {
        // Sets Closest to x_extent or y_extent.
        if (Closest.x > 0)
        {

          Closest.x = x_extent;

        }
        else
        {

          Closest.x = -x_extent;

        }
      }
      else
      {
        if (Closest.y > 0)
        {

          Closest.y = y_extent;

        }
        else
        {

          Closest.y = -y_extent;

        }
      }

    }
    // Find second normal
    normalTwo = normal - Closest;
    float Length = glm::length(normalTwo);

    // Early out of the radius is shorter than distance to closest point and
    // Circle not inside the AABB
    if (Length > r && !inside)
      return;

    normalTwo = normalTwo / glm::length(normalTwo);

    // Collision normal needs to be flipped to point outside if circle was
    // inside the AABB
    if (inside)
    {
      // There really shouldn't be a case where something spawns inside each other, will fix if needed but for now its just here lol
      circleCollider.ColliderSendMessage(&squareCollider);
      squareCollider.ColliderSendMessage(&circleCollider);

      if (!(squareCollider.IsGhostCollider() || circleCollider.IsGhostCollider()))
      {

        ImpulseResolution(&squareCollider, &circleCollider, engine, -normalTwo, Physics1, Physics2);
        PositionalCorrection(&squareCollider, &circleCollider, engine, r - Length, -normalTwo, Physics1, Physics2, transformA, transformB);

      }

    }
    else
    {
      circleCollider.ColliderSendMessage(&squareCollider);
      squareCollider.ColliderSendMessage(&circleCollider);

      if (!(squareCollider.IsGhostCollider() || circleCollider.IsGhostCollider()))
      {

        ImpulseResolution(&squareCollider, &circleCollider, engine, normalTwo, Physics1, Physics2);
        PositionalCorrection(&squareCollider, &circleCollider, engine, r - Length, normalTwo, Physics1, Physics2, transformA, transformB);

      }

    }

  }

  /*--------------------------------------
  Description: Impulse Resolution, immediate resolution/reaction
    Params:
    IComponent* a - First component/collider
    IComponent* b - Second component/collider
    Engine& engine - Engine
    glm::vec2 norm - normal vector of the collision
    Physics& Physics1 - Physics Component of first collider
    Physics& Physics2 - Physics Component of second collider
  Return: Void
  ---------------------------------------*/
  void ImpulseResolution(IComponent* a, IComponent* b, Engine& engine, glm::vec2 norm, Physics& Physics1,
    Physics& Physics2)
  {

    // Calculate Relative Velocity
    glm::vec2 rv = Physics2.GetVelocity() - Physics1.GetVelocity();

    // Calculate relative velocity in terms of the normal direction
    float velAlongNormal = glm::dot(rv, norm);

    // Do not resolve if velocities are separating
    if (velAlongNormal > 0)
      return;

    // Calculate Restitution
    float e = std::min(Physics1.GetRestitution(), Physics2.GetRestitution());

    // Calculate impulse scalar
    float j = -(1 + e) * velAlongNormal;
    j /= Physics1.GetInverseMass() + Physics2.GetInverseMass();

    // Apply impulse
    glm::vec2 impulse = j * norm;
    glm::vec2 NewVelocity = Physics1.GetVelocity();
    NewVelocity -= Physics1.GetInverseMass() * impulse;
    Physics1.SetVelocity(NewVelocity);
    NewVelocity = Physics2.GetVelocity();
    NewVelocity += Physics2.GetInverseMass() * impulse;
    Physics2.SetVelocity(NewVelocity);

  }

  /*--------------------------------------
  Description: PositionalCorrection when two objects are up and each other's faces.
  Params:
    IComponent* a - First component/collider
    IComponent* b - Second component/collider
    Engine& engine - Engine
    float penetration - pen of the collision
    glm::vec2 norm - norm of collision
    Physics& Physics1 - Physics Component of first collider
    Physics& Physics2 - Physics Component of second collider
    Transform& transformA - Transform Component of first collider
    Transform& transformB - Transform Component of second collider.
  Return: Void
  ---------------------------------------*/
  void PositionalCorrection(IComponent* a, IComponent* b, Engine& engine, float penetration, glm::vec2 norm, Physics& Physics1,
    Physics& Physics2, Transform& transformA, Transform& transformB)
  {

    glm::vec3 translationA = transformA.GetTranslation();
    glm::vec3 translationB = transformB.GetTranslation();

    const float percent = 0.8f; // Usually 0.2 to 0.8
    const float slop = 0.01f; // Usually 0.01 to 0.1
    glm::vec2 correction = std::max(penetration - slop, 0.0f) / (Physics1.GetInverseMass() + Physics2.GetInverseMass()) * percent * norm;

    translationA -= Physics1.GetInverseMass() * glm::vec3(correction, 0);
    translationB += Physics2.GetInverseMass() * glm::vec3(correction, 0);

    transformA.setTranslation(translationA);
    transformB.setTranslation(translationB);

  }

  CollisionData SimpleAABB(glm::vec2 pos1, glm::vec2 size1,
    glm::vec2 pos2, glm::vec2 size2)
  {
    CollisionData dataBoi;

    // Calculation normal
    glm::vec2 normal = pos2 - pos1;

    // Calculate half extents along x axis for each object
    float a_extent = size1.x / 2;
    float b_extent = size2.x / 2;

    // Calculate overlap on x axis
    dataBoi.x_overlap = a_extent + b_extent - abs(normal.x);


    // SAT test on x axis
    if (dataBoi.x_overlap > 0)
    {
      // Calculate half extents along y axis for each object
      a_extent = size1.y / 2;
      b_extent = size2.y / 2;

      // Calculate overlap on y axis
      dataBoi.y_overlap = a_extent + b_extent - abs(normal.y);

      // SAT test on y axis
      if (dataBoi.y_overlap > 0)
      {

        // Find out which axis is axis of least pen
        if (dataBoi.x_overlap < dataBoi.y_overlap)
        {

          // Point towards B knowing that n points from A to B
          if (normal.x < 0)
            dataBoi.resolutionNorm = glm::vec2(-1, 0);
          else
            dataBoi.resolutionNorm = glm::vec2(1, 0);
        }
        else
        {
          // Other way around!
          if (normal.y < 0)
            dataBoi.resolutionNorm = glm::vec2(0, -1);
          else
            dataBoi.resolutionNorm = glm::vec2(0, 1);

        }

      }

    }
    if (dataBoi.x_overlap > 0 && dataBoi.y_overlap > 0)
    {
      dataBoi.has_collided = true;
    }
    return dataBoi;
  }

} // End Of Dapper Namespace