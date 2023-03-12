//------------------------------------------------------------------------------
//
// File Name:	SpriteManager.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#include "SpriteManager.h"
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
	SpriteManager::SpriteManager(Engine& engine1)
		: engine(engine1)
	{
	}

	/*--------------------------------------
	Description: Destructor
	Params:
	Return: Void
	---------------------------------------*/
	SpriteManager::~SpriteManager()
	{
	}

	/*--------------------------------------
	Description: Initializes the Sprite Manager.
	Params: 
	Return: Void
	---------------------------------------*/
	void SpriteManager::Initialize()
	{
		ComponentRegistry::FunctionBuffer SpriteFunctions{ SpriteCreate, SpriteDestroy, SpriteGet };

		MeshManager* mesh_manager = GetSystemFromEngine<MeshManager*>(engine, "MeshManager");
		mesh_manager->RegisterFunction(std::bind(&SpriteManager::OnMeshReallocation, this, std::placeholders::_1));

		dynamic_cast<Deserializer*>(engine.Get("Deserializer"))->RegisterFunction("Sprite", SpriteDeserialize);
		dynamic_cast<ComponentRegistry*>(engine.Get("ComponentRegistry"))->Register("Sprite", SpriteFunctions);
	}

	/*--------------------------------------
	Description: Creates and reads a new sprite into the sprite manager.
	Params:
		Engine& engine - the engine.
		const Value& val - the json file to be read from.
	Return: The string "Sprite" and the index of the new sprite.
	---------------------------------------*/
	std::pair<std::string_view, int> SpriteDeserialize(Engine& engine, const rapidjson::Value& val)
	{
		// Grab the sprite manager, new sprite, and deserializer.
		SpriteManager& manager = *dynamic_cast<SpriteManager*>(engine.Get("SpriteManager"));
		int index = manager.GetNew();
		Sprite& sprite = manager[index];
		Deserializer& deserializer = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));

		// Read the sprite.
		sprite.Read(deserializer, val);
		return { sprite.GetName(), index };
	}

	/*--------------------------------------
	Description: Returns a sprite at the specified index.
	Params: int index - the index of the sprite.
	Return: The sprite at the specified index.
	---------------------------------------*/
	Sprite& SpriteManager::operator[](int ID)
	{
		return sprites[ID_manager[ID]];
	}

	Sprite& SpriteManager::operator[](std::string sprite_name)
	{
		for (Sprite& sprite : sprites)
		{

			if (sprite.GetName() == sprite_name)
			{
				return sprite;
			}
		}
		throw std::out_of_range("Shader name does not exist");
	}

	/*--------------------------------------
	Description: Unused update function
	Params:
		float dt - the time between frames.
	Return: Void
	---------------------------------------*/
	void SpriteManager::Update(float dt)
	{
		dt;
	}

	/*--------------------------------------
	Description: Renders all of the sprites.
	Params:
	Return: Void
	---------------------------------------*/
	void SpriteManager::Render()
	{
		// Go through the vector and render every sprite.
		int spritesLength = (int)sprites.size();
		for (int i = 0; i < spritesLength; i++)
		{
			sprites[i].Render(engine);
		}
	}
	void SpriteManager::Destroy(int ID)
	{
		ID_manager.Destroy(sprites, ID);
	}

	/*--------------------------------------
	Description: Returns "SpriteManager"
	Params:
	Return: "SpriteManager"
	---------------------------------------*/
	const std::string& SpriteManager::Name() const
	{
		return name;
	}

	/*--------------------------------------
	Description: Creates a new sprite and adds it to the manager.
	Params:
	Return: the index of the new sprite.
	---------------------------------------*/
	int SpriteManager::GetNew()
	{
		// Add the new sprite
		Sprite newSprite(&engine);
		int obj_ID = ID_manager.Create(sprites, newSprite);
		sprites[ID_manager[obj_ID]].SetID(obj_ID);
		return obj_ID;
	}

	bool SpriteManager::UpdateWhilePaused() const
	{
		return false;
	}

	void SpriteManager::OnMeshReallocation(MeshManager& mesh_manager)
	{
		int size = sprites.size();
		for (int i = 0; i < size; ++i)
		{
			sprites[i].OnMeshReallocation(mesh_manager);
		}
	}

	/*--------------------------------------
	Description: Creates a new sprite and adds it to the manager.
	Params:
	Return: a pointer to the new sprite.
	---------------------------------------*/
	Sprite* SpriteCreate(Engine& engine)
	{

		SpriteManager& the_man = *dynamic_cast<SpriteManager*>(engine.Get("SpriteManager"));
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
	void SpriteDestroy(int ID, Engine& engine)
	{
		// Destroy the sprite at the specified location.
		SpriteManager& the_man = *dynamic_cast<SpriteManager*>(engine.Get("SpriteManager"));

		the_man.Destroy(ID);
	}

	/*--------------------------------------
	Description: Returns a sprite at the specified index.
	Params:
		int ID- the ID of the sprite to be returned.
		Engine& engine- the engine.
	Return: The sprite of the specified ID in the manager.
	---------------------------------------*/
	Sprite* SpriteGet(int ID, Engine& engine)
	{
		// Return the sprite at the specified location.
		SpriteManager& the_man = *dynamic_cast<SpriteManager*>(engine.Get("SpriteManager"));
		return &the_man[ID];
	}

} // End Of Dapper Namespace