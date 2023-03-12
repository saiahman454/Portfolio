//------------------------------------------------------------------------------
//
// File Name:	SwordfishMissle.cpp
// Author(s):	
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "SwordfishMissle.h"
#include "GameObjectManager.h"
#include "Utility.h"
#include "Transform.h"
#include "Transform.h"
#include <random>
#include "Camera.h"
#include "Physics.h"
#include "imgui.h"
#include "PlayerController.h"
#include "SquareCollider.h"
#include "AnimationSpine.h"
#include "Health.h"

// Dapper Namespace!
namespace Dapper {



  SwordfishMissle::SwordfishMissle(int ID, Engine& engine) :
    IStateMachine(swordfishMissle_state::Invalid, swordfishMissle_state::Idle),
    my_engine(engine),
    parent_ID(ID)
  {
  }

  void SwordfishMissle::Init()
  {
    std::srand(time(NULL));

    SquareCollider& collider = *GetComponentFromParent<SquareCollider>(my_engine, parent_ID, "SquareCollider");
    std::function<void(IComponent*)> func = std::bind(&SwordfishMissle::ResolutionFunction, this, std::placeholders::_1);
    collider.RegisterOnChange(func);

    const Audio& audio = *GetSystemFromEngine<Audio*>(my_engine, "Audio");

    sound = audio.GetSound(fileName);

    sound->SetVolume(volume);

    warningSound = audio.GetSound("/SFX/Enemies/Swordfish/Swordfish_Warning");

    warningSound->SetVolume(0.2);

    dashSound = audio.GetSound("/SFX/Enemies/Swordfish/Swordfish_Dash");

  }

  int SwordfishMissle::GetParentID() const
  {
    return parent_ID;
  }

  void SwordfishMissle::SetParentID(int ID)
  {
    parent_ID = ID;
  }

  const std::string& SwordfishMissle::GetName() const
  {
    return name;
  }

