//------------------------------------------------------------------------------
//
// File Name:	SquareCollider.h
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
#include <vector>

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
	using vec2 = glm::vec2;

	class SquareCollider : public ICollider
	{
	public:

		using ResolutionFunc = std::function<void(IComponent*)>;

		SquareCollider(Engine* eng);

		void Init() override;
		void Update(float dt) override;
		inline const std::string& GetName() const override;
		void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;
		//dont implement until level editor is up
		void Write(IJsonWriter& writer) const override;
		void ImGuiDisplay(float scale)override;
		const int GetID() const override;
		inline void SetID(int ID) override;
		inline void SetParentID(int ID) override;
		inline int GetParentID() const override;
		inline bool IsGhostCollider() const;
		void SetGhostCollider(bool is_ghost);
		inline glm::vec2 GetSize() const;
		void SetSize(glm::vec2 new_size);

		inline void SetPause(bool Condition)override { paused = Condition; }
		inline bool GetPause() const override { return paused; }

	private:
		static inline const std::string name_ = "SquareCollider";
		int my_id;
		int parent_id = -1;
		glm::vec2 size = glm::vec2(0,0);
		bool ghost_collider = false;
		bool paused = false;



	}; //End of SquareCollider class


	bool IsOverlapping(const glm::vec2* (a_points), const glm::vec2* b_points, glm::vec2 axis);


	class CircleCollider;

	void SimpleAABBvsAABB(SquareCollider& squarecolliderA, SquareCollider& squarecolliderB, Engine& engine, Physics& Physics1,
		Physics& Physics2, Transform& transformA, Transform& transformB);
	void AABBvsCircle(SquareCollider& squareCollider, CircleCollider& circleCollider, Engine& engine, Physics& Physics1,
		Physics& Physics2, Transform& transformA, Transform& transformB);
	void ImpulseResolution(IComponent* a, IComponent* b, Engine& engine, glm::vec2 norm, Physics& Physics1,
		Physics& Physics2);
	void PositionalCorrection(IComponent* a, IComponent* b, Engine& engine, float penetration, glm::vec2 norm, Physics& Physics1,
		Physics& Physics2, Transform& transformA, Transform& transformB);
	
	struct CollisionData {
		glm::vec2 resolutionNorm;
		float x_overlap = 0;
		float y_overlap = 0;
		bool has_collided = false;
	};
	CollisionData SimpleAABB(glm::vec2 pos1, glm::vec2 size1,
					glm::vec2 pos2, glm::vec2 size2);
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


