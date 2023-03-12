//------------------------------------------------------------------------------
//
// File Name:	UI.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Transform.h"
#include "Camera.h"
#include "imgui.h"
#include <math.h>
#include "UI.h"
#include "Utility.h"

namespace Dapper
{
  UI::UI(Engine* theEngine) : engine(theEngine), parentID(-1),
      paused(false)
  {
  }

  UI::~UI()
  {
  }

  void UI::Init()
  {
  }

  void UI::Update(float dt)
  {
    Transform* theTransform = GetComponentFromParent<Transform>
      (*engine, parentID, "Transform");
    glm::vec3 thatBoi = theTransform->GetTranslation();
    float thatZboi = thatBoi.z;

    //lerp it eventually
    Camera* theCamera = GetSystemFromEngine<Camera*>
      (*engine, "Camera");

    glm::vec3 camPos = theCamera->getCameraPosition();
    glm::vec3 newTranslation(camPos.x + offset.x, camPos.y + offset.y, thatZboi);
    theTransform->setTranslation(newTranslation);
  }

  void UI::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    auto obj = jsonVal.GetObject();
    auto theoffset = obj["Offset"].GetArray();
    offset = glm::vec3(theoffset[0].GetFloat(), theoffset[1].GetFloat(), theoffset[2].GetFloat());
  }

  void UI::Write(IJsonWriter& writer) const
  {
    writer.WriteKey("UI");
    writer.StartObject();
    writer.WriteVec3("Offset", offset.x, offset.y, offset.z);
    writer.EndObject();
  }

  void UI::ImGuiDisplay(float scale)
  {
    
    ImVec2 cursor = ImGui::GetCursorPos();
    ImVec2 size(148 * scale, 35 * scale);
    if (ImGui::Button("+##X", size))
    {
      offset.x += 50;
    }
    ImGui::SameLine(size.x + cursor.x + (4 * scale));
    if (ImGui::Button("+##Y", size))
    {
      offset.y += 50;
    }
    ImGui::PushItemWidth(300 * scale);
    ImGui::InputFloat2("Position", &offset[0]);
    ImGui::PopItemWidth();
    if (ImGui::Button("-##X", size))
    {
      offset.x -= 50;
    }
    cursor = ImGui::GetCursorPos();
    ImGui::SameLine(size.x + cursor.x + (4 * scale));
    if (ImGui::Button("-##Y", size))
    {
      offset.y -= 50;
    }
  }
}