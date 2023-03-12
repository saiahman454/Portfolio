//------------------------------------------------------------------------------
//
// File Name:	Health.h
// Author(s):	Adam Tackett
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
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
	class Health : public IComponent
	{
	public:
		Health(Engine* theEngine);
		~Health();
		// Inherited via IComponent
		virtual void Init() override;
		virtual void Update(float dt) override;
		inline virtual int GetParentID() const override { return parentID; }
		inline virtual void SetParentID(int ID) override { parentID = ID; }
		inline virtual const std::string& GetName() const override { return name; }
		virtual void Read(Deserializer& deserial,
			const rapidjson::Value& jsonVal) override;
		inline virtual void SetID(int ID) override { my_ID = ID; }
		virtual void Write(IJsonWriter& writer) const override;
		virtual void ImGuiDisplay(float scale) override;
		inline const int GetID() const override { return my_ID; }

		inline void SetPause(bool Condition)override { paused = Condition; }
		inline bool GetPause() const override { return paused; }

		inline void SetHealth(int theHealth) { healthCounter = theHealth; }
		inline int GetHealth() { return healthCounter; }
		void IncreaseHealth();
		void DecreaseHealth();

	private:
		inline static const std::string name = "Health";
		Engine* engine;
		int parentID; // The ID of the parent Game Object
		int my_ID; // The ID number of the health anchor
		IDManager ID_manager;
		int healthCounter;
		int maxHealth;
		std::vector<int> heartTracker;
		bool tempINIT;
		int counter = 0;
		bool paused = false;
	};//Health

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


