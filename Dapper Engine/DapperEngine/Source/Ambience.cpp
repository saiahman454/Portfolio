//------------------------------------------------------------------------------
//
// File Name:	Ambience.cpp
// Author(s):	Gavin Lu
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Ambience.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Utility.h"
#include "SceneManager.h"
#include <iostream>
#include "Input.h"

// Dapper Namespace!
namespace Dapper {
  Ambience::Ambience(Engine& engine_)
    : engine(engine_),
      paused(false)
  {
  }

  Ambience::~Ambience()
  {
    ambience->Stop();
  }


  void Ambience::Initialize()
  {

    const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");

    ambience = audio.GetSound(fileName);

    ambience->SetVolume(volume);

    ambience->Play();

    /*
    SceneManager& scene_man = *GetSystemFromEngine< SceneManager*>(*engine, "SceneManager");
    std::function<void(void)> func = std::bind(&BGMBehavior::Stop, this);
    scene_man.RegisterOnChange(func);
    */

  }

  void Ambience::Stop()
  {

    ambience->Stop();

  }

  void Ambience::Render()
  {

    SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");

    std::string current_scene = "NULL";

    if (scene_man.IsSceneLoaded("Level1"))
    {
      current_scene = "Level1";
    }
    else if (scene_man.IsSceneLoaded("Level2"))
    {
      current_scene = "Level2";
    }
    else if (scene_man.IsSceneLoaded("Level3"))
    {
      current_scene = "Level3";
    }
    if (current_scene != "NULL") {

      if (scene_man.GetScene(current_scene).IsPaused()) {

        ambience->SetParameter("paused", 1);

      }
      else {

        ambience->SetParameter("paused", 0);

      }

    }


  }

  void Ambience::AmbienceSetParameter(std::string_view scene)
  {

    SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");

    if (scene == "Level1")
    {
      ambience->SetParameter("gameState", 1);
    }
    else if (scene == "Level2")
    {
      ambience->SetParameter("gameState", 2);
    }
    else if (scene == "Level3")
    {
      ambience->SetParameter("gameState", 3);
    }
    else if (scene == "MainMenu")
    {
      ambience->SetParameter("gameState", 0);
    }
    else if (scene == "Lose")
    {
      ambience->SetParameter("gameState", 5);
    }
    else if (scene == "LevelPassed1")
    {
      ambience->SetParameter("gameState", 4);
    }
    else if (scene == "LevelPassed2")
    {
      ambience->SetParameter("gameState", 4);
    }
    else if (scene == "WinGame")
    {
      ambience->SetParameter("gameState", 4);
    }

  }

  bool Ambience::UpdateWhilePaused() const
  {
    return true;
  }

  void Ambience::SetPaused(int paused)
  {

    ambience->SetParameter("paused", paused);

  }

  const std::string& Ambience::Name() const
  {
    return name;
  }

  void Ambience::Update(float dt)
  {

    dt;
    

  }

} // End Of Dapper Namespace