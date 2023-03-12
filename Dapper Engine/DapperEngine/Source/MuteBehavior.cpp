//------------------------------------------------------------------------------
//
// File Name:	MuteBehavior.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "MuteBehavior.h"
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
	MuteBehavior::MuteBehavior(int ID_, Engine& engine_)
		: my_ID(ID_)
		, engine(engine_)
	{
	}

	MuteBehavior::~MuteBehavior()
	{
	}

	void MuteBehavior::Init()
	{
		//Camera* theCamera = GetSystemFromEngine<Camera*>(engine, "Camera");
		//Transform& theTransform = *GetComponentFromParent<Transform>(engine, my_ID, "Transform");

		//theTransform.setTranslation(theCamera->getCameraPosition());
		//theTransform.AddToPosition(glm::vec2(-500.0f, -3200.0));
		Audio* theAudio = GetSystemFromEngine<Audio*>(engine, "Audio");
		Dapper::Bus* theBus = theAudio->GetBus("/");
		isOn = theBus->GetMute();
		if (isOn == false)
		{
			ChangeTexture(buttonOff.c_str());
		}
		else
		{
			ChangeTexture(buttonOn.c_str());
		}
	}

	void MuteBehavior::Update(float dt)
	{
		dt;
		Input& input = *dynamic_cast<Input*>(engine.Get("Input"));


		if (MouseOnButton()
			&& input.getMouse(GLFW_MOUSE_BUTTON_1) == mouse_button::MOUSE_RELEASE
			&& isOn == false)
		{
			Audio* theAudio = GetSystemFromEngine<Audio*>(engine, "Audio");
			Dapper::Bus* theBus = theAudio->GetBus("/");
			theBus->SetMute(true);
			ChangeTexture(buttonOn.c_str());
			isOn = true;
		}
		else if (MouseOnButton()
			&& input.getMouse(GLFW_MOUSE_BUTTON_1) == mouse_button::MOUSE_RELEASE
			&& isOn == true)
		{
			Audio* theAudio = GetSystemFromEngine<Audio*>(engine, "Audio");
			Dapper::Bus* theBus = theAudio->GetBus("/");
			theBus->SetMute(false);
			ChangeTexture(buttonOff.c_str());
			isOn = false;
		}

	}

	void MuteBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
		deserial;
		auto daObject = jsonVal.GetObject();
		buttonOn = jsonVal["ButtonOn"].GetString();
		buttonOff = jsonVal["ButtonOff"].GetString();
		//scrollRate = daObject["ScrollRate"].GetFloat();
		//mainMenu = daObject["MainMenu"].GetBool();
	}

	void MuteBehavior::Write(IJsonWriter& writer) const
	{
		writer.WriteKey("MuteBehavior");
		writer.StartObject();
		//writer.WriteFloat("ScrollRate", scrollRate);
		writer.WriteString("ButtonOn", buttonOn.c_str());
		writer.WriteString("ButtonOff", buttonOff.c_str());
		//writer.WriteBool("MainMenu", mainMenu);
		writer.EndObject();
	}

	void MuteBehavior::ImGuiDisplay(float scale)
	{
		// Display editable values as float inputs.
		scale;
	}

	void MuteBehavior::ChangeTexture(std::string input)
	{
		TextureManager& tex_man = *GetSystemFromEngine<TextureManager*>
			(engine, "TextureManager");
		SpriteSource* theSpriteSource = GetComponentFromParent<SpriteSource>
			(engine, parent_ID, "SpriteSource");

		if (!input.empty())
		{
			tex_man.LoadNewTexture("./Assets/Texture/" + input + ".png");
			//tex_man.LoadNewTexture(input);
			if (tex_man.Search(input) != -1)
			{
				theSpriteSource->SetTextureID(tex_man.Search(input));
			}
		}
	}

	bool MuteBehavior::MouseOnButton() const
	{
		Input& input = *dynamic_cast<Input*>(engine.Get("Input"));
		glm::vec2 theCoords = input.GetMousePosinWorldCoords();

		Transform* theTransform = GetComponentFromParent<Transform>
			(engine, parent_ID, "Transform");
		glm::vec3 theButton = theTransform->GetTranslation();
		float theWidth = (theTransform->GetScale().x * 0.5);
		float theHeight = (theTransform->GetScale().y * 0.5);
		// input.getMouse(GLFW_MOUSE_BUTTON_1) == mouse_button::MOUSE_RELEASE &&
		//If met we are inside the box and click released
		if (theCoords.x <= theButton.x + theWidth &&
			theCoords.x >= theButton.x - theWidth &&
			theCoords.y <= theButton.y + theHeight &&
			theCoords.y >= theButton.y - theHeight)
		{
			return true;
		}
		return false;
	}

} // End Of Dapper Namespace