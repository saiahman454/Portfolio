//------------------------------------------------------------------------------
//
// File Name:	Lerp.h
// Author(s):	Jacob Varboncoeur
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include <cmath>
#include "glm/vec2.hpp"


namespace Dapper
{
	using vec2 = glm::vec2;
	// a = source  b = target
	// intensity = distance between a & b
	// lerp(a, b, 0.3) -> closer to a
	// lerp(a, b, 0.8) -> closer to b
	// lerp(a, b, 0.5) -> halfway between a & b
	vec2 Lerp(vec2 source, vec2 target, float intensity);
	vec2 LerpExponential
	(vec2 source, vec2 target, float intensity, int exponent);
	vec2 LerpLog
	(vec2 source, vec2 target, float intensity, int logarithm);
}