  void SwordfishMissle::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial;
    auto obj = jsonVal.GetObject();
    trigger = obj["Trigger"].GetFloat();
    warning_lerp = obj["WarningLerp"].GetFloat();
    moving_timer = obj["MovingTime"].GetFloat();
    stay_timer = obj["StayTime"].GetFloat();
    speed = obj["Speed"].GetFloat();
    fileName = obj["SoundFileName"].GetString();
    volume = obj["SoundVolume"].GetFloat();

  }

  void SwordfishMissle::SetID(int ID)
  {
    my_ID = ID;
  }

  void SwordfishMissle::Write(IJsonWriter& writer) const
  {
    writer.WriteKey("SwordfishMissle");
    writer.StartObject();
    writer.WriteFloat("Trigger", trigger);
    writer.WriteFloat("WarningLerp", warning_lerp);
    writer.WriteFloat("MovingTime", moving_timer);
    writer.WriteFloat("StayTime", stay_timer);
    writer.WriteFloat("Speed", speed);
    writer.WriteString("SoundFileName", fileName.c_str());
    writer.WriteFloat("SoundVolume", volume);
    writer.EndObject();

  }

  void SwordfishMissle::ImGuiDisplay(float scale)
  {
    ImGui::InputFloat("Trigger", &trigger);
    if (ImGui::Button("Set Trigger to Current X Position"))
    {
      Transform& transform = *GetComponentFromParent<Transform>(my_engine, parent_ID, "Transform");
      glm::vec2 translation = (glm::vec2)transform.GetTranslation();

      trigger = translation.x;
    }
    ImGui::InputFloat("Warning Lerp", &warning_lerp);
    ImGui::InputFloat("Moving Time", &moving_timer);
    ImGui::InputFloat("Stay Time", &stay_timer);
    ImGui::InputFloat("Speed", &speed);

    if (ImGui::InputFloat("Volume", &volume, 0.05f))
    {

      sound->SetVolume(volume);

    }

  }

  const int SwordfishMissle::GetID() const
  {
    return my_ID;
  }

  void SwordfishMissle::ResolutionFunction(IComponent* object)
  {

    int theParentId = object->GetParentID();

    GameObjectManager& gameobject_manager =
      *GetSystemFromEngine<GameObjectManager*>(my_engine, "GameObjectManager");
    GameObject& gameobject = gameobject_manager[theParentId];

    if (gameobject.GetFlag() == "Player")
    {

      PlayerController& controller =
        *GetComponentFromParent<PlayerController>(my_engine, theParentId, "PlayerController");

      if (!controller.GetInvincible() && !controller.GetInvincibleFlag())
      {
        //Hurt Sound
        Health& theHealth =
          *GetComponentFromParent<Health>(my_engine, theParentId, "Health");
        const Audio& audio = *GetSystemFromEngine<Audio*>(my_engine, "Audio"); 
        auto sound2 = audio.GetSound("/SFX/Player/Player_Damage");

        sound2->SetParameter("health", theHealth.GetHealth() - 1);
        sound2->Play();
        sound->Play();
      }
    }

  }

  void SwordfishMissle::OnExit()
  {
  }

  void SwordfishMissle::OnInit()
  {
  }

  void SwordfishMissle::OnUpdate(float dt)
  {
    Transform& transform = *GetComponentFromParent<Transform>(my_engine, parent_ID, "Transform");

    switch (GetState())
    {
    case swordfishMissle_state::Idle:
    {
      if (!triggerd)
      {
        GameObjectManager& obj_man = *GetSystemFromEngine<GameObjectManager*>(my_engine, "GameObjectManager");
        if (Target_ID == -1)
        {
          Target_ID = GetTarget();
          //no player
          assert(Target_ID != -1);
        }
        GameObject& target = obj_man[Target_ID];
        Transform& target_trans = *GetComponentFromParent<Transform>(my_engine, target.GetID(), "Transform");
        glm::vec3 target_pos = target_trans.GetTranslation();
        if (target_pos.x > trigger)
        {
          warning_ID = obj_man.MakePrefabWithName("Bubble_Warning_UI");
          Transform& warning_trans = *GetComponentFromParent<Transform>(my_engine, warning_ID, "Transform");
          Transform& target_trans = *GetComponentFromParent<Transform>(my_engine, Target_ID, "Transform");
          glm::vec3 target_pos = target_trans.GetTranslation();
          glm::vec3 warning_pos = warning_trans.GetTranslation();
          warning_pos.x = target_pos.x + 740;
          float random_y = -400.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (400.0f - -400.0f)));
          warning_pos.y = target_pos.y + random_y;
          warning_trans.setTranslation({ warning_pos.x,warning_pos.y, warning_pos.z });

          // Warning Sound
          warningSound->Play();
          SetNextState(swordfishMissle_state::Warning);
        }

      }
      break;
    }
    case swordfishMissle_state::Warning:
    {
      Transform& warning_trans = *GetComponentFromParent<Transform>(my_engine, warning_ID, "Transform");
      Transform& target_trans = *GetComponentFromParent<Transform>(my_engine, Target_ID, "Transform");
      glm::vec3 target_pos = target_trans.GetTranslation();
      glm::vec3 warning_pos = warning_trans.GetTranslation();
      Camera& camera = *GetSystemFromEngine<Camera*>(my_engine, "Camera");
      warning_pos.x = target_pos.x + 550;
      if (moving_timer > 0)
      {
        float down_dist = target_pos.y - warning_pos.y;
        down_dist = down_dist / warning_lerp;
        warning_trans.setTranslation({ warning_pos.x,warning_pos.y + down_dist, warning_pos.z });
        moving_timer -= dt;
        if (moving_timer < 0)
        {
          stay_timer = 1;
        }
      }
      if (stay_timer > 0 && moving_timer < 0)
      {
        stay_timer -= dt;
        warning_trans.setTranslation({ warning_pos.x,warning_pos.y, warning_pos.z });
        if (stay_timer < 0)
        {
          Transform& transform = *GetComponentFromParent<Transform>(my_engine, parent_ID, "Transform");
          last_warning_pos = { warning_pos.x,warning_pos.y, warning_pos.z };
          GameObjectManager& obj_man = *GetSystemFromEngine< GameObjectManager*>(my_engine, "GameObjectManager");
          obj_man.DeleteGameObject(warning_ID);

          AnimationSpine& animation = *GetComponentFromParent<AnimationSpine>(my_engine, parent_ID, "AnimationSpine");
          animation.GetInstance()->GetAnimationState()->setAnimation(0, "Swim", true);

          SetNextState(swordfishMissle_state::Attack);
          // Dash Sound
          dashSound->Play();
          transform.setTranslation({ last_warning_pos.x + 200,last_warning_pos.y,last_warning_pos.z });
        }
      }
      break;
    }
    case swordfishMissle_state::Attack:
    {
      Physics& physics = *GetComponentFromParent<Physics>(my_engine, parent_ID, "Physics");
      transform.setRotation(180);
      physics.SetVelocity({ -speed,0 });
      if (self_destruct_timer < 0)
      {
        GameObjectManager& obj_man = *GetSystemFromEngine< GameObjectManager*>(my_engine, "GameObjectManager");
        obj_man.DeleteGameObject(parent_ID);
      }
      self_destruct_timer -= dt;
      break;
    }
    }
    if (std::abs(transform.GetRotation()) > 90)
    {
      glm::vec2 new_scale = { transform.GetScale().x, -transform.GetScale().y };
      transform.setScale(new_scale);
    }
  }

  bool SwordfishMissle::UpdateWhilePaused() const
  {
    return false;
  }


  /*--------------------------------------
  Description: gets the player target
  ---------------------------------------*/
  const int SwordfishMissle::GetTarget()
  {
    GameObjectManager& obj_man = *GetSystemFromEngine<GameObjectManager*>(my_engine, "GameObjectManager");
    const std::vector<GameObject>& all_objs = obj_man.GetAllGameObjects();
    for (const GameObject& current : all_objs)
    {
      if (current.GetFlag() == "Player")
      {
        return current.GetID();
      }
    }
    return -1;
  }

} // End Of Dapper Namespace