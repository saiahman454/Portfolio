//------------------------------------------------------------------------------
//
// File Name:	Circle
// 
// 
// .cpp
// Author(s):	Gavin Lu
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "CircleCollider.h"
#include "Transform.h"
#include "Utility.h"
#include <glm/ext/scalar_constants.hpp>
#include "SquareCollider.h"
#include "Physics.h"
#include "imgui.h"
// Dapper Namespace!
namespace Dapper 
{

	/*--------------------------------------
	Description: CircleCollider Constructor
	Return: Void
	---------------------------------------*/
	CircleCollider::CircleCollider(Engine* eng)
		: ICollider(eng)
	{
	}

	/*--------------------------------------
	Description: Init function for circle collider
	Return: Void
	---------------------------------------*/
	void CircleCollider::Init()
	{
		SetPhysicsPointer(GetComponentFromParent<Physics>(*GetEngine(), GetParentID(), "Physics"));
		SetTransformPointer(GetComponentFromParent<Transform>(*GetEngine(), GetParentID(), "Transform"));
	}
	
	/*--------------------------------------
	Description: CircleCollider Update
	Params: float dt ¡V Time derivative!
	Return: Void
	---------------------------------------*/
	void CircleCollider::Update(float dt)
	{
		dt;
	}
	
	/*--------------------------------------
	Description: SquareCollider Deserialization/read
	Params:
		Engine& engine - engine for getting audio system.
		rapidjson::Value& val - value gotten from json
	Return: Void
	---------------------------------------*/
	void CircleCollider::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
		deserial;
		// The instantaneous acceleration which a player starts at, IE when they start moving.
		auto obj = jsonVal.GetObject();
		auto rad = obj["Radius"].GetFloat();
		this->radius = rad;
		auto ghost = obj["GhostCollider"].GetBool();
		this->ghost_collider = ghost;

	}
	
	/*--------------------------------------
	Description: CircleCollider writes to json for deserializing.
	Params:
		IJsonWriter& writer - Writer for writing
	Return: Void
	---------------------------------------*/
	void CircleCollider::Write(IJsonWriter& writer) const
	{
		writer.WriteKey("CircleCollider");
		writer.StartObject();
		writer.WriteFloat("Radius", radius);
		writer.WriteBool("GhostCollider", ghost_collider);
		writer.EndObject();
	}

	/*--------------------------------------
	Description: CircleCollider gui display for editor.
	Params:
		float scale - scale
	Return: Void
	---------------------------------------*/
	void CircleCollider::ImGuiDisplay(float scale)
	{
		scale;
		ImGui::Checkbox("Ghost Collider", &ghost_collider);
	}

	/*--------------------------------------
	Description: Gets component ID
	Return: const int
	---------------------------------------*/
	const int CircleCollider::GetID() const
	{
		return my_id;
	}
	
	/*--------------------------------------
	Description: Gets Component name, aka "Circle Collider"
	Return: const std::string&
	---------------------------------------*/
	const std::string& CircleCollider::GetName() const
	{
		// TODO: insert return statement here
		return name_;
	}

	/*--------------------------------------
	Description: Sets component ID
	Params:
		int ID - ID you're setting
	Return: Void
	---------------------------------------*/
	void CircleCollider::SetID(int ID)
	{
		my_id = ID;
	}

	/*--------------------------------------
	Description: Gets parent id of component
	Return: int
	---------------------------------------*/
	int CircleCollider::GetParentID() const
	{
		return parent_id;
	}

	/*--------------------------------------
	Description: Gets radius of this collider
	Return: float
	---------------------------------------*/
	float CircleCollider::GetRadius()
	{
		return radius;
	}

	/*--------------------------------------
	Description: Sets radius of this collider
	Params:
		float new_radius - radius you're setting
	Return: float
	---------------------------------------*/
	void CircleCollider::SetRadius(float new_radius)
	{

		radius = new_radius;

	}

	/*--------------------------------------
	Description: Returns if the collider is a ghost collider
	Return: bool
	---------------------------------------*/
	bool CircleCollider::IsGhostCollider()
	{
		return ghost_collider;
	}

	/*--------------------------------------
	Description: Sets if collider is ghost
	Params:
		bool is_ghost - if it's ghost
	Return: Void
	---------------------------------------*/
	void CircleCollider::SetGhostCollider(bool is_ghost)
	{
		ghost_collider = is_ghost;
	}

	/*--------------------------------------
	Description: Sets parent ID
	Params:
		int ID - ID you're setting
	Return: Void
	---------------------------------------*/
	void CircleCollider::SetParentID(int ID)
	{
		parent_id = ID;
	}

	/*--------------------------------------
	Description: Collider check circle vs circle, uses function from squarecolliders.
	Params:
		CircleCollider& circlecolliderA - First component/collider
		CircleCollider& circlecolliderB - Second component/collider
		Engine& engine - Engine
		Physics& Physics1 - Physics Component of first collider
		Physics& Physics2 - Physics Component of second collider
		Transform& transformA - Transform Component of first collider
		Transform& transformB - Transform Component of second collider.
	Return: Void
	---------------------------------------*/
	void CircleVsCircleOptimized(CircleCollider& circlecolliderA, CircleCollider& circlecolliderB, Engine& engine, Physics& Physics1,
		Physics& Physics2, Transform& transformA, Transform& transformB)
	{

		// Well if both object's mass is 0, it means they're both static, so we dont do the check.

		if (Physics1.GetMass() == 0 && Physics2.GetMass() == 0)
		{
			return;
		}

		// SETUP

		glm::vec2 translationA = transformA.GetTranslation();
		glm::vec2 translationB = transformB.GetTranslation();

		// Calculation normal
		glm::vec2 normal = translationB - translationA;
		glm::vec2 normParam;

		float normLength = glm::length(normal);
		float penetration;
		float r = circlecolliderA.GetRadius() + circlecolliderB.GetRadius();
		// might need a r*=2 here

		if ((normLength) > r)
			return;

		// Circles have collided, now computer necessary parameters

		// If distance between circle is not 0
		if (normLength != 0)
		{

			// Distance between radius and distance
			penetration = r - normLength;

			// Utilize our d since we performed sqrt on it already within Length( )
			// Points from A to B, and is a unit vector
			normParam = normal / normLength;

			circlecolliderA.ColliderSendMessage(&circlecolliderB);
			circlecolliderB.ColliderSendMessage(&circlecolliderA);
			if (!(circlecolliderA.IsGhostCollider() || circlecolliderB.IsGhostCollider())) 
			{

				ImpulseResolution(&circlecolliderA, &circlecolliderB, engine, normParam, Physics1, Physics2);
				PositionalCorrection(&circlecolliderA, &circlecolliderB, engine, penetration, normParam, Physics1, Physics2, transformA, transformB);

			}
			
			return;

		}
		else {
			// Circles are on same position
			// Choose random (but consistent) values

			penetration = circlecolliderA.GetRadius();
			normParam = glm::vec2(1, 0);

			circlecolliderA.ColliderSendMessage(&circlecolliderB);
			circlecolliderB.ColliderSendMessage(&circlecolliderA);
			if (!(circlecolliderA.IsGhostCollider() || circlecolliderB.IsGhostCollider()))
			{

				ImpulseResolution(&circlecolliderA, &circlecolliderB, engine, normParam, Physics1, Physics2);
				PositionalCorrection(&circlecolliderA, &circlecolliderB, engine, penetration, normParam, Physics1, Physics2, transformA, transformB);
				
			}

			return;

		}

	}

} // End Of Dapper Namespace