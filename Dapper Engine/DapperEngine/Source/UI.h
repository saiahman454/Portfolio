//------------------------------------------------------------------------------
//
// File Name:	UI.h
// Author(s):	Adam Tackett (Luke Mostert)
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include <iostream>
#include "IComponent.h"
#include "IDManager.h"
#include "Engine.h"
#include "glm/vec3.hpp"


namespace Dapper
{
	class UI : public IComponent
	{
	public:		
		UI(Engine* theEngine);

		~UI();

		 void Init() override;
		 void Update(float dt) override;
		 inline int GetParentID() const override { return parentID; }
		 inline void SetParentID(int iD) override { parentID = iD; }
		 inline const std::string& GetName() const override { return name; }
		 void Read(Deserializer& deserial,
			const rapidjson::Value& jsonVal) override;
		 inline void SetID(int iD) override { ID = iD; }
		 void Write(IJsonWriter& writer) const override;
		 void ImGuiDisplay(float scale) override;
		 inline const int GetID() const override { return ID; }
		 inline void SetOffset(glm::vec3 newOffset) { offset = newOffset; }
		 inline const glm::vec3 GetOffset()const { return offset; };
		 void SetPause(bool Condition)override { paused = Condition; }
		 bool GetPause() const override { return paused; }

	
	private:
		inline static const std::string name = "UI";
		int parentID; //id of parent game object
		int ID;
		glm::vec3 offset;
		Engine* engine;
		bool paused = false;
	};
}