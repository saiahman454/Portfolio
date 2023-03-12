//------------------------------------------------------------------------------
//
// File Name:	CircleCollider.h
// Author(s):	Gavin Lu
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "ICollider.h"
#include "Physics.h"
#include "Transform.h"
#include "glm/vec2.hpp"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{

	class CircleCollider : public ICollider
	{
	public:

		using ResolutionFunc = std::function<void(IComponent*)>;

		CircleCollider(Engine* eng);

		void Init() override;
		void Update(float dt) override;
		const inline std::string& GetName() const override;
		void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;
		void Write(IJsonWriter& writer) const override;
		void ImGuiDisplay(float scale)override;
		const inline int GetID() const override;
		inline void SetID(int ID) override;
		inline void SetParentID(int ID) override;
		inline int GetParentID() const override;
		inline float GetRadius();
		inline void SetRadius(float new_radius);
		inline bool IsGhostCollider();
		void SetGhostCollider(bool is_ghost);

		inline void SetPause(bool Condition)override { paused = Condition; }
		inline bool GetPause() const override { return paused; }
	private:
		static inline const std::string name_ = "CircleCollider";
		int my_id;
		int parent_id = -1;
		float radius = 0;
		bool ghost_collider = false;
		bool paused = false;
	};

	void CircleVsCircleOptimized(CircleCollider& circlecolliderA, CircleCollider& circlecolliderB, Engine& engine, Physics& Physics1,
		Physics& Physics2, Transform& transformA, Transform& transformB);

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


