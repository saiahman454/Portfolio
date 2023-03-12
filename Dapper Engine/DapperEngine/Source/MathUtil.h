//------------------------------------------------------------------------------
//
// File Name:	MathUtil.h
// Author(s):	Jacob Varboncoeur
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include <glm/vec4.hpp>
#include <vector>
#include <cmath>
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <iostream>
#include <string>
#include "ICollider.h"
namespace Dapper
{
	using mat4 = glm::mat4;
	using vec2 = glm::vec2;
	using vec3 = glm::vec3;
	// Because it was in h file, was being remade every time 
	// this was accessed 
	// "inline" -> code for this var is not found anywhere else.
	inline const float PI = glm::pi<float>();

	
	//&&&Jacob 
	// Getting linker error if in the .cpp
	template<typename vecType>
	float EasyDistance(vecType origin, vecType target)
	{
		float x1 = origin.x,
			x2 = target.x,
			y1 = origin.y,
			y2 = target.y;

		return sqrtf(powf((x2 - x1), 2.f) + powf((y2 - y1), 2.f));
	}

	

}