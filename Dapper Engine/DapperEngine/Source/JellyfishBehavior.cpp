//------------------------------------------------------------------------------
//
// File Name:	JellyfishBehavior.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "JellyfishBehavior.h"
#include "Utility.h"
#include "imgui.h"
#include "SquareCollider.h"
#include "imgui_stdlib.h"
#include "glm/gtx/compatibility.hpp"
#include "glm/vec3.hpp"
#include "AnimationSpine.h"
#include "Utility.h"
#include "PlayerController.h"
#include "Audio.h"
#include "Health.h"

// Dapper Namespace!
namespace Dapper {
  JellyfishBehavior::JellyfishBehavior(int ID_, Engine* engine_)
    : my_ID(ID_)
    , engine(engine_)
  {
  }

  JellyfishBehavior::~JellyfishBehavior()
  {
  }


  void JellyfishBehavior::Init()
  {
      // Register the collider resolution function
    SquareCollider& collider = *GetComponentFromParent<SquareCollider>(*engine, parent_ID, "SquareCollider");
    std::function<void(IComponent*)> func = std::bind(&JellyfishBehavior::ResolutionFunction, this, std::placeholders::_1);
    collider.RegisterOnChange(func);

    AnimationSpine& animation = *GetComponentFromParent<AnimationSpine>(*engine, parent_ID, "AnimationSpine");
    animation.GetInstance()->GetAnimationState()->setAnimation(0, "JellySwimZap", true);

    const Audio& audio = *GetSystemFromEngine<Audio*>(*engine, "Audio");

    auto d = audio.GetSound(fileName);

    d->SetVolume(volume);

  }

  void JellyfishBehavior::Update(float dt)
  {
    dt;
    // Get the transform and physics
    Physics& physics = *GetComponentFromParent<Physics>(*engine, parent_ID, "Physics");
    Transform& transform = *GetComponentFromParent<Transform>(*engine, parent_ID, "Transform");
    glm::vec2 translation = (glm::vec2)transform.GetTranslation();

    // Find the next pathpoint to go to.
    FindNextPathpoint(translation);

    // Get the transform and physics
    glm::vec2 direction;
    float distance;

    if (next_pathpoint == 0)
    {
      direction = pathpoint0 - translation;
      glm::vec2 velocity = glm::normalize(direction);
      physics.SetVelocity(velocity * speed);
    }
    if (next_pathpoint == 1)
    {
      direction = pathpoint1 - translation;
      glm::vec2 velocity = glm::normalize(direction);
      physics.SetVelocity(velocity * speed);
    }
  }

  void JellyfishBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial;
    // Read the data from the json: Speed (float), Pathpoint1 (vec2), Pathpoint2 (vec2)

    auto obj = jsonVal.GetObject();
    speed = obj["Speed"].GetFloat();

    auto read_pathpoint0 = obj["Pathpoint1"].GetArray();
    pathpoint0 = glm::vec2(read_pathpoint0[0].GetFloat(), read_pathpoint0[1].GetFloat());

    auto read_pathpoint1 = obj["Pathpoint2"].GetArray();
    pathpoint1 = glm::vec2(read_pathpoint1[0].GetFloat(), read_pathpoint1[1].GetFloat());

    auto fname = obj["SoundFileName"].GetString();
    this->fileName = fname;

    auto vol = obj["SoundVolume"].GetFloat();
    this->volume = vol;
  }

  void JellyfishBehavior::Write(IJsonWriter& writer) const
  {
    // Write all of the values to the json file.
    writer.WriteKey("JellyfishBehavior");
    writer.StartObject();
    writer.WriteFloat("Speed", speed);
    writer.WriteVec2("Pathpoint1", pathpoint0.x, pathpoint0.y);
    writer.WriteVec2("Pathpoint2", pathpoint1.x, pathpoint1.y);
    writer.WriteString("SoundFileName", fileName.c_str());
    writer.WriteFloat("SoundVolume", volume);
    writer.EndObject();
  }

  void JellyfishBehavior::ImGuiDisplay(float scale)
  {
    // Display editable values as float inputs.
    scale;
    ImGui::InputFloat("Speed", &speed);
    ImGui::InputFloat2("Pathpoint 1", &pathpoint0[0]);
    ImGui::InputFloat2("Pathpoint 2", &pathpoint1[0]);
    if (ImGui::Button("Set Pathpoint 1 to Current Position"))
    {
        Transform& transform = *GetComponentFromParent<Transform>(*engine, parent_ID, "Transform");
        glm::vec2 translation = (glm::vec2)transform.GetTranslation();

        pathpoint0 = translation;
    }

    if (ImGui::Button("Set Pathpoint 2 to Current Position"))
    {
        Transform& transform = *GetComponentFromParent<Transform>(*engine, parent_ID, "Transform");
        glm::vec2 translation = (glm::vec2)transform.GetTranslation();

        pathpoint1 = translation;
    }
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    if (ImGui::InputFloat("Volume", &volume, 0.05f)) 
    {
      const Audio& audio = *GetSystemFromEngine<Audio*>(*engine, "Audio");
      auto sound = audio.GetSound(fileName);
      sound->SetVolume(volume);
    }
  }

  void JellyfishBehavior::ResolutionFunction(IComponent* object)
  {
    int theParentId = object->GetParentID();

    GameObjectManager& gameobject_manager =
      *GetSystemFromEngine<GameObjectManager*>(*engine, "GameObjectManager");
    GameObject& gameobject = gameobject_manager[theParentId];

    if (gameobject.GetFlag() == "Player")
    {

      PlayerController& controller =
        *GetComponentFromParent<PlayerController>(*engine, theParentId, "PlayerController");

      Health& theHealth =
        *GetComponentFromParent<Health>(*engine, theParentId, "Health");

      if (!controller.GetInvincible() && !controller.GetInvincibleFlag())
      {
        //Hurt Sound
        const Audio& audio = *GetSystemFromEngine<Audio*>(*engine, "Audio");
        auto sound = audio.GetSound(fileName);

        auto sound2 = audio.GetSound("/SFX/Player/Player_Damage");

        sound2->SetParameter("health", theHealth.GetHealth() - 1);

        //sound->SetParameter("health",theHealth.GetHealth());
        sound2->Play();
        sound->Play();
      }
    }

  }

  void JellyfishBehavior::FindNextPathpoint(glm::vec2 translation)
  {
    // Set the next pathpoint.
    float distance = glm::distance(translation, pathpoint0);
    if (distance < detect_radius)
    {
      next_pathpoint = 1;
    }

    distance = glm::distance(translation, pathpoint1);
    if (distance < detect_radius)
    {
      next_pathpoint = 0;
    }
  }

} // End Of Dapper Namespace