//------------------------------------------------------------------------------
//
// File Name:	EmitHealthManager.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "EmitHealthManager.h"
#include "ComponentRegistry.h"
#include "Utility.h"
#include "Transform.h"
#include "Camera.h"
#include <cmath>

// Dapper Namespace!
namespace Dapper
{
	EmitHealthManager::EmitHealthManager(Engine& engine) :
		the_engine(engine)
	{
	}

	EmitHealthManager::~EmitHealthManager()
	{
	}

	const std::string& EmitHealthManager::Name() const
	{
		return name;
	}

	void EmitHealthManager::Initialize()
	{
		//register the deserialize function
		ComponentRegistry::FunctionBuffer EmitHealthFunctions
		{ EmitHealthCreate,  EmitHealthDestroy,  EmitHealthGet };

		dynamic_cast<Deserializer*>(the_engine.Get("Deserializer"))->
			RegisterFunction("EmitHealth", EmitHealthDeserialize);

		dynamic_cast<ComponentRegistry*>
			(the_engine.Get("ComponentRegistry"))
			->Register("EmitHealth", EmitHealthFunctions);
	}

	void EmitHealthManager::Update(float dt)
	{
		for (EmitHealth& theEmitHealth : emit_health_list)
		{
			if (theEmitHealth.GetPause() == false)
			{
				theEmitHealth.Update(dt);
			}
		}
	}

	void EmitHealthManager::Render()
	{
	}

	EmitHealth& EmitHealthManager::operator[](int index)
	{
		return emit_health_list[idManager[index]];
	}

	int EmitHealthManager::GetNew()
	{
		// Add the new health
		EmitHealth newEmitHealth(&the_engine);
		int obj_ID = idManager.Create(emit_health_list, newEmitHealth);
		emit_health_list[idManager[obj_ID]].SetID(obj_ID);
		//health_list[idManager[obj_ID]].Init();
		return obj_ID;
	}

	std::pair<EmitHealth*, int> EmitHealthManager::HasEmitHealth(std::string emit_health_name)
	{
		int count = 0;
		for (EmitHealth& the_emit_health : emit_health_list)
		{
			if (the_emit_health.GetName() == emit_health_name)
			{
				return { &the_emit_health, count };
			}
			count++;
		}

		return { nullptr, -1 };
	}

	void EmitHealthManager::Destroy(int ID)
	{
		idManager.Destroy(emit_health_list, ID);
	}

	bool EmitHealthManager::UpdateWhilePaused() const
	{
		return false;
	}

	std::pair<std::string_view, int> EmitHealthDeserialize(Engine& engine, const rapidjson::Value& val)
	{
		EmitHealthManager* manager = dynamic_cast<EmitHealthManager*>
			(engine.Get("EmitHealthManager"));
		Deserializer* deserial = dynamic_cast<Deserializer*>
			(engine.Get("Deserializer"));
		int ID = manager->GetNew();
		EmitHealth& toReturn = (*manager)[ID];
		toReturn.Read(*deserial, val);
		return { toReturn.GetName(), ID };
	}

	EmitHealth* EmitHealthCreate(Engine& engine)
	{
		EmitHealthManager& the_man = *dynamic_cast<EmitHealthManager*>
			(engine.Get("EmitHealthManager"));
		int index = the_man.GetNew();
		return &the_man[index];
	}

	void EmitHealthDestroy(int ID, Engine& engine)
	{
		EmitHealthManager& the_man = *dynamic_cast<EmitHealthManager*>
			(engine.Get("EmitHealthManager"));
		the_man.Destroy(ID);
	}

	EmitHealth* EmitHealthGet(int ID, Engine& engine)
	{
		EmitHealthManager& the_man = *dynamic_cast<EmitHealthManager*>
			(engine.Get("EmitHealthManager"));
		return &the_man[ID];
	}

} // End Of Dapper Namespace