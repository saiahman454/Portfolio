//------------------------------------------------------------------------------
//
// File Name:	HealthManager.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "HealthManager.h"
#include "ComponentRegistry.h"
#include "Utility.h"
#include "Transform.h"
#include "Camera.h"
#include <cmath>

// Dapper Namespace!
namespace Dapper
{
	HealthManager::HealthManager(Engine& engine) :
		the_engine(engine)
	{
	}

	HealthManager::~HealthManager()
	{
	}

	const std::string& HealthManager::Name() const
	{
		return name;
	}

	void HealthManager::Initialize()
	{
		//register the deserialize function
		ComponentRegistry::FunctionBuffer HealthFunctions
		{ HealthCreate, HealthDestroy, HealthGet };

		dynamic_cast<Deserializer*>(the_engine.Get("Deserializer"))->
			RegisterFunction("Health", HealthDeserialize);

		dynamic_cast<ComponentRegistry*>
			(the_engine.Get("ComponentRegistry"))
			->Register("Health", HealthFunctions);
	}

	void HealthManager::Update(float dt)
	{
		for (Health& theHealth : health_list)
		{
			if (theHealth.GetPause() == false)
			{
				theHealth.Update(dt);
			}
		}
	}

	void HealthManager::Render()
	{
	}

	Health& HealthManager::operator[](int index)
	{
		return health_list[idManager[index]];
	}

	int HealthManager::GetNew()
	{
		// Add the new health
		Health newHealth(&the_engine);
		int obj_ID = idManager.Create(health_list, newHealth);
		health_list[idManager[obj_ID]].SetID(obj_ID);
		//health_list[idManager[obj_ID]].Init();
		return obj_ID;
	}

	std::pair<Health*, int> HealthManager::HasHealth(std::string health_name)
	{
		int count = 0;
		for (Health& the_health : health_list)
		{
			if (the_health.GetName() == health_name)
			{
				return { &the_health, count };
			}
			count++;
		}

		return { nullptr, -1 };
	}

	void HealthManager::Destroy(int ID)
	{
		idManager.Destroy(health_list, ID);
	}

	bool HealthManager::UpdateWhilePaused() const
	{
		return false;
	}

	std::pair<std::string_view, int> HealthDeserialize(Engine& engine, const rapidjson::Value& val)
	{
		HealthManager* manager = dynamic_cast<HealthManager*>
			(engine.Get("HealthManager"));
		Deserializer* deserial = dynamic_cast<Deserializer*>
			(engine.Get("Deserializer"));
		int ID = manager->GetNew();
		Health& toReturn = (*manager)[ID];
		toReturn.Read(*deserial, val);
		return { toReturn.GetName(), ID };
	}

	Health* HealthCreate(Engine& engine)
	{
		HealthManager& the_man = *dynamic_cast<HealthManager*>
			(engine.Get("HealthManager"));
		int index = the_man.GetNew();
		return &the_man[index];
	}

	void HealthDestroy(int ID, Engine& engine)
	{
		HealthManager& the_man = *dynamic_cast<HealthManager*>
			(engine.Get("HealthManager"));
		the_man.Destroy(ID);
	}

	Health* HealthGet(int ID, Engine& engine)
	{
		HealthManager& the_man = *dynamic_cast<HealthManager*>
			(engine.Get("HealthManager"));
		return &the_man[ID];
	}

} // End Of Dapper Namespace