//------------------------------------------------------------------------------
//
// File Name:	UI.cpp
// Author(s):	Adam Tackett (Luke Mostert)
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "UIManager.h"
#include "ComponentRegistry.h"
#include "Utility.h"
#include "Transform.h"
#include "UI.h"
#include "Camera.h"

// Dapper Namespace!
namespace Dapper
{
	UIManager::UIManager(Engine& engine) :
		the_engine(engine)
	{
	}

	UIManager::~UIManager()
	{
	}

	void UIManager::Initialize()
	{
		//register the deserialize function
		ComponentRegistry::FunctionBuffer UIFunctions
		{ UICreate, UIDestroy, UIGet };

		dynamic_cast<Deserializer*>(the_engine.Get("Deserializer"))->
			RegisterFunction("UI", UIDeserialize);

		dynamic_cast<ComponentRegistry*>
			(the_engine.Get("ComponentRegistry"))
			->Register("UI", UIFunctions);

	}

	void UIManager::Update(float dt)
	{
		for (UI& theUI : UI_list)
		{
			if (theUI.GetPause() == false)
			{
				theUI.Update(dt);
			}
		}
	}

	void UIManager::Render()
	{
	}

	int UIManager::GetNew()
	{
		// Add the new ui
		UI newUI(&the_engine);
		int obj_ID = idManager.Create(UI_list, newUI);
		UI_list[idManager[obj_ID]].SetID(obj_ID);
		//UI_list[idManager[obj_ID]].Init();
		return obj_ID;
	}

	std::pair<UI*, int> UIManager::HasUI(std::string camera_name)
	{
		int count = 0;
		for (UI& the_UI : UI_list)
		{
			if (the_UI.GetName() == camera_name)
			{
				return { &the_UI, count };
			}
			count++;
		}

		return { nullptr, -1 };
	}

	void UIManager::Destroy(int ID)
	{
		idManager.Destroy(UI_list, ID);
	}

	bool UIManager::UpdateWhilePaused() const
	{
		return false;
	}

	std::pair<std::string_view, int> UIDeserialize(Engine& engine, const rapidjson::Value& val)
	{
		UIManager* manager = dynamic_cast<UIManager*>
			(engine.Get("UIManager"));
		Deserializer* deserial = dynamic_cast<Deserializer*>
			(engine.Get("Deserializer"));
		int ID = manager->GetNew();
		UI& toReturn = (*manager)[ID];
		toReturn.Read(*deserial, val);
		return { toReturn.GetName(), ID };
	}

	UI* UICreate(Engine& engine)
	{
		UIManager& the_man = *dynamic_cast<UIManager*>
			(engine.Get("UIManager"));
		int index = the_man.GetNew();
		return &the_man[index];
	}

	void UIDestroy(int ID, Engine& engine)
	{
		UIManager& the_man = *dynamic_cast<UIManager*>
			(engine.Get("UIManager"));
		the_man.Destroy(ID);
	}

	UI* UIGet(int ID, Engine& engine)
	{
		UIManager& the_man = *dynamic_cast<UIManager*>
			(engine.Get("UIManager"));
		return &the_man[ID];
	}

} // End Of Dapper Namespace