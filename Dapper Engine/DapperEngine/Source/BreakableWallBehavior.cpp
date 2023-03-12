//------------------------------------------------------------------------------
//
// File Name:	BreakableWallBehavior.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "BreakableWallBehavior.h"
#include "ColliderManager.h"
#include "GameStateManager.h"
#include "PlayerController.h"
#include "Utility.h"
#include "imgui.h"
#include "ParticleEmitter.h"
#include "Input.h"
#include "Sprite.h"
#include "SquareCollider.h"

// Dapper Namespace!
namespace Dapper {
	BreakableWallBehavior::BreakableWallBehavior(int ID_, Engine* engine_)
		: my_ID(ID_)
		, engine(engine_)
	{
	}
	BreakableWallBehavior::~BreakableWallBehavior()
	{
	}

	void BreakableWallBehavior::Init()
	{
		// Register the collider resolution function
		SquareCollider& collider = *GetComponentFromParent<SquareCollider>(*engine, parent_ID, "SquareCollider");
		std::function<void(IComponent*)> func = std::bind(&BreakableWallBehavior::ResolutionFunction, this, std::placeholders::_1);
		collider.RegisterOnChange(func);

		// Set audio
		const Audio& audio = *GetSystemFromEngine<Audio*>(*engine, "Audio");

		sound = audio.GetSound(fileName);
	}
	void BreakableWallBehavior::Update(float dt)
	{
		if (killTimer > 0)
		{
			killTimer -= dt;
			if (killTimer <= 0)
			{
				GameObjectManager& gameobject_manager =
					*GetSystemFromEngine<GameObjectManager*>(*engine, "GameObjectManager");
				gameobject_manager.DeleteGameObject(parent_ID);
			}
		}
	}

	void BreakableWallBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
		deserial;
		auto obj = jsonVal.GetObject();

		auto fname = obj["SoundFileName"].GetString();
		this->fileName = fname;

		auto vol = obj["SoundVolume"].GetFloat();
		this->volume = vol;
	}

	void BreakableWallBehavior::Write(IJsonWriter& writer) const
	{
		writer.WriteKey("BreakableWallBehavior");
		writer.StartObject();
		writer.WriteString("SoundFileName", fileName.c_str());
		writer.WriteFloat("SoundVolume", volume);
		writer.EndObject();
	}

	void BreakableWallBehavior::ImGuiDisplay(float scale)
	{
		// Display editable values as float inputs.
		scale;

		if (ImGui::InputFloat("Volume", &volume, 0.05f))
		{
			sound->SetVolume(volume);
		}
	}

	void BreakableWallBehavior::ResolutionFunction(IComponent* object)
	{
		int theParentId = object->GetParentID();
		GameStateManager& state =
			*GetSystemFromEngine<GameStateManager*>(*engine, "GameStateManager");
		GameObjectManager& gameobject_manager =
			*GetSystemFromEngine<GameObjectManager*>(*engine, "GameObjectManager");
		GameObject& gameobject = gameobject_manager[theParentId];

		// Check if it is colliding with the player
		if (gameobject.GetFlag() == "Player" && killTimer == 0)
		{
			// Check if the player is currently dashing (GetState() from PlayerController)
			PlayerController& controller =
				*GetComponentFromParent<PlayerController>(*engine, theParentId, "PlayerController");
  			if (controller.GetState() == PlayerStates::PlayerDash)
			{
				// Emit particles
				ParticleEmitter& particleEmitter = *GetComponentFromParent<ParticleEmitter>
					(*engine, parent_ID, "ParticleEmitter");
				ParticleInfo info(0, 360, 50, 100, 90, true, 1.0f, 75, false, { 0,0 }, { 64,128 });
				particleEmitter.SetBurstInfo(info);
				particleEmitter.ResetParticles();

				SquareCollider& theCollider = *GetComponentFromParent<SquareCollider>
					(*engine, parent_ID, "SquareCollider");
				theCollider.SetGhostCollider(true);

				Sprite& theSprite = *GetComponentFromParent<Sprite>(*engine, parent_ID, "Sprite");
				theSprite.SetTint(glm::vec4(0));
				
				// Play break sound

				sound->SetParameter("speed",controller.GetPlayerVel());

				sound->Play();

				killTimer = particleEmitter.GetLifeOfParticles();	
			}
		}
	}

} // End Of Dapper Namespace