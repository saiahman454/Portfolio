//------------------------------------------------------------------------------
//
// File Name:	SplashFadeBehavior.cpp
// Author(s):	Adam Tackett
//						
//
//
// Copyright ?2022 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "SplashFadeBehavior.h"
#include "ColliderManager.h"
#include "SceneManager.h"
#include "PlayerController.h"
#include "Utility.h"
#include "imgui.h"
#include "Input.h"
#include "Sprite.h"


// Dapper Namespace!
namespace Dapper {
	SplashFadeBehavior::SplashFadeBehavior(int ID_, Engine& engine_)
		: my_ID(ID_)
		, engine(engine_)
	{
	}
	SplashFadeBehavior::~SplashFadeBehavior()
	{
	}

	void SplashFadeBehavior::Init()
	{
		//startTime = 0;
		//fadeInTime = 4;
		//stayTime = 4;
		//fadeOutTime = 4;
		transitionTime = 18; //Time till main menu
	}

	void SplashFadeBehavior::Update(float dt)
	{
		currentTime += dt;
		
		Sprite& fade_sprite = *GetComponentFromParent<Sprite>(engine, my_ID, "Sprite");
		if (currentTime < startTime)
		{
			fade_sprite.SetTint(glm::vec4(0.0, 0.0, 0.0, 0.0));//Dont Show Image
		}
		else if (currentTime > startTime && currentTime < fadeInTime + startTime)
		{
			float howLong = (currentTime - startTime) / fadeInTime;
			fade_sprite.SetTint(glm::vec4(1.0, 1.0, 1.0, howLong));//Fade In
		}
		else if (currentTime > startTime + fadeInTime 
			     && currentTime < startTime + fadeInTime + stayTime)
		{
			// Leave it on screen for Stay Time
		}
		else if (currentTime > startTime + fadeInTime + stayTime &&
			currentTime < fadeOutTime + startTime + fadeInTime + stayTime)
		{
			float howLong = (currentTime - startTime - fadeInTime - stayTime) / fadeOutTime;
			fade_sprite.SetTint(glm::vec4(1.0, 1.0, 1.0, 1 - howLong));//Fade Out 1- Reverses it
		}
		else
		{
			fade_sprite.SetTint(glm::vec4(0.0, 0.0, 0.0, 0.0));//Dont Show Image
		}

		if (currentTime > transitionTime)
		{
			SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
			scene_man.UnloadAllScenes();
			scene_man.LoadScene("MainMenu");
		}

		Input& input = *dynamic_cast<Input*>(engine.Get("Input"));
		const std::vector<keyboard_key> keys = input.GetAllKeys();

		// Skip SplashSreen
		if (keys[GLFW_KEY_SPACE] == keyboard_key::KEY_DOWN)
		{
			SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
			scene_man.UnloadAllScenes();
			scene_man.LoadScene("MainMenu");
		}
	}

	void SplashFadeBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
		deserial;
		auto daObject = jsonVal.GetObject();
		startTime  = daObject["StartTime"].GetFloat();
		fadeInTime = daObject["FadeInTime"].GetFloat();
		stayTime  = daObject["StayTime"].GetFloat();
		fadeOutTime = daObject["FadeOutTime"].GetFloat();
	}

	void SplashFadeBehavior::Write(IJsonWriter& writer) const
	{
		writer.WriteKey("SplashFadeBehavior");
		writer.StartObject();
		writer.WriteFloat("StartTime", startTime);
		writer.WriteFloat("FadeInTime", fadeInTime);
		writer.WriteFloat("StayTime", stayTime);
		writer.WriteFloat("FadeOutTime", fadeOutTime);
		writer.EndObject();
	}

	void SplashFadeBehavior::ImGuiDisplay(float scale)
	{
		// Display editable values as float inputs.
		scale;
	}

} // End Of Dapper Namespace