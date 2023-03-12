//------------------------------------------------------------------------------
//
// File Name:	ProgressionBarBehavior.h
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
#include "PlayerController.h"
#include "Transform.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
	class ProgressionBarBehavior : public IComponent
	{
	public:
		ProgressionBarBehavior(int ID, Engine* theEngine);
		~ProgressionBarBehavior();
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

	private:
		inline static const std::string name = "ProgressionBarBehavior";
		Engine* engine;
		int parent_ID; // The ID of the parent Game Object
		int my_ID; // The ID number of the UI behavior component
		bool paused = false;

		float slider_width = 0.0f; // The bar's width
		float slider_min_pos = 0.0f; // The x position of the left side of the bar
		float slider_max_pos = 0.0f; //. The x position of the right side of the bar

		float player_curr_pos = 0.0f;
		float player_start_pos = 0.0f; // The beginning position of the player
		float player_end_pos = 0.0f; // The "end" of the level
	};

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


