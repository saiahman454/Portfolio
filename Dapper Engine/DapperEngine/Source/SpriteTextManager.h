//------------------------------------------------------------------------------
//
// File Name:	SpriteTextManager.h
// Author(s):	Natalie Bouley
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "ITypeManager.h"
#include "SpriteText.h"
#include "Engine.h"
#include "IDManager.h"
#include "MeshManager.h"
#include <vector>
#include <stdlib.h>
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class SpriteTextManager : public ITypeManager<SpriteText>
	{
  public:
		// Constructor with Engine
		SpriteTextManager(Engine& engine1);

		// Destructor
		~SpriteTextManager() override;

		// Initialize the manager
		void Initialize() override;

		// Returns specific sprite at the specified index
		SpriteText& operator[] (int index) override;

		// Not needed
		void Update(float dt) override;

		// Render function (draw function?)
		void Render() override;

		// Destroy a sprite
		void Destroy(int ID);

		// Names for debugging 
		virtual const std::string& Name()const override;

		// Add a sprite to the Sprite Manager
		int GetNew()override;


	private:
		// Name of the Manager
		const static inline std::string name = "SpriteTextManager";

		// Vector to hold all of the spritetexts
		std::vector <SpriteText> spritetexts;

		// Reference to the engine
		Engine& engine;

		IDManager ID_manager;

		// Inherited via ITypeManager
		virtual bool UpdateWhilePaused() const override;

		void OnMeshReallocation(MeshManager& mesh_manager);
	};

	// NON-member functions
	// Create an sprite and read it
	std::pair<std::string_view, int> SpriteTextDeserialize(Engine& engine, const rapidjson::Value& val);

	// Create an animation component and add it to the manager
	SpriteText* SpriteTextCreate(Engine& engine);

	// Destroy an animation
	void SpriteTextDestroy(int ID, Engine& engine);

	// Get a sprite at a specified ID
	SpriteText* SpriteTextGet(int ID, Engine& engine);


} // End Of Dapper Namespace

//------------------------------------------------------------------------------


