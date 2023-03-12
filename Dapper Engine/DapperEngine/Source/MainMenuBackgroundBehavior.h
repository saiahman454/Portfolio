//------------------------------------------------------------------------------
//
// File Name:	MainMenuBackgroundBehavior.h
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
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
	class MainMenuBackgroundBehavior : public IComponent
	{
	public:
		MainMenuBackgroundBehavior(int ID, Engine* theEngine);
		~MainMenuBackgroundBehavior();
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
		inline static const std::string name = "MainMenuBackgroundBehavior";
		Engine* engine;
		int parent_ID; // The ID of the parent Game Object
		int my_ID; // The ID number of the component

		bool paused = false;
	};
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


