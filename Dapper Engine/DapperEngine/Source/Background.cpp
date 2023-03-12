//------------------------------------------------------------------------------
//
// File Name:	Background.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Background.h"
#include "SpriteSourceManager.h"
#include "Utility.h"
#include "imgui.h"
#include "GameObjectManager.h"
#include "Physics.h"
#include "Transform.h"

namespace Dapper
{
  Background::Background(Engine* engine) : theEngine(engine)
  {
  }

  Background::~Background()
  {
  }

  void Background::Init()
  {
    //Texture front = Texture("./Assets/BackgroundTexture/front.png");
    //front.LoadTextureA();
    //Texture middle = Texture("./Assets/BackgroundTexture/middle.png");
    //front.LoadTextureA();
    //Texture back = Texture("./Assets/BackgroundTexture/back.png");
    //front.LoadTextureA();
  }

  void Background::Update(float dt)
  {
    glm::vec3 theMove;
    if (player_id == -1)
    {
      GetPlayer();
    }
    if (player_id != -1)
    {
      Physics& phys = *GetComponentFromParent<Physics>(*theEngine, player_id, "Physics");
      Transform& trans = *GetComponentFromParent<Transform>(*theEngine, player_id, "Transform");
      glm::vec2 old_pos = phys.GetOldPos();
      glm::vec3 old_pos3 = { old_pos.x,old_pos.y, 0 };
      //theMove = trans.GetTranslation() - old_pos3;
      Camera& camera = *GetSystemFromEngine<Camera*>(*theEngine, "Camera");
      glm::vec3 pos = camera.getCameraPosition();
      theMove = pos - last_pos;
      last_pos = pos;

    SpriteSource* theSprite = GetComponentFromParent<SpriteSource>
      (*theEngine, parent_ID, "SpriteSource");

    theU.y += theMove.x * (speed.x / 10000);
    theV.y += theMove.y * (speed.y / 10000);

    theSprite->SetCustomUV(theU, theV);
    }

    dt;
  }

  void Background::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial;
    auto theObject = jsonVal.GetObject();
    theU.x = theObject["u"].GetFloat();
    theV.x = theObject["v"].GetFloat();
    auto theSpeed = theObject["speed"].GetArray();
    speed.x = theSpeed[0].GetFloat();
    speed.y = theSpeed[1].GetFloat();
  }

  void Background::Write(IJsonWriter& writer) const
  {
    writer.WriteKey("Background");
    writer.StartObject();
    writer.WriteFloat("u", theU.x);
    writer.WriteFloat("v", theV.x);
    writer.WriteVec2("speed", speed.x, speed.y);
    writer.EndObject();
  }

  void Background::ImGuiDisplay(float scale)
  {
    scale;
    ImVec2 cursor = ImGui::GetCursorPos();
    ImVec2 button(175 * scale, 35 * scale);
    if (ImGui::Button("+##X", button))
    {
      speed.x += 1;
    }
    ImGui::SameLine(cursor.x + button.x + (5 * scale));
    if (ImGui::Button("+##Y", button))
    {
      speed.y += 1;
    }
    ImGui::InputFloat2("Speed", &speed[0]);
    if (ImGui::Button("-##X", button))
    {
      speed.x -= 1;
    }
    cursor = ImGui::GetCursorPos();
    ImGui::SameLine(cursor.x + button.x + (5 * scale));
    if (ImGui::Button("-##Y", button))
    {
      speed.y -= 1;
    }
  }

  void Background::GetPlayer()
  {
    GameObjectManager& obj_man = *GetSystemFromEngine<GameObjectManager*>(*theEngine, "GameObjectManager");
    const std::vector<GameObject>& all_objs = obj_man.GetAllGameObjects();
    for (const GameObject& current : all_objs)
    {
      if (current.GetFlag() == "Player")
      {
        player_id = current.GetID();
        return;
      }
    }
    player_id = -1;
  }

}//End Dapper Namespace