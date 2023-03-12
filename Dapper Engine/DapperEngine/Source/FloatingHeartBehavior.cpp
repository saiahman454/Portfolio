//------------------------------------------------------------------------------
//
// File Name:	FloatingHeartBehavior.cpp
// Author(s):	Adam Tackett
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "FloatingHeartBehavior.h"
#include "ICollider.h"
#include "Transform.h"
#include "Physics.h"
#include "Utility.h"
#include "imgui.h"
#include "SquareCollider.h"
#include "glm/gtx/compatibility.hpp"
#include "glm/vec3.hpp"
#include "Audio.h"
#include "PlayerController.h"

// Dapper Namespace!
namespace Dapper {
  FloatingHeartBehavior::FloatingHeartBehavior(int ID_, Engine* engine_)
    : my_ID(ID_)
    , engine(engine_)
    , paused(false)
  {
  }

  FloatingHeartBehavior::~FloatingHeartBehavior()
  {
  }


  void FloatingHeartBehavior::Init()
  {
    //get collider on game object
    //subscribe the on coldie function
    ICollider* theCollider = GetComponentFromParent<ICollider>
      (*engine, GetParentID(), "SquareCollider");
    std::function<void(IComponent*)> func =
      std::bind(&FloatingHeartBehavior::OnCollide, this, std::placeholders::_1);
    theCollider->RegisterOnChange(func);
    //*GetComponentFromParent(Engine & engine, int parentID, 
    //const std::string_view & comp_name)

    const Audio& audio = *GetSystemFromEngine<Audio*>(*engine, "Audio");

    auto d = audio.GetSound(fileName);

    d->SetVolume(volume);


  }

  int FloatingHeartBehavior::GetParentID() const
  {
    return parent_ID;
  }

  void FloatingHeartBehavior::SetParentID(int ID)
  {
    parent_ID = ID;
  }

  const std::string& FloatingHeartBehavior::GetName() const
  {
    return name;
  }

  void FloatingHeartBehavior::Update(float dt)
  {
    timer -= dt;
    if (timer < 0)
    {
      GameObjectManager& gameObMan = *GetSystemFromEngine<GameObjectManager*>
        (*engine, "GameObjectManager");
      gameObMan.DeleteGameObject(GetParentID());
    }

  }

  void FloatingHeartBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial;
    // Read the data from the json: Speed (float), Pathpoint1 (vec2), Pathpoint2 (vec2)

    auto obj = jsonVal.GetObject();
    //speed = obj["Speed"].GetFloat();

    if (obj.HasMember("FileName"))
    {
      auto fname = obj["FileName"].GetString();
      fileName = fname;
    }

    if (obj.HasMember("Volume"))
    {
      auto vol = obj["Volume"].GetFloat();
      volume = vol;
    }

  }

  void FloatingHeartBehavior::Write(IJsonWriter& writer) const
  {
    // Write all of the values to the json file.
    writer.WriteKey("FloatingHeartBehavior");
    writer.StartObject();
    //writer.WriteFloat("Speed", speed);
    //writer.WriteVec2("Pathpoint1", pathpoint0.x, pathpoint0.y);
    //writer.WriteVec2("Pathpoint2", pathpoint1.x, pathpoint1.y);
    writer.WriteString("SoundFileName", fileName.c_str());
    writer.WriteFloat("SoundVolume", volume);
    writer.EndObject();
  }

  void FloatingHeartBehavior::ImGuiDisplay(float scale)
  {
    // Display editable values as float inputs.
    scale;
    //ImGui::InputFloat("Speed", &speed);
  }

  void FloatingHeartBehavior::SetID(int ID)
  {
    my_ID = ID;
  }

  const int FloatingHeartBehavior::GetID() const
  {
    return my_ID;
  }

  void FloatingHeartBehavior::SetDespawnTimer(float despawnTimer)
  {
    timer = despawnTimer;
  }

  void FloatingHeartBehavior::OnCollide(IComponent* comp)
  {

    GameObjectManager& gameObMan = *GetSystemFromEngine<GameObjectManager*>
      (*engine, "GameObjectManager");
    GameObject& theOb = gameObMan[comp->GetParentID()];

    if (theOb.GetFlag() == "Player")
    {

      const Audio& audio = *GetSystemFromEngine<Audio*>(*engine, "Audio");

      auto d = audio.GetSound(fileName);

      d->Play();

      gameObMan.DeleteGameObject(GetParentID());

    }
  }

} // End Of Dapper Namespace