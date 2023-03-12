//------------------------------------------------------------------------------
//
// File Name:	Particle.h
// Author(s):	Adam Tackett
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
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "Utility.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
	struct Particle {
		glm::vec3 Position;
		glm::vec2 Velocity;
		glm::vec2 size;
		glm::vec4 Color;
		float     Life;
		float sin_wave;
		int textuere_id;
		//-10 in position moves bubble behind when initalized
		Particle() : Position(0.0f, 0.0f, 100.0f), Velocity(0.0f), Color(1.0f), Life(10.0f), sin_wave(RandomRange(1, 100)), size({25,25}) { }
	};

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


