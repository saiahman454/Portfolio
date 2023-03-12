//------------------------------------------------------------------------------
//
// File Name:	Transform.h
// Author(s):	Gavin Lu
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/vec3.hpp"
#include <iostream>
#include "IComponent.h"
#include <string>
#include "IJsonWriter.h"

namespace Dapper
{
	
	class Transform : public IComponent
	{
	public:		
		Transform();

		//Transform(int id, glm::vec3 translation, float rotationDegrees, glm::vec2 scale);

		~Transform() override;

		void Init() override;

		void Update(float dt) override;

		inline int GetParentID() const override;

		inline void SetParentID(int ID);

		const std::string& GetName() const override;

		inline void SetID(int ID);
		inline const int GetID() const override;

		void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;

		//dont implement until level editor is up
		void Write(IJsonWriter& writer) const override;

		void ImGuiDisplay(float scale)override;

		inline void SetPause(bool Condition)override { paused = Condition; }
		inline bool GetPause() const override { return paused; }

		glm::mat4 getMatrix();
		void Clean();

		void setTranslation(glm::vec3 trans);

		void setScale(glm::vec2 sc);

		void setRotation(float rot);

		void setFlipX(bool _flip_x);

		void setFlipY(bool _flip_y);

		glm::vec3 GetTranslation() const;
		float GetRotation();
		glm::vec2 GetScale();
		bool GetFlipX();
		bool GetFlipY();
	

		void AddToPosition(glm::vec2 lhs);
		void AddToScale(glm::vec2 lhs);
	private:
		static inline const std::string name = "Transform";
		glm::mat4 transformation{};
		glm::vec3 translation{0};
		glm::vec2 scale{50};
		float rotation = 0;
		bool is_dirty = true;
		int my_ID = -1;
		int parent_ID = -1;
		bool flip_x = false;
		bool flip_y = false;
		bool paused = false;
	};
	
}
