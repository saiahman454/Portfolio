//------------------------------------------------------------------------------
//
// File Name:	PlayerCollectiblesUIBehavior.cpp
// Author(s):	Natalie Bouley
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "PlayerCollectiblesUIBehavior.h"
#include "ComponentRegistry.h"
#include "Utility.h"
#include "UIManager.h"
#include "imgui.h"
#include "Utility.h"
#include "PlayerController.h"
#include "GameObjectManager.h"
#include "SpriteText.h"
#include "SpriteTextManager.h"
#include "SceneManager.h"
#include "JsonWriter.h"
#include "BGMController.h"

// Dapper Namespace!
namespace Dapper 
{
	PlayerCollectiblesUIBehavior::PlayerCollectiblesUIBehavior(int ID, Engine* theEngine) :
		engine(theEngine),
		paused(false),
		my_ID(ID),
		parent_ID(-1),
		num_collectibles(0),
		parent_spritetext_ID(0)
	{

	}

	PlayerCollectiblesUIBehavior::~PlayerCollectiblesUIBehavior()
	{
		JsonWriter writer;
		writer.StartObject();
		writer.WriteInt("Number", num_collectibles);
		writer.EndObject();
		writer.WriteToFile(level_name.c_str());
	}

	void PlayerCollectiblesUIBehavior::Init()
	{
		parent_spritetext_ID = GetComponentFromParent<SpriteText>(*engine, parent_ID, "SpriteText")->GetID();

		// Register message that will increase number of collectibles on collision with collectible
		GameObjectManager& gameobject_manager = *GetSystemFromEngine<GameObjectManager*>(*engine, "GameObjectManager");
		int player_ID = gameobject_manager.FindObjectWithTag("Player");
		PlayerController& player_controller = *GetComponentFromParent<PlayerController>(*engine, player_ID, "PlayerController");
		player_controller.RegisterMessage(std::bind(&PlayerCollectiblesUIBehavior::OnCollection, this));

		// Update the Sprite Text to have the number of collectibles
		SpriteTextManager* spritetext_manager = GetSystemFromEngine<SpriteTextManager*>(*engine, "SpriteTextManager");
		SpriteText& spritetext = (*spritetext_manager)[parent_spritetext_ID];
		std::string new_string = std::to_string(num_collectibles);
		spritetext.SetText(new_string);


	}

	void PlayerCollectiblesUIBehavior::Update(float dt)
	{
		SceneManager& scenes = *GetSystemFromEngine<SceneManager*>(*engine, "SceneManager");
		if (scenes.IsSceneLoaded("Level1"))
		{
			level_name = "./Data/Level1Data.json";
		}
		if (scenes.IsSceneLoaded("Level2"))
		{
			level_name = "./Data/Level2Data.json";
		}
		if (scenes.IsSceneLoaded("Level3"))
		{
			level_name = "./Data/Level3Data.json";
		}
	}

	void PlayerCollectiblesUIBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
		deserial;
		//auto object = jsonVal.GetObject();
		//auto collectibles = object["NumCollectibles"].GetInt();
		//num_collectibles = collectibles;
	}

	void PlayerCollectiblesUIBehavior::Write(IJsonWriter& writer) const
	{
		writer.WriteKey("PlayerCollectiblesUIBehavior");
		writer.StartObject();
		//writer.WriteInt("NumCollectibles", num_collectibles);
		writer.EndObject();
	}

	void PlayerCollectiblesUIBehavior::ImGuiDisplay(float scale)
	{
	}

	void PlayerCollectiblesUIBehavior::OnCollection()
	{
		num_collectibles++;
		// Update the Sprite Text to have the number of collectibles
		SpriteTextManager* spritetext_manager = GetSystemFromEngine<SpriteTextManager*>(*engine, "SpriteTextManager");
		SpriteText& spritetext = (*spritetext_manager)[parent_spritetext_ID];
		std::string new_string = std::to_string(num_collectibles);
		spritetext.SetText(new_string);

		// Check if max to send message to bgmcontroller :)) very temp implementation

		SceneManager& scenes = *GetSystemFromEngine<SceneManager*>(*engine, "SceneManager");
		BGMController& bgmcontroller = *(GetSystemFromEngine<BGMController*>(*engine, "BGMController"));
		
		const Audio& audio = *GetSystemFromEngine<Audio*>(*engine, "Audio");
		auto sound = audio.GetSound("/SFX/Events/Event_AllCollectables");

		if (scenes.IsSceneLoaded("Level1") && num_collectibles == 69)
		{

			bgmcontroller.CollectedAll(1);
			sound->Play();

		}
		else if (scenes.IsSceneLoaded("Level2") && num_collectibles == 39)
		{

			bgmcontroller.CollectedAll(1);
			sound->Play();

		}
		else if (scenes.IsSceneLoaded("Level3") && num_collectibles == 45)
		{

			bgmcontroller.CollectedAll(1);
			sound->Play();

		}

	}

} // End Of Dapper Namespace