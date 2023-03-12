//------------------------------------------------------------------------------
//
// File Name:	Collider.cpp
// Author(s):	Jacob Varboncoeur
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Collider.h"
#include <glm/ext/scalar_constants.hpp>
// Dapper Namespace!
namespace Dapper 
{
	const float PI = glm::pi<float>();	
	
	Collider::Collider() : offset_(vec2(0.f))
	{

	}
	Collider::~Collider()
	{
	
	}
	void Collider::SetOffset(vec2 offset)
	{
		offset_ = offset;
	}
	const vec2& Collider::GetOffset() const
	{
		return offset_;
	}

	void Collider::SetScale(vec2 scale)
	{
		scale_ = scale;
	}

	const vec2& Collider::GetScale() const
	{
		return scale_;
	}


	int  Collider::GetParentID()
	{
		return parentID_;
	}

	void Collider::SetID(int ID)
	{
		my_ID = ID;
	}

	void Collider::SetParentID(int ID)
	{
		parentID_ = ID;
	}
	vec2 decomposeX(vec2 vector)
	{
		vector;
		return vec2(0.f);
	}

	vec2 decomposeY(vec2 vector)
	{
		vector;
		return vec2(0.f);
	}


} // End Of Dapper Namespace