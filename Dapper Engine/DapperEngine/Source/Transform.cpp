//------------------------------------------------------------------------------
//
// File Name:	Transform.cpp
// Author(s):	Gavin Lu
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Transform.h"
#include "imgui.h"
#include <math.h>

namespace Dapper
{
  Transform::Transform() :
    translation({ 0,0,0 }),
    scale({ 50,50 }),
    rotation(0),
    flip_x(false),
    flip_y(false),
    paused(false)
  {
  }

  Transform::~Transform()
  {


  }

  // &&&Gavin Double Check Clean Method and put in comments. And change all to vec4

  // Removed dirty functions, as they are not needed.

  // Might not need an init
  void Transform::Init()
  {

  }

  // Does Clean need to be here? Or should there be a get Matrix?
  void Transform::Update(float dt)
  {
    
    dt;
    //Clean();
  }

  glm::mat4 Transform::getMatrix()
  {
    Clean();
    return transformation;
  }

  void Transform::Clean()
  {
    if (is_dirty)
    {

      // Concat the translate, rotate, and scale into the position vector. //Adam made openGL see 0 for z
      transformation = glm::translate(glm::mat4(1.f), glm::vec3(translation.x, translation.y, 0.0f)) *
        glm::rotate(glm::mat4(1.f), glm::radians(rotation), glm::vec3(0.0, 0.0, 1.0)) *
        glm::scale(glm::mat4(1.f), glm::vec3(scale, 1));
      is_dirty = false;

    }
  }

  // Returns parent GameObject
  // All Components have a parent id which is set on creation
  int Transform::GetParentID() const
  {

    return 1;

  }

  void Transform::SetParentID(int ID)
  {
    parent_ID = ID;
  }

  const std::string& Transform::GetName() const
  {
    return name;
  }

  void Transform::SetID(int ID)
  {
    my_ID = ID;
  }

  const int Transform::GetID() const
  {
    return my_ID;
  }

  void Transform::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial;

    auto obj = jsonVal.GetObject();
    auto trans = obj["Translation"].GetArray();
    translation = glm::vec3(trans[0].GetFloat(), trans[1].GetFloat(), trans[2].GetFloat());

    auto sc = obj["Scale"].GetArray();
    this->scale = glm::vec2(sc[0].GetFloat(), sc[1].GetFloat());

    auto rot = obj["Rotation"].GetFloat();
    rotation = rot;

    is_dirty = true;

  }

  void Transform::Write(IJsonWriter& writer) const
  {
    writer.WriteKey("Transform");
    writer.StartObject();
    writer.WriteVec3("Translation", translation.x, translation.y, translation.z);
    writer.WriteVec2("Scale", scale.x, scale.y);
    writer.WriteFloat("Rotation", rotation);
    writer.EndObject();
  }

  void Transform::ImGuiDisplay(float win_scale)
  {
    ImVec2 cursor = ImGui::GetCursorPos();
    ImVec2 button(175 * win_scale, 35 * win_scale);
    if (ImGui::Button("+##X", button))
    {
      translation.x += 50;
    }
    ImGui::SameLine(cursor.x + button.x + (5 * win_scale));
    if (ImGui::Button("+##Y", button))
    {
      translation.y += 50;
    }
    ImGui::InputFloat2("Position", &translation[0]);
    if (ImGui::Button("-##X", button))
    {
      translation.x -= 50;
    }
    cursor = ImGui::GetCursorPos();
    ImGui::SameLine(cursor.x + button.x + (5 * win_scale));
    if (ImGui::Button("-##Y", button))
    {
      translation.y -= 50;
    }
    ImGui::InputFloat("Z order", &translation.z, 0.01f);
    //ImGui::InputFloat("Position y", &translation.y, 50.0f);
    ImVec2 button3(115 * win_scale, 35 * win_scale);
    ImGui::NewLine();
    ImGui::InputFloat("Rotation", &rotation, 10.0f);
    ImGui::NewLine();
    if (ImGui::Button("+##Xsize", button3))
    {
      scale.x += 50;
    }
    cursor = ImGui::GetCursorPos();
    ImGui::SameLine(cursor.x + button3.x + (5 * win_scale));
    if (ImGui::Button("Both + ##+size", button3))
    {
      scale.x += 50;
      scale.y += 50;
    }
    cursor = ImGui::GetCursorPos();
    ImGui::SameLine(cursor.x + (button3.x * 2) + (10 * win_scale));
    if (ImGui::Button("+##Ysize", button3))
    {
      scale.y += 50;
    }
    ImGui::InputFloat2("Width", &scale[0]);
    if (ImGui::Button("-##Xsize", button3))
    {
      scale.x -= 50;
    }
    cursor = ImGui::GetCursorPos();
    ImGui::SameLine(cursor.x + button3.x + (5 * win_scale));
    if (ImGui::Button("Both -##-size", button3))
    {
      scale.x -= 50;
      scale.y -= 50;
    }
    cursor = ImGui::GetCursorPos();
    ImGui::SameLine(cursor.x + (button3.x * 2) + (10 * win_scale));
    if (ImGui::Button("-##Ysize", button3))
    {
      scale.y -= 50;
    }

    //ImGui::InputFloat("Hieght", &scale.y, 50.0f);
  }

  void Transform::setTranslation(glm::vec3 trans)
  {

    translation = trans;
    is_dirty = true;

  }

  void Transform::setScale(glm::vec2 sc)
  {

    scale = sc;
    is_dirty = true;

  }

  void Transform::setRotation(float rot)
  {
    rotation = fmod(rot, 360.0f);
    //rotation = rot;
    is_dirty = true;

  }

  void Transform::setFlipX(bool _flip_x)
  {
    flip_x = _flip_x;
  }

  void Transform::setFlipY(bool _flip_y)
  {
    flip_y = _flip_y;
  }

  glm::vec3 Transform::GetTranslation() const
  {
    return translation;
  }
  float Transform::GetRotation()
  {
    return rotation;
  }
  glm::vec2 Transform::GetScale()
  {
    return scale;
  }

  bool Transform::GetFlipX()
  {
    return flip_x;
  }

  bool Transform::GetFlipY()
  {
    return flip_y;
  }

  void Transform::AddToPosition(glm::vec2 lhs)
  {
    translation.x += lhs.x;
    translation.y += lhs.y;
  }

  void Transform::AddToScale(glm::vec2 lhs)
  {
    scale.x += lhs.x;
    scale.y += lhs.y;
  }

}