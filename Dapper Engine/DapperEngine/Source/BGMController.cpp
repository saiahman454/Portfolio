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

#include "BGMController.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Utility.h"
#include "SceneManager.h"
#include <iostream>
#include "Input.h"

// Dapper Namespace!
namespace Dapper {
  BGMController::BGMController(Engine& engine_)
    : engine(engine_),
      paused(false)
  {
  }

  BGMController::~BGMController()
  {
    music->Stop();
  }


  void BGMController::Initialize()
  {

    const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");

    music = audio.GetSound(fileName2);

    music->SetVolume(volume);

    music->Play();

    /*
    SceneManager& scene_man = *GetSystemFromEngine< SceneManager*>(*engine, "SceneManager");
    std::function<void(void)> func = std::bind(&BGMBehavior::Stop, this);
    scene_man.RegisterOnChange(func);
    */

  }

  void BGMController::Play()
  {

    music->Play();

  }

  void BGMController::Stop()
  {

    music->Stop();

  }

  void BGMController::Render()
  {

    music->SetParameter("paused", paused);

  }

  void BGMController::BGMSetParameter(std::string_view scene)
  {

    SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");

    if (scene == "Level1")
    {
      music->SetParameter("gameState", 1);
    }
    else if (scene == "Level2")
    {
      music->SetParameter("gameState", 2);
    }
    else if (scene == "Level3")
    {
      music->SetParameter("gameState", 3);
    }
    else if (scene == "MainMenu")
    {
      music->SetParameter("gameState", 0);
    }
    else if (scene == "Lose")
    {
      music->SetParameter("gameState", 5);
    }
    else if (scene == "LevelPassed1")
    {
      music->SetParameter("gameState", 4);
    }
    else if (scene == "LevelPassed2")
    {
      music->SetParameter("gameState", 4);
    }
    else if (scene == "WinGame")
    {
      music->SetParameter("gameState", 4);
    }
    else if (scene == "Credits")
    {
      music->SetParameter("gameState", 6);
    }
    else if (scene == "CreditsPause")
    {
      music->SetParameter("gameState", 6);
    }

  }

  void BGMController::SetPaused(int paused)
  {

    this->paused = paused;

  }

  void BGMController::CollectedAll(int collected)
  {

    music->SetParameter("allCollectables", collected);

  }

  bool BGMController::UpdateWhilePaused() const
  {
    return true;
  }

  const std::string& BGMController::Name() const
  {
    return name;
  }

  void BGMController::Update(float dt)
  {

    dt;

  }

} // End Of Dapper Namespace