//------------------------------------------------------------------------------
//
// File Name:	CollectibleBehavior.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "CollectibleBehavior.h"
#include "ColliderManager.h"
#include "GameStateManager.h"
#include "PlayerController.h"
#include "Utility.h"
#include "imgui.h"
#include "Audio.h"
#include "ParticleEmitter.h"
#include "Sprite.h"
#include "BehaviorManager.h"

// Dapper Namespace!
namespace Dapper {
	CollectibleBehavior::CollectibleBehavior(int ID_, Engine* engine_)
		: my_ID(ID_)
		, engine(engine_)
	{
	}
	CollectibleBehavior::~CollectibleBehavior()
	{
		BehaviorManager& behavior_manager = *GetSystemFromEngine<BehaviorManager*>(*engine, "BehaviorManager");
		behavior_manager.DecrementNumCollectibles();
	}

	void CollectibleBehavior::Init()
	{
		// Register the collider resolution function
		SquareCollider& collider = *GetComponentFromParent<SquareCollider>(*engine, parent_ID, "SquareCollider");
		std::function<void(IComponent*)> func = std::bind(&CollectibleBehavior::ResolutionFunction, this, std::placeholders::_1);
		collider.RegisterOnChange(func);

		// Set audio
		const Audio& audio = *GetSystemFromEngine<Audio*>(*engine, "Audio");

		auto d = audio.GetSound(fileName);

		d->SetVolume(volume);

		// Increment number of collectibles in behavior manager
		BehaviorManager& behavior_manager = *GetSystemFromEngine<BehaviorManager*>(*engine, "BehaviorManager");
		behavior_manager.IncrementNumCollectibles();
	}

	void CollectibleBehavior::Update(float dt)
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

	void CollectibleBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
		deserial;
		auto obj = jsonVal.GetObject();

		auto fname = obj["SoundFileName"].GetString();
		this->fileName = fname;

		auto vol = obj["SoundVolume"].GetFloat();
		this->volume = vol;
	}

	void CollectibleBehavior::Write(IJsonWriter& writer) const
	{
		writer.WriteKey("CollectibleBehavior");
		writer.StartObject();
		writer.WriteString("SoundFileName", fileName.c_str());
		writer.WriteFloat("SoundVolume", volume);
		writer.EndObject();
	}

	void CollectibleBehavior::ImGuiDisplay(float scale)
	{
		// Display editable values as float inputs.
		scale;

		if (ImGui::InputFloat("Volume", &volume, 0.05f))
		{
			const Audio& audio = *GetSystemFromEngine<Audio*>(*engine, "Audio");
      auto sound = audio.GetSound(fileName);
      sound->SetVolume(volume);
		}
	}

	void CollectibleBehavior::ResolutionFunction(IComponent* object)
	{
		int theParentId = object->GetParentID();
		GameObjectManager& gameobject_manager =
			*GetSystemFromEngine<GameObjectManager*>(*engine, "GameObjectManager");
		GameObject& gameobject = gameobject_manager[theParentId];

		// Check if it is colliding with the player
		if (gameobject.GetFlag() == "Player" && killTimer == 0)
		{
			// Emit particles
			ParticleEmitter& particleEmitter = *GetComponentFromParent<ParticleEmitter>
				(*engine, parent_ID, "ParticleEmitter");
			particleEmitter.ResetParticles();

			// Play Collectible sound
			const Audio& audio = *GetSystemFromEngine<Audio*>(*engine, "Audio");
			auto sound = audio.GetSound(fileName);
			sound->Play();

			// Delete game object
			Sprite& theSprite = *GetComponentFromParent<Sprite>(*engine, parent_ID, "Sprite");
			theSprite.SetTint(glm::vec4(0));
			killTimer = particleEmitter.GetLifeOfParticles();
		}
	}

} // End Of Dapper Namespace