//------------------------------------------------------------------------------
//
// File Name:	FullScreenBehavior.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "FullScreenBehavior.h"
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
#include "Window.h"


// Dapper Namespace!
namespace Dapper {
	FullScreenBehavior::FullScreenBehavior(int ID_, Engine& engine_)
		: my_ID(ID_)
		, engine(engine_)
	{
	}

	FullScreenBehavior::~FullScreenBehavior()
	{
	}

	void FullScreenBehavior::Init()
	{
		//Camera* theCamera = GetSystemFromEngine<Camera*>(engine, "Camera");
		//Transform& theTransform = *GetComponentFromParent<Transform>(engine, my_ID, "Transform");

		//theTransform.setTranslation(theCamera->getCameraPosition());
		//theTransform.AddToPosition(glm::vec2(-500.0f, -3200.0));
		Window* theWindow = GetSystemFromEngine<Window*>(engine, "Window");
		isOn = theWindow->IsFullScreen();

		if (isOn == false)
		{
			ChangeTexture(buttonOff.c_str());
		}
		else
		{
			ChangeTexture(buttonOn.c_str());
		}
	}

	void FullScreenBehavior::Update(float dt)
	{
		dt;
		Input& input = *dynamic_cast<Input*>(engine.Get("Input"));


		if (MouseOnButton()
			&& input.getMouse(GLFW_MOUSE_BUTTON_1) == mouse_button::MOUSE_RELEASE
			&& isOn == false)
		{
			Window* theWindow = GetSystemFromEngine<Window*>(engine, "Window");
			theWindow->SetFullScreen(true);
			ChangeTexture(buttonOn.c_str());
			isOn = true;
		}
		else if (MouseOnButton()
			&& input.getMouse(GLFW_MOUSE_BUTTON_1) == mouse_button::MOUSE_RELEASE
			&& isOn == true)
		{
			Window* theWindow = GetSystemFromEngine<Window*>(engine, "Window");
			theWindow->SetFullScreen(false);
			ChangeTexture(buttonOff.c_str());
			isOn = false;
		}

	}

	void FullScreenBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
		deserial;
		auto daObject = jsonVal.GetObject();
		//isOn = jsonVal["IsOn"].GetBool();
		buttonOn = jsonVal["ButtonOn"].GetString();
		//buttonHovered = jsonVal["ButtonHovered"].GetString();
		buttonOff = jsonVal["ButtonOff"].GetString();
		//scrollRate = daObject["ScrollRate"].GetFloat();
		//mainMenu = daObject["MainMenu"].GetBool();
		
	}

	void FullScreenBehavior::Write(IJsonWriter& writer) const
	{
		writer.WriteKey("FullScreenBehavior");
		writer.StartObject();
		//writer.WriteBool("IsOn", isOn);
		writer.WriteString("ButtonOn", buttonOn.c_str());
		//writer.WriteString("ButtonHovered", buttonHovered.c_str());
		writer.WriteString("ButtonOff", buttonOff.c_str());
		writer.EndObject();
	}

	void FullScreenBehavior::ImGuiDisplay(float scale)
	{
		// Display editable values as float inputs.
		scale;
	}

	void FullScreenBehavior::ChangeTexture(std::string input)
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

	bool FullScreenBehavior::MouseOnButton() const
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