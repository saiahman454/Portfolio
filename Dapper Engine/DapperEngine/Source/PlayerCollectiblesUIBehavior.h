//------------------------------------------------------------------------------
//
// File Name:	PlayerCollectiblesUIBehavior.h
// Author(s):	Natalie Bouley
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "IComponent.h"
#include "Engine.h"
#include "IDManager.h"
#include "SpriteText.h"
#include "Messenger.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
	class PlayerCollectiblesUIBehavior : public IComponent
	{
	public:
		PlayerCollectiblesUIBehavior(int ID, Engine* theEngine);
		~PlayerCollectiblesUIBehavior();
		// Inherited via IComponent
		virtual void Init() override;
		virtual void Update(float dt) override;
		inline virtual int GetParentID() const override { return parent_ID; }
		inline virtual void SetParentID(int ID) override { parent_ID = ID; }
		inline virtual const std::string& GetName() const override { return name; }
		virtual void Read(Deserializer& deserial,
			const rapidjson::Value& jsonVal) override;
		inline virtual void SetID(int ID) override { my_ID = ID; }
		virtual void Write(IJsonWriter& writer) const override;
		virtual void ImGuiDisplay(float scale) override;
		inline const int GetID() const override { return my_ID; }

		inline void SetPause(bool Condition)override { paused = Condition; }
		inline bool GetPause() const override { return paused; }

		inline void SetNumCollectibles(int num_collectibles_) { num_collectibles = num_collectibles_; }
		inline int GetNumCollectibles() { return num_collectibles; }
		void OnCollection();

	private:
		inline static const std::string name = "PlayerCollectiblesUIBehavior";
		Engine* engine;
		int parent_ID; // The ID of the parent Game Object
		int my_ID; // The ID number of the UI behavior component
		bool paused = false;

		int parent_spritetext_ID;
		int num_collectibles; // The number of collectibles that the player has
		std::string level_name;
	};

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


