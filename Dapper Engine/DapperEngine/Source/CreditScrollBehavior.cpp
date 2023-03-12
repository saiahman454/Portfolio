//------------------------------------------------------------------------------
//
// File Name:	CreditScrollBehavior.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "CreditScrollBehavior.h"
#include "ColliderManager.h"
#include "SceneManager.h"
#include "PlayerController.h"
#include "Transform.h"
#include "Utility.h"
#include "imgui.h"
#include "Input.h"
#include "Sprite.h"
#include "Camera.h"
#include "UI.h"


// Dapper Namespace!
namespace Dapper {
	CreditScrollBehavior::CreditScrollBehavior(int ID_, Engine& engine_)
		: my_ID(ID_)
		, engine(engine_)
	{
	}
	CreditScrollBehavior::~CreditScrollBehavior()
	{
	}

	void CreditScrollBehavior::Init()
	{
		//Camera* theCamera = GetSystemFromEngine<Camera*>(engine, "Camera");
		//Transform& theTransform = *GetComponentFromParent<Transform>(engine, my_ID, "Transform");

		//theTransform.setTranslation(theCamera->getCameraPosition());
		//theTransform.AddToPosition(glm::vec2(-500.0f, -3200.0));
		UI& theUi = *GetComponentFromParent<UI>(engine, parent_ID, "UI");
		theUi.SetOffset(glm::vec3(0, -3080.0f, 0));
		transitionTime = 60; //Time till main menu
	}

	void CreditScrollBehavior::Update(float dt)
	{
		currentTime += dt;

		//Sprite& fade_sprite = *GetComponentFromParent<Sprite>(engine, my_ID, "Sprite");
		UI& theUi = *GetComponentFromParent<UI>(engine, parent_ID, "UI");

		
		if (currentTime > 2.0f)
		{
			glm::vec3 theOff = theUi.GetOffset();
			theUi.SetOffset(glm::vec3(0, theOff.y + (dt * scrollRate), 0));
			//glm::vec2 moveIt = glm::vec2(0, dt * scrollRate);
			//theTransform.AddToPosition(moveIt);
		}

		if (currentTime - 2.0f > transitionTime && mainMenu == true)
		{
			SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
			scene_man.UnloadAllScenes();
			scene_man.LoadScene("MainMenu");
		}

		if (currentTime - 2.0f > transitionTime && mainMenu == false)
		{
			SceneManager& state =
				*GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
			int toUnload = state.SceneInCharge(GetParentID());

			state.UnloadScene(toUnload);

			Scene& resume = state.GetScene("Pause");
			resume.setEnableScene(true);
			resume.setPaused(false);
		}

		Input& input = *dynamic_cast<Input*>(engine.Get("Input"));
		const std::vector<keyboard_key> keys = input.GetAllKeys();
		// Skip SplashSreen
		if (keys[GLFW_KEY_SPACE] == keyboard_key::KEY_DOWN && mainMenu == true)
		{
			SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
			scene_man.FadeToScene("Credits", "MainMenu", ConditionCheck::pop);
		}
		if (keys[GLFW_KEY_SPACE] == keyboard_key::KEY_DOWN && mainMenu == false)
		{
			SceneManager& state =
				*GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
			
			state.FadeToScene("CreditsPause", "Pause", ConditionCheck::pop);
		}
	}

	void CreditScrollBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
		deserial;
		auto daObject = jsonVal.GetObject();
		scrollRate = daObject["ScrollRate"].GetFloat();
		mainMenu = daObject["MainMenu"].GetBool();
	}

	void CreditScrollBehavior::Write(IJsonWriter& writer) const
	{
		writer.WriteKey("CreditScrollBehavior");
		writer.StartObject();
		writer.WriteFloat("ScrollRate", scrollRate);
		writer.WriteBool("MainMenu", mainMenu);
		writer.EndObject();
	}

	void CreditScrollBehavior::ImGuiDisplay(float scale)
	{
		// Display editable values as float inputs.
		scale;
	}

} // End Of Dapper Namespace