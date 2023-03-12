//------------------------------------------------------------------------------
//
// File Name:	SpriteTextManager.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "SpriteTextManager.h"
#include "ComponentRegistry.h"
#include <fstream>
#include "Utility.h"

// Dapper Namespace!
namespace Dapper {
	/*--------------------------------------
	Description: Default Constructor
	Params: Engine& engine1 - the engine.
	Return: Void
	---------------------------------------*/
	SpriteTextManager::SpriteTextManager(Engine& engine1)
		: engine(engine1)
	{
	}

	/*--------------------------------------
	Description: Destructor
	Params:
	Return: Void
	---------------------------------------*/
	SpriteTextManager::~SpriteTextManager()
	{
	}

	/*--------------------------------------
	Description: Initializes the Sprite Manager.
	Params: 
	Return: Void
	---------------------------------------*/
	void SpriteTextManager::Initialize()
	{
		ComponentRegistry::FunctionBuffer SpriteTextFunctions{ SpriteTextCreate, SpriteTextDestroy, SpriteTextGet };

		MeshManager* mesh_manager = GetSystemFromEngine<MeshManager*>(engine, "MeshManager");
		mesh_manager->RegisterFunction(std::bind(&SpriteTextManager::OnMeshReallocation, this, std::placeholders::_1));

		dynamic_cast<Deserializer*>(engine.Get("Deserializer"))->RegisterFunction("SpriteText", SpriteTextDeserialize);
		dynamic_cast<ComponentRegistry*>(engine.Get("ComponentRegistry"))->Register("SpriteText", SpriteTextFunctions);
	}

	/*--------------------------------------
	Description: Creates and reads a new sprite into the sprite manager.
	Params:
		Engine& engine - the engine.
		const Value& val - the json file to be read from.
	Return: The string "Sprite" and the index of the new sprite.
	---------------------------------------*/
	std::pair<std::string_view, int> SpriteTextDeserialize(Engine& engine, const rapidjson::Value& val)
	{
		// Grab the sprite manager, new sprite, and deserializer.
		SpriteTextManager& manager = *dynamic_cast<SpriteTextManager*>(engine.Get("SpriteTextManager"));
		int index = manager.GetNew();
		SpriteText& spritetext = manager[index];
		Deserializer& deserializer = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));

		// Read the sprite.
		spritetext.Read(deserializer, val);
		return { spritetext.GetName(), index };
	}

	/*--------------------------------------
	Description: Returns a sprite at the specified index.
	Params: int index - the index of the sprite.
	Return: The sprite at the specified index.
	---------------------------------------*/
	SpriteText& SpriteTextManager::operator[](int ID)
	{
		return spritetexts[ID_manager[ID]];
	}

	/*--------------------------------------
	Description: Unused update function
	Params:
		float dt - the time between frames.
	Return: Void
	---------------------------------------*/
	void SpriteTextManager::Update(float dt)
	{
		dt;
	}

	/*--------------------------------------
	Description: Renders all of the sprites.
	Params:
	Return: Void
	---------------------------------------*/
	void SpriteTextManager::Render()
	{
		// Go through the vector and render every sprite.
		int spritesLength = (int)spritetexts.size();
		for (int i = 0; i < spritesLength; i++)
		{
			spritetexts[i].Render(engine);
		}
	}
	void SpriteTextManager::Destroy(int ID)
	{
		ID_manager.Destroy(spritetexts, ID);
	}

	/*--------------------------------------
	Description: Returns "SpriteTextManager"
	Params:
	Return: "SpriteTextManager"
	---------------------------------------*/
	const std::string& SpriteTextManager::Name() const
	{
		return name;
	}

	/*--------------------------------------
	Description: Creates a new sprite text and adds it to the manager.
	Params:
	Return: the index of the new sprite text.
	---------------------------------------*/
	int SpriteTextManager::GetNew()
	{
		// Add the new sprite
		SpriteText newSpriteText(&engine);
		int capacity = spritetexts.capacity();
		int size = spritetexts.size();
		int obj_ID = ID_manager.Create(spritetexts, newSpriteText);
		spritetexts[ID_manager[obj_ID]].SetID(obj_ID);
		// Send message to Level Collectible UI Behavior to change parent sprite text when the manager reallocates
		if (capacity == size)
		{
			for (auto& spritetext : spritetexts)
			{
				spritetext.on_reallocate.SendMessage(&spritetext);
			}
		}
		return obj_ID;
	}

	bool SpriteTextManager::UpdateWhilePaused() const
	{
		return false;
	}

	void SpriteTextManager::OnMeshReallocation(MeshManager& mesh_manager)
	{
		int size = spritetexts.size();
		for (int i = 0; i < size; ++i)
		{
			spritetexts[i].OnMeshReallocation(mesh_manager);
		}
	}

	/*--------------------------------------
	Description: Creates a new sprite and adds it to the manager.
	Params:
	Return: a pointer to the new sprite.
	---------------------------------------*/
	SpriteText* SpriteTextCreate(Engine& engine)
	{

		SpriteTextManager& the_man = *dynamic_cast<SpriteTextManager*>(engine.Get("SpriteTextManager"));
		int index = the_man.GetNew();
		return &the_man[index];

	}

	/*--------------------------------------
	Description: Destroys a sprite at the specified index.
	Params: 
		int ID- the ID of the sprite to be destroyed.
		Engine& engine- the engine.
	Return: void
	---------------------------------------*/
	void SpriteTextDestroy(int ID, Engine& engine)
	{
		// Destroy the sprite at the specified location.
		SpriteTextManager& the_man = *dynamic_cast<SpriteTextManager*>(engine.Get("SpriteTextManager"));

		the_man.Destroy(ID);
	}

	/*--------------------------------------
	Description: Returns a sprite at the specified index.
	Params:
		int ID- the ID of the sprite to be returned.
		Engine& engine- the engine.
	Return: The sprite of the specified ID in the manager.
	---------------------------------------*/
	SpriteText* SpriteTextGet(int ID, Engine& engine)
	{
		// Return the sprite at the specified location.
		SpriteTextManager& the_man = *dynamic_cast<SpriteTextManager*>(engine.Get("SpriteTextManager"));
		return &the_man[ID];
	}

} // End Of Dapper Namespace