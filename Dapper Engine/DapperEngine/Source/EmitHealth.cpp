//------------------------------------------------------------------------------
//
// File Name:	EmitHealth.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "EmitHealth.h"
#include "ComponentRegistry.h"
#include "Utility.h"
#include "UIManager.h"
#include "Transform.h"
#include "imgui.h"
#include "FloatingHeartBehavior.h"
#include "SceneManager.h"
// Dapper Namespace!
namespace Dapper 
{
	EmitHealth::EmitHealth(Engine* theEngine) :
		engine(theEngine),
		paused(false)
	{
	}

	EmitHealth::~EmitHealth()
	{
	}

	void EmitHealth::Init()
	{
		tempINIT = true;
		timer = initialTimer;
	}

	void EmitHealth::Update(float dt)
	{
		if (tempINIT == true)
		{
			//MakeHeartGoUp();
			tempINIT = false;
		}
		timer -= dt;
		if (timer < 0)
		{
			MakeHeartGoUp();
			timer = initialTimer;
		}
	}

	void EmitHealth::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
		deserial;
		auto object = jsonVal.GetObject();
		initialTimer = object["Timer"].GetFloat();
		despawnTimer = object["DespawnTimer"].GetFloat();
	}

	void EmitHealth::Write(IJsonWriter& writer) const
	{
		writer.WriteKey("EmitHealth");
		writer.StartObject();
		writer.WriteFloat("Timer", initialTimer);
		writer.WriteFloat("DespawnTimer", despawnTimer);
		writer.EndObject();
	}

	void EmitHealth::ImGuiDisplay(float scale)
	{
		ImGui::PushItemWidth(300 * scale);
		ImGui::InputFloat("SpawnTimer", &initialTimer, 1);
		ImGui::InputFloat("DespawnTimer", &despawnTimer, 1);
		ImGui::PopItemWidth();
	}

	void EmitHealth::MakeHeartGoUp()
	{
		Deserializer* deserial = dynamic_cast<Deserializer*>
			(engine->Get("Deserializer"));
		int emitHealthPush = deserial->DeserializePrefab
		("Data/Prefabs/FloatingHealth.json");
		GameObjectManager& gameObMan = *GetSystemFromEngine<GameObjectManager*>
			(*engine, "GameObjectManager");
		GameObject& theEmitHealth = gameObMan[emitHealthPush];
		theEmitHealth.SetName("EmitHealth" + std::to_string(counter));
		SceneManager& scene_manager = *GetSystemFromEngine<SceneManager*>(*engine, "SceneManager");
		std::string current_scene;
		if (scene_manager.IsSceneLoaded("Level1"))
		{
			current_scene = "Level1";
		}
		if (scene_manager.IsSceneLoaded("Level2"))
		{
			current_scene = "Level2";
		}
		if (scene_manager.IsSceneLoaded("Level3"))
		{
			current_scene = "Level3";
		}
		Scene& scene = scene_manager.GetScene(current_scene);
		scene.Add(emitHealthPush);

		//move thing to parent thing
		Transform* theTransform = GetComponentFromParent<Transform>
			(*engine, emitHealthPush, "Transform");
		Transform* theTransformOfParent = GetComponentFromParent<Transform>
			(*engine, GetParentID(), "Transform");
		theTransform->setTranslation(theTransformOfParent->GetTranslation());

		FloatingHeartBehavior* theBehave = GetComponentFromParent<FloatingHeartBehavior>
			(*engine, emitHealthPush, "FloatingHeartBehavior");
		theBehave->SetDespawnTimer(despawnTimer);

		//emitHealthTracker.push_back(emitHealthPush);
	}

} // End Of Dapper Namespace