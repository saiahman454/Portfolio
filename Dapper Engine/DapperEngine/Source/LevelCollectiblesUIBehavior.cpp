//------------------------------------------------------------------------------
//
// File Name:	LevelCollectiblesUIBehavior.cpp
// Author(s):	Natalie Bouley
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "LevelCollectiblesUIBehavior.h"
#include "ComponentRegistry.h"
#include "Utility.h"
#include "UIManager.h"
#include "imgui.h"
#include "Utility.h"
#include "PlayerController.h"
#include "GameObjectManager.h"
#include "BehaviorManager.h"
#include "SpriteText.h"
#include "SpriteTextManager.h"


// Dapper Namespace!
namespace Dapper 
{
	LevelCollectiblesUIBehavior::LevelCollectiblesUIBehavior(int ID, Engine* theEngine) :
		engine(theEngine),
		paused(false),
		my_ID(ID),
		parent_ID(-1),
		num_collectibles(0),
		parent_spritetext_ID(-1)
	{
	}

	LevelCollectiblesUIBehavior::~LevelCollectiblesUIBehavior()
	{
		BehaviorManager& behavior_manager = *GetSystemFromEngine<BehaviorManager*>(*engine, "BehaviorManager");
		behavior_manager.unRegisterCreateMessage(createMessageID);
		behavior_manager.unRegisterDestroyMessage(destroyMessageID);

	}

	void LevelCollectiblesUIBehavior::Init()
	{
		parent_spritetext_ID = GetComponentFromParent<SpriteText>(*engine, parent_ID, "SpriteText")->GetID();

		// Register message that will increase number of collectibles on collision with collectible
		BehaviorManager& behavior_manager = *GetSystemFromEngine<BehaviorManager*>(*engine, "BehaviorManager");
		createMessageID = behavior_manager.RegisterCreateMessage(std::bind(&LevelCollectiblesUIBehavior::OnCollectibleCreate, this));
		destroyMessageID = behavior_manager.RegisterDestroyMessage(std::bind(&LevelCollectiblesUIBehavior::OnCollectibleDestroy, this));

		// Register message that changes pointer on sprite text reallocation
		SpriteTextManager* spritetext_manager = GetSystemFromEngine<SpriteTextManager*>(*engine, "SpriteTextManager");
		SpriteText& spritetext = (*spritetext_manager)[parent_spritetext_ID];

		// Update the Sprite Text to have the number of collectibles
		std::string new_string = std::to_string(num_collectibles);
		spritetext.SetText(new_string);

		// Set num_collectibles to how many collectibles are in the behavior manager
		num_collectibles = behavior_manager.GetNumCollectibles();
	}

	void LevelCollectiblesUIBehavior::Update(float dt)
	{
		// Update the Sprite Text to have the number of collectibles
		SpriteTextManager* spritetext_manager = GetSystemFromEngine<SpriteTextManager*>(*engine, "SpriteTextManager");
		SpriteText& spritetext = (*spritetext_manager)[parent_spritetext_ID];
		std::string new_string = std::to_string(num_collectibles);
		spritetext.SetText(new_string);
	}

	void LevelCollectiblesUIBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
		deserial;
		//auto object = jsonVal.GetObject();
		//auto collectibles = object["NumCollectibles"].GetInt();
		//num_collectibles = collectibles;
	}

	void LevelCollectiblesUIBehavior::Write(IJsonWriter& writer) const
	{
		writer.WriteKey("LevelCollectiblesUIBehavior");
		writer.StartObject();
		//writer.WriteInt("NumCollectibles", num_collectibles);
		writer.EndObject();
	}

	void LevelCollectiblesUIBehavior::ImGuiDisplay(float scale)
	{
	}

	void LevelCollectiblesUIBehavior::OnCollectibleCreate()
	{
		num_collectibles++;
		// Update the Sprite Text to have the number of collectibles
		SpriteTextManager* spritetext_manager = GetSystemFromEngine<SpriteTextManager*>(*engine, "SpriteTextManager");
		SpriteText& spritetext = (*spritetext_manager)[parent_spritetext_ID];
		std::string new_string = std::to_string(num_collectibles);
		spritetext.SetText(new_string);
	}

	void LevelCollectiblesUIBehavior::OnCollectibleDestroy()
	{
	}

} // End Of Dapper Namespace