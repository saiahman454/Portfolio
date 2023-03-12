//------------------------------------------------------------------------------
//
// File Name:	AnimationManager.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "AnimationManager.h"
#include "ComponentRegistry.h"
#include "AnimationSpine.h"
#include <fstream>
#include "Utility.h"

// Dapper Namespace!
namespace Dapper {

	/*--------------------------------------
	Description: Default Constructor
	Params: Engine& engine1 - the engine.
	Return: Void
	---------------------------------------*/
	AnimationManager::AnimationManager(Engine& engine1)
		: engine(engine1)
	{
	}

	/*--------------------------------------
	Description: Destructor
	Params:
	Return: Void
	---------------------------------------*/
	AnimationManager::~AnimationManager()
	{
	}

	/*--------------------------------------
	Description: Initializes the Animation Manager.
	Params:
	Return: Void
	---------------------------------------*/
	void AnimationManager::Initialize()
	{
		ComponentRegistry::FunctionBuffer AnimationSpriteFunctions{ AnimationSpriteCreate, AnimationDestroy, AnimationGet };
		ComponentRegistry::FunctionBuffer AnimationSpineFunctions{ AnimationSpineCreate, AnimationDestroy, AnimationGet };

		dynamic_cast<Deserializer*>(engine.Get("Deserializer"))->RegisterFunction("Animation", AnimationDeserialize);

		dynamic_cast<ComponentRegistry*>(engine.Get("ComponentRegistry"))->Register("AnimationSprite", AnimationSpriteFunctions);
		dynamic_cast<ComponentRegistry*>(engine.Get("ComponentRegistry"))->Register("AnimationSpine", AnimationSpineFunctions);

	}

	void AnimationManager::Update(float dt)
	{
		// Go through the vector and update every animation.
		int animations_length = (int)animations.size();
		for (int i = 0; i < animations_length; i++)
		{
			animations[i]->Update(dt);
		}
	}

	void AnimationManager::Render()
	{
	}

	// Destroy an animation at the specified ID index.
	void AnimationManager::Destroy(int ID)
	{
		ID_manager.Destroy(animations, ID);
	}

	// Names for Debugging
	const std::string& AnimationManager::Name() const
	{
		return name;
	}

	// Add a Animation to the Animation Manager
	int AnimationManager::GetNew()
	{
		int obj_ID = ID_manager.Create<IAnimation*>(animations, nullptr);
		return obj_ID;
	}

	bool AnimationManager::UpdateWhilePaused()const
	{
		return true;
	}

	/*--------------------------------------
	Description: Returns an animation at the specified index.
	Params: int index - the index of the sprite.
	Return: The animation at the specified index.
	---------------------------------------*/
	IAnimation*& AnimationManager::operator[](int ID)
	{
		return animations[ID_manager[ID]];
	}

	/*--------------------------------------
	Description: Creates and reads a new animation into the animation manager.
	Params:
		Engine& engine - the engine.
		const Value& val - the json file to be read from.
	Return: The string "Animation" and the index of the new animation.
	---------------------------------------*/
	std::pair<std::string_view, int> AnimationDeserialize(Engine& engine, const rapidjson::Value& val)
	{
		auto object = val.GetObject();
		std::string type = object["Type"].GetString();

		AnimationManager* manager = dynamic_cast<AnimationManager*>(engine.Get("AnimationManager"));
		Deserializer& deserializer = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));

		if (type == "Sprite")
		{
			int index = manager->GetNew();
			AnimationSprite* animation_sprite = new AnimationSprite(index, &engine);
			(*manager)[index] = animation_sprite;

			animation_sprite->Read(deserializer, val);
			return { animation_sprite->GetName(), index };

			
		}
		else if (type == "Spine")
		{
			int index = manager->GetNew();
			AnimationSpine* animation_spine = new AnimationSpine(index, &engine); 
			(*manager)[index] = animation_spine;

			animation_spine->Read(deserializer, val);
			return { animation_spine->GetName(), index };
		}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
	}

	/*--------------------------------------
	Description: Creates a new animation and adds it to the manager.
	Params:
	Return: a pointer to the new animation.
	---------------------------------------*/
	IAnimation* AnimationCreate(Engine& engine)
	{
		// Create an animation, grab the animation manager, and return the new animation.
		AnimationManager& manager = *dynamic_cast<AnimationManager*>(engine.Get("AnimationManager"));
		int index = manager.GetNew();
		return manager[index];
	}

	IAnimation* AnimationSpriteCreate(Engine& engine)
	{
	  AnimationManager& manager = *dynamic_cast<AnimationManager*>(engine.Get("AnimationManager"));
	  int index = manager.GetNew();
	  manager[index] = new AnimationSprite(index, &engine);
	  return manager[index];
	}


	IAnimation* AnimationSpineCreate(Engine& engine)
	{
	  AnimationManager& manager = *dynamic_cast<AnimationManager*>(engine.Get("AnimationManager"));
	  int index = manager.GetNew();
	  manager[index] = new AnimationSpine(index, &engine);
	  return manager[index];
	}

	/*--------------------------------------
	Description: Destroys an animation at the specified indedx.
	Params:
		int ID- the ID of the animation to be destroyed.
		Engine& engine- the engine.
	Return: void
	---------------------------------------*/
	void AnimationDestroy(int ID, Engine& engine)
	{
		// Destroy the animation at the specified location.
		AnimationManager& manager = *dynamic_cast<AnimationManager*>(engine.Get("AnimationManager"));
		manager.Destroy(ID);
	}

	/*--------------------------------------
	Description: Returns an animation at the specified index.
	Params:
		int ID- the ID of the animation to be returned.
		Engine& engine- the engine.
	Return: The animation of the specified ID  the manager.
	---------------------------------------*/
	IAnimation* AnimationGet(int ID, Engine& engine)
	{
		// Return the animation at the specified location.
		AnimationManager& manager = *dynamic_cast<AnimationManager*>(engine.Get("AnimationManager"));
		return manager[ID];
	}

} // End Of Dapper Namespace