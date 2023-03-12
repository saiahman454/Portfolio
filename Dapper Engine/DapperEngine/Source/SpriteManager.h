//------------------------------------------------------------------------------
//
// File Name:	SpriteManager.h
// Author(s):	Natalie Bouley
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "ITypeManager.h"
#include "Sprite.h"
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

  class SpriteManager : public ITypeManager<Sprite>
	{
  public:
		// Constructor with Engine
		SpriteManager(Engine& engine1);

		// Destructor
		~SpriteManager() override;

		// Initialize the manager
		void Initialize() override;

		// Returns specific sprite at the specified index
		Sprite& operator[] (int index) override;

		Sprite& operator[](std::string sprite_name);

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
		const static inline std::string name = "SpriteManager";

		// Vector to hold all of the sprites.
		std::vector <Sprite> sprites;

		// Reference to the engine
		Engine& engine;

		IDManager ID_manager;

		// Inherited via ITypeManager
		virtual bool UpdateWhilePaused() const override;

		void OnMeshReallocation(MeshManager& mesh_manager);
	};

	// NON-member functions
	// Create an sprite and read it
	std::pair<std::string_view, int> SpriteDeserialize(Engine& engine, const rapidjson::Value& val);

	// Create an animation component and add it to the manager
	Sprite* SpriteCreate(Engine& engine);

	// Destroy an animation
	void SpriteDestroy(int ID, Engine& engine);

	// Get a sprite at a specified ID
	Sprite* SpriteGet(int ID, Engine& engine);


} // End Of Dapper Namespace

//------------------------------------------------------------------------------


