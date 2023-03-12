//------------------------------------------------------------------------------
//
// File Name:	Background.h
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "IComponent.h"
#include "Texture.h"
#include "Camera.h"

namespace Dapper
{

	class Background : public IComponent
	{
	public:
		Background(Engine* engine);

		~Background() override;

		void Init() override;
		void Update(float dt) override;
		inline int GetParentID() const override { return parent_ID; }
		inline void SetParentID(int ID) { parent_ID = ID; }
		inline const std::string& GetName() const override { return name; }
		inline void SetID(int ID) { my_ID = ID; }
		void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;
		void Write(IJsonWriter& writer) const override;
		void ImGuiDisplay(float scale)override;
		inline const int GetID() const override { return my_ID; }

		inline void SetPause(bool Condition)override { paused = Condition; }
		inline bool GetPause() const override { return paused; }

	private:
		void GetPlayer();


		static inline const std::string name = "Background";
		Engine* theEngine;
		int my_ID;
		int parent_ID = -1;
		glm::vec2 theU{0,0};
		glm::vec2 theV{0,0};
		glm::vec2 speed{0,0};
		bool paused = false;
		int player_id = -1;
		glm::vec3 last_pos;
	};//End Background class

}//End Dapper Namespace