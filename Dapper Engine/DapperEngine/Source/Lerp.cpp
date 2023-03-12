//------------------------------------------------------------------------------
//
// File Name:	Lerp.cpp
// Author(s):	Jacob Varboncoeur
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "Lerp.h"


namespace Dapper
{
	vec2 Lerp
	(vec2 source, vec2 target, float intensity)
	{		
		vec2 lerped = source +
			((target - source) * intensity);
		return lerped;
	}

	vec2 LerpExponential
	(vec2 source, vec2 target, float intensity, int exponent)
	{		
		return source + 
			((target - source) * 
				static_cast<float>(pow(intensity, exponent)));
	}
	
	vec2 LerpLog
	(vec2 source, vec2 target, float intensity, int logarithm)
	{
		//log(intensity) / log(logarithm);
		logarithm;
		return source +
			((target - source) *
				static_cast<float>(log(intensity)));
		
	}
	
}