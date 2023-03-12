//------------------------------------------------------------------------------
//
// File Name:	Deserializer.cpp
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "GameObject.h"
#include "ComponentRegistry.h"
#include "IComponent.h"
#include "Utility.h"


// Dapper Namespace!
namespace Dapper
{
	/*--------------------------------------
	Description: GameObject constructor
	Params:
	int id - the idex that the gameobject is in
	---------------------------------------*/
	GameObject::GameObject(Engine* engine) : my_engine(engine), ID(-1),
		paused(false)
	{
	}

	/*--------------------------------------
	Description: GameObject destructor
	---------------------------------------*/
	GameObject::~GameObject()
	{
	}



	/*--------------------------------------
	Description: Calls all the compoents Destroy
	functions to make an empty game object for
	decostruction
	---------------------------------------*/
	void GameObject::PrepDestroy()
	{
		ComponentRegistry& registry = *GetSystemFromEngine<ComponentRegistry*>(*my_engine, "ComponentRegistry");
		for (std::pair current : components)
		{
			registry.Destroy(current.first, current.second);
		}
	}

	/*--------------------------------------
	Description: Removes a component from a GameObject
	Params: comp_name - the name of the component you
	want to remove
	---------------------------------------*/
	void GameObject::RemoveComponent(const std::string_view comp_name)
	{
		components.erase(comp_name);
	}
	/*--------------------------------------
	Description: Adds a component to a GameObject
	Params: comp_name - the name of the component
	ID - the id of that component
	---------------------------------------*/
	void GameObject::AddComponent(const std::string_view comp_name, int ID)
	{
		components.insert({ comp_name, ID });
	}
	/*--------------------------------------
	Description: Writes a gameobject to a json file
	Params: writer - the custom writer class that hold
	writing functions and hold the file destination.
	---------------------------------------*/
	void GameObject::Write(IJsonWriter& writer)
	{
		ComponentRegistry& reg = *GetSystemFromEngine<ComponentRegistry*>(*my_engine, "ComponentRegistry");
		writer.StartObject();
		writer.WriteString("Name", name.c_str());
		writer.WriteString("Flag", flag.c_str());
		for (std::pair current : components)
		{
			IComponent* comp = reg.GetComponent(current.first, current.second);
			comp->Write(writer);
		}
		writer.EndObject();
	}

	//Adam
	void GameObject::SetPause(bool Condition)
	{
		paused = Condition;
		ComponentRegistry& registry = *GetSystemFromEngine<ComponentRegistry*>
			(*my_engine, "ComponentRegistry");
		for (std::pair current : components)
		{
			IComponent* isaiisawesome = registry.GetComponent(current.first, current.second);
			if (isaiisawesome)
			{
				isaiisawesome->SetPause(Condition);
			}
		}
	}
	//Adam
	void GameObject::setEnabled(bool condition)
	{
		enabled = condition;
		ComponentRegistry& registry = *GetSystemFromEngine<ComponentRegistry*>
			(*my_engine, "ComponentRegistry");
		for (std::pair current : components)
		{
			IComponent* isaiisawesome = registry.GetComponent(current.first, current.second);
			if (isaiisawesome)
			{
				isaiisawesome->isEnabled = condition;
			}
		}
	}
		/*--------------------------------------
		Description: Getter for the game object.
		Params:
		compname - the name of the component you want.
		Return: the index of the component.
		---------------------------------------*/
		int GameObject::GetComponentID(const std::string_view & compname)const
		{
			auto iter = components.find(compname);
			if (components.end() != iter)
			{
				return iter->second;
			}
			return -1;
		}

		void GameObject::SetComponentID(const std::string_view & compname, int new_ID)
		{
			auto iter = components.find(compname);
			if (components.end() != iter)
			{
				iter->second = new_ID;
			}
		}

		/*--------------------------------------
		Description: Reads a game object.
		Param:
		deserial - the deserialzer from the engine.
		jsonVal - the json value you want to read.
		---------------------------------------*/
		void GameObject::Read(Deserializer & deserial, const rapidjson::Value & jsonVal)
		{

			auto member = jsonVal.MemberBegin();
			Engine& engine = deserial.GetEngine();
			ComponentRegistry& registry = *dynamic_cast<ComponentRegistry*>(engine.Get("ComponentRegistry"));

			while (member != jsonVal.MemberEnd())
			{
				if (strcmp(member->name.GetString(), "Name") == 0)
				{
					SetName(member->value.GetString());

				}
				else if (strcmp(member->name.GetString(), "Flag") == 0)
				{
					flag = member->value.GetString();
				}
				else
				{
					auto pairData = deserial.Deserialize(member->name.GetString(), member->value);
					if (pairData.first.empty())
					{
						++member;
						continue;
					}
					auto iter = components.insert(pairData);
					
					IComponent* comp = registry.GetComponent(iter.first->first, iter.first->second);
					if (comp)
					{
						comp->SetParentID(ID);
					}
				}
				++member;
			}

			/*for (const auto& iter : components)
			{
				registry.GetComponent(iter.first, iter.second)->Init();
			}*/

		}
		/*--------------------------------------
		Description: Initalizes the gameobject
		---------------------------------------*/
		void GameObject::Initialize()
		{
			ComponentRegistry* theRegistry =
				GetSystemFromEngine<ComponentRegistry*>(*my_engine, "ComponentRegistry");

			for (const auto& theComponent : components)
			{
				IComponent* comp = theRegistry->GetComponent
				(theComponent.first, theComponent.second);
				if (comp)
				{
					comp->Init();
				}
			}
		}


	} // End Of Dapper Namespace