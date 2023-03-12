//------------------------------------------------------------------------------
//
// File Name:	AnimationManager.h
// Author(s):	Natalie Bouley
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
#include "ITypeManager.h"
#include "AnimationSprite.h"
#include "Engine.h"
#include "IDManager.h"
#include <vector>
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class AnimationManager : public ITypeManager<IAnimation*>
  {
	public:
		// Constructor with Engine
		AnimationManager(Engine& engine);

		// Destructor
		~AnimationManager() override;

		// Initialize the manager
		void Initialize() override;

		// Returns specific animation at the specified index
		IAnimation*& operator[] (int index) override;

		// Not needed
		void Update(float dt) override;

		// Render function (draw function?)
		void Render() override;

		// Destroy an animation
		void Destroy(int ID);

		// Names for debugging 
		virtual const std::string& Name()const override;

		// Add a sprite to the Animation Manager
		int GetNew()override;



	private:
		// Name of the Manager
		const std::string name = "AnimationManager";

		virtual bool UpdateWhilePaused()const override;

		// Vector to hold all of the animations
		std::vector <IAnimation*> animations;

		// The engine
		Engine& engine;

		IDManager ID_manager;
  };

	// NON-member functions
	// Create an animation and read it
	std::pair<std::string_view, int> AnimationDeserialize(Engine& engine, const rapidjson::Value& val);

	// Create an animation component and add it to the manager
	IAnimation* AnimationCreate(Engine& engine);
	IAnimation* AnimationSpineCreate(Engine& engine);
	IAnimation* AnimationSpriteCreate(Engine& engine);

	// Destroy an animation
	void AnimationDestroy(int ID, Engine& engine);

	// Get a sprite at a specified ID
	IAnimation* AnimationGet(int ID, Engine& engine);
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


