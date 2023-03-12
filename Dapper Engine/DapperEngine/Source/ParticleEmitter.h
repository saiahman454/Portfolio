//------------------------------------------------------------------------------
//
// File Name:	ParticleEmitter.h
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "Engine.h"
#include "IComponent.h"
#include "Particle.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper
{
	struct Particle;
	struct ParticleInfo
	{
		float minAngle;
		float maxAngle;
		float minSpeed;
		float maxSpeed;
		float direction;
		bool burst;
		float timer;
		float gravity_value;
		bool wavey;
		glm::vec2 offset;
		glm::vec2 size;
		ParticleInfo(float minA,float maxA,float minS , float maxS, float dir, bool burst_, float timer_, float gravity,bool wave,glm::vec2 offset_, glm::vec2 size_) 
			:minAngle(minA), maxAngle(maxA), minSpeed(minS), maxSpeed(maxS), direction(dir),
			burst(burst_),timer(timer_), gravity_value(gravity), wavey(wave), offset(offset_), size(size_) {}
		ParticleInfo() :minAngle(0), maxAngle(360), minSpeed(0), maxSpeed(80), direction(90),
			burst(true), timer(1.0f), gravity_value(0.0f), wavey(false), offset({0,0}), size({ 0,0 }) {}
	};
	enum class emmiter_type:int { point, square, circle };
	class ParticleEmitter : public IComponent
	{
	public:
		ParticleEmitter(Engine *engine);
		~ParticleEmitter();

		void Init() override;
		void Update(float dt) override;
		inline int GetParentID() const override { return parentID; }
		inline void SetParentID(int ID) override { parentID = ID; }
		inline const std::string& GetName() const override { return theName; }
		void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;
		inline void SetPause(bool Condition)override { paused = Condition; }
		inline bool GetPause() const override { return paused; }

		//dont implement until level editor is up
		void Write(IJsonWriter& writer) const override;
		inline void SetID(int ID)override { my_ID = ID; }
		void ImGuiDisplay(float scale)override;
		inline const int GetID() const override { return my_ID; }
		inline std::vector<Particle>const& GetParticle()const { return burst_particles; }
		inline std::vector<Particle>const& GetParticleTrail()const { return trail_particles; }
		const int GetTextureId()const;
		void ResetParticles();
		inline float GetLifeOfParticles()const { return lifeOfParticles; }
		void SetBurst(bool value);
		ParticleInfo CopyInfo();
		void SetInfo(ParticleInfo& information);
		void SetBurstInfo(ParticleInfo information);
		inline void setTimer(float new_timer) { info.timer = new_timer; };
	private:
		inline static const std::string theName = "ParticleEmitter";
		Engine* theEngine;
		std::vector<int> textureID; // The texture of the particles
		int parentID;
		int numOfParticles;
		float lifeOfParticles;
		float fadeRate = 0.075f;
		int my_ID;
		int particleIndex;

		ParticleInfo info;
		ParticleInfo burst_info = info;

		emmiter_type type = emmiter_type::point;
		std::vector<Particle> trail_particles;
		std::vector<Particle> burst_particles;


		float decrementTimer = 1.0f;
		void makeParticle(Particle* theParticle)const;
		void makeBurstParticle(Particle* theParticle)const;
		bool paused = false;
	};//End Particle Emitter Class

} // End Of Dapper Namespace
//------------------------------------------------------------------------------