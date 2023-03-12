//------------------------------------------------------------------------------
//
// File Name:	Swordfish.cpp
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Swordfish.h"
#include "Utility.h"
#include <assert.h>
#include "ComponentRegistry.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "AnimationSpine.h"
#include "Audio.h"
#include "SquareCollider.h"
#include "PlayerController.h"

// Dapper Namespace!
namespace Dapper {
  /*--------------------------------------
  Description: Swordfish constructor
  Params:
  engine - the parent engine for the tile
  ID - id of the object
  ---------------------------------------*/
  Swordfish::Swordfish(int ID, Engine& engine) :
    IStateMachine(swordfish_state::Invalid, swordfish_state::Idle),
    my_engine(engine),
    my_ID(ID)
  {
  }
  /*--------------------------------------
  Description: Initalize function
  ---------------------------------------*/
  void Swordfish::Init()
  {

    SquareCollider& collider = *GetComponentFromParent<SquareCollider>(my_engine, parent_ID, "SquareCollider");
    std::function<void(IComponent*)> func = std::bind(&Swordfish::ResolutionFunction, this, std::placeholders::_1);
    collider.RegisterOnChange(func);

    AnimationSpine& animation = *GetComponentFromParent<AnimationSpine>(my_engine, parent_ID, "AnimationSpine");
    animation.GetInstance()->GetAnimationState()->setAnimation(0, "Swim", true);

    Audio& audio = *GetSystemFromEngine<Audio*>(my_engine, "Audio");
    audio.Load(("./Assets/SFX/Swordfish/" + fileName).c_str(), false);
    FMOD::Sound* sound = audio.GetSound("./Assets/SFX/Swordfish/" + fileName);
    audio.SetVolume("./Assets/SFX/Swordfish/" + fileName, volume);

  }

  /*--------------------------------------
  Description: The impact resolution
  Params:
  object - the other collider
  ---------------------------------------*/
  void Swordfish::ResolutionFunction(IComponent* object)
  {
    
    int theParentId = object->GetParentID();

    GameObjectManager& gameobject_manager =
      *GetSystemFromEngine<GameObjectManager*>(my_engine, "GameObjectManager");
    GameObject& gameobject = gameobject_manager[theParentId];

    if (gameobject.GetFlag() == "Player")
    {

      PlayerController& controller =
        *GetComponentFromParent<PlayerController>(my_engine, theParentId, "PlayerController");

      if (!controller.GetInvincible())
      {

        //Hurt Sound
        Audio& audio = *GetSystemFromEngine<Audio*>(my_engine, "Audio");
        auto sound = audio.GetSound("./Assets/SFX/Swordfish/" + fileName);
        audio.Play(sound, "./Assets/SFX/Swordfish/" + fileName);

      }
    }

  }
  /*--------------------------------------
  Description: Read function
  Params:
  deseial - unused
  jsonVal - the json object
  ---------------------------------------*/
  void Swordfish::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial;
    auto json_obj = jsonVal.GetObject();
    speed = json_obj["Speed"].GetFloat();
    engage_radius = json_obj["EngageRadius"].GetFloat();

    auto fname = json_obj["SoundFileName"].GetString();
    this->fileName = fname;

    auto vol = json_obj["SoundVolume"].GetFloat();
    this->volume = vol;
  }


  /*--------------------------------------
  Description: writes object to a file
  Params:
  writer - custom writer class 
  ---------------------------------------*/
  void Swordfish::Write(IJsonWriter& writer) const
  {
    writer.WriteKey("Swordfish");
    writer.StartObject();
    writer.WriteFloat("Speed", speed);
    writer.WriteFloat("EngageRadius", engage_radius);
    writer.WriteString("SoundFileName", fileName.c_str());
    writer.WriteFloat("SoundVolume", volume);
    writer.EndObject();
  }
  /*--------------------------------------
  Description: level editor display
  Params:
  scale - screen resolution
  ---------------------------------------*/
  void Swordfish::ImGuiDisplay(float scale)
  {
    ImGui::InputFloat("Speed", &speed, 50);
    ImGui::InputFloat("EngageRadius", &engage_radius, 50);
    ImGui::Text("Current file path");
    ImGui::Text(("./Assets/SFX/Swordfish/" + fileName).c_str());
    ImGui::InputText("FileName", &fileName);
    if (ImGui::Button("Reload Swordfish Sound"))
    {

      Audio& audio = *GetSystemFromEngine<Audio*>(my_engine, "Audio");
      audio.Load(("./Assets/SFX/Swordfish/" + fileName).c_str(), false);
      FMOD::Sound* sound = audio.GetSound("./Assets/SFX/Swordfish/" + fileName);
      audio.SetVolume("./Assets/SFX/Swordfish/" + fileName, volume);

    }

    if (ImGui::InputFloat("Volume", &volume, 0.05f)) {

      Audio& audio = *GetSystemFromEngine<Audio*>(my_engine, "Audio");
      audio.SetVolume("./Assets/SFX/Swordfish/" + fileName, volume);

    }

  }

  /*--------------------------------------
  Description: on exit function
  ---------------------------------------*/
  void Swordfish::OnExit()
  {

  }
  /*--------------------------------------
  Description: on init function
  ---------------------------------------*/
  void Swordfish::OnInit()
  {

  }
  /*--------------------------------------
  Description: update function for swordfish
  Params:
  dt - delta time
  ---------------------------------------*/
  void Swordfish::OnUpdate(float dt)
  {
    Transform& trans = *GetComponentFromParent<Transform>(my_engine, parent_ID, "Transform");
    Physics& physics = *GetComponentFromParent<Physics>(my_engine, parent_ID, "Physics");

    //float rotationRawAngle = atan2(new_move_direction.y, new_move_direction.x);
    //rotationRawAngle = glm::degrees(rotationRawAngle);

    AnimationSpine& animation = *GetComponentFromParent<AnimationSpine>(my_engine, parent_ID, "AnimationSpine");
    switch (GetState())
    {
    case swordfish_state::Idle:
    {
      // Set the swim animation
      spine::TrackEntry* current_animation = animation.GetInstance()->GetAnimationState()->getCurrent(0);
      if (!current_animation || current_animation->getAnimation()->getName() != "Swim")
      {
        animation.GetInstance()->GetAnimationState()->setAnimation(0, "Swim", true);
      }
      if (LookforTarget())
      {
        SetNextState(swordfish_state::Attack);
      }
      break;
    }
    case swordfish_state::Attack:
    {
      // Set the spot animation once, then back to swim aniamtion (Untested)
      //animation.GetInstance()->GetAnimationState()->setAnimation(0, "Spot", false);
      //animation.GetInstance()->GetAnimationState()->addAnimation(0, "Swim", true, 0);
      if (LookforTarget())
      {
        FollowTarget();
      }
      else
      {
        StopFollow();
        SetNextState(swordfish_state::Idle);
      }
    }
    }
  }


  /*--------------------------------------
  Description: Stops the follow behavoir
  ---------------------------------------*/
  void Swordfish::StopFollow()
  {
    Physics& my_pyhs = *GetComponentFromParent<Physics>(my_engine, parent_ID, "Physics");
    my_pyhs.SetVelocity({ 0,0 });
  }
  /*--------------------------------------
  Description: starts the follow behavoir
  ---------------------------------------*/
  void Swordfish::FollowTarget()
  {
    ComponentRegistry& reg = *GetSystemFromEngine< ComponentRegistry*>(my_engine, "ComponentRegistry");
    Transform& my_trans = *GetComponentFromParent<Transform>(my_engine, parent_ID, "Transform");
    Physics& my_pyhs = *GetComponentFromParent<Physics>(my_engine, parent_ID, "Physics");
    const GameObject* player = GetTarget();
    //there is no player
    assert(player != nullptr);
    if (player)
    {
      int ID = player->GetComponentID("Transform");
      Transform& target_trans = *dynamic_cast<Transform*>(reg.GetComponent("Transform", ID));
      glm::vec3 targetpos = target_trans.GetTranslation();
      glm::vec3 mypos = my_trans.GetTranslation();
      glm::vec2 target2D(targetpos.x, targetpos.y);
      glm::vec2 my2D(mypos.x, mypos.y);
      float distance = glm::distance(mypos, targetpos);
      if (distance < stop_radius)
      {
        my_pyhs.SetVelocity(glm::vec2(0.0, 0.0));
        return;
      }
      glm::vec2 direction = target2D - my2D;
      direction = glm::normalize(direction);
      direction *= 250.0f;
      my_pyhs.SetVelocity(direction);


      float rotationRawAngle = atan2(direction.y, direction.x);
      rotationRawAngle = glm::degrees(rotationRawAngle);

      if (std::abs(rotationRawAngle) > 90)
      {
        if (my_trans.GetScale().y > 0)
        {
          SpriteFlip(my_pyhs, my_trans);
        }
        my_trans.setRotation(-rotationRawAngle);
      }
      else
      {
        if (my_trans.GetScale().y < 0)
        {
          SpriteFlip(my_pyhs, my_trans);
        }
        my_trans.setRotation(rotationRawAngle);
      }
    }

  }
  /*--------------------------------------
  Description: Checks for player to start folowing 
  ---------------------------------------*/
  bool Swordfish::LookforTarget()
  {
    ComponentRegistry& reg = *GetSystemFromEngine< ComponentRegistry*>(my_engine, "ComponentRegistry");
    Transform& my_trans = *GetComponentFromParent<Transform>(my_engine, parent_ID, "Transform");

    const GameObject*  player = GetTarget();
    
    //there is no player
    assert(player != nullptr);
    if (player)
    {
      int ID = player->GetComponentID("Transform");
      Transform& target_trans = *dynamic_cast<Transform*>(reg.GetComponent("Transform", ID));
      glm::vec3 targetpos = target_trans.GetTranslation();
      glm::vec3 mypos = my_trans.GetTranslation();
      glm::vec2 target2D(targetpos.x, targetpos.y);
      glm::vec2 my2D(mypos.x, mypos.y);
      float distance = glm::distance(target2D, my2D);
      if (distance < engage_radius)
      {
        return true;
      }
    }
    return false;
  }

  /*--------------------------------------
  Description: gets the player target 
  ---------------------------------------*/
  const GameObject* Swordfish::GetTarget()
  {
    GameObjectManager& the_man = *GetSystemFromEngine<GameObjectManager*>(my_engine, "GameObjectManager");
    const std::vector<GameObject>& all_objs = the_man.GetAllGameObjects();
    for (const GameObject& current : all_objs)
    {
      if (current.GetFlag() == "Player")
      {
        return &current;
      }
    }
    return nullptr;
  }

  /*--------------------------------------
  Description: visually flips the sprite of the swordfish
  ---------------------------------------*/
  void Swordfish::SpriteFlip(Physics& physics, Transform& transform)
  {
    glm::vec2 new_scale = { transform.GetScale().x, -transform.GetScale().y };
    //transform.setFlipY(!transform.GetFlipY());
    transform.setScale(new_scale);
  }


} // End Of Dapper Namespace