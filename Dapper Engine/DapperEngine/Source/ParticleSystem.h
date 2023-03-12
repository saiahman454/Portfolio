//------------------------------------------------------------------------------
//
// File Name:	ParticleSystem.h
// Author(s):	Adam Tackett				
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
#include "ISystem.h"
#include "Texture.h"
#include "Engine.h"
#include "Particle.h"

//------------------------------------------------------------------------------
// Dapper Namespace!
namespace Dapper 
{
    class ParticleSystem : public ISystem
    {
    public:
        ParticleSystem(unsigned int theAmount, Engine& engine1);
        ~ParticleSystem()override;

        void Initialize()override;
        void Update(float dt) override;
		//draw the particles
        void Render() override;

		inline const std::string& Name()const override { return name; }
		inline std::vector<Particle>& operator[](int index) { return particles[index]; }
		inline void SetTextureID(int theTextureID) { textureID = theTextureID; }

		// update all particles
		void UpdateParticle(float dt);
		int GiveMeParticles(int howManyBro);

	private:
        const std::string name = "ParticleSystem";
		Engine& engine;
		std::vector<std::vector<Particle>> particles;
		unsigned int amount;
		int textureID;
		int transformID;
		int meshID = -1;

		//Currently handeled by emitters
		void respawnParticle(Particle& particle, glm::vec3 translation, 
			glm::vec2 offset = glm::vec2(0.0f, 0.0f));

		// Inherited via ISystem
		virtual bool UpdateWhilePaused() const override;

		};//End of ParticleSystem class

} // End Of Dapper Namespace