//------------------------------------------------------------------------------
//
// File Name:	Engine.cpp
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Engine.h"
#include "ISystem.h"
#include "Time.h"
#include "Trace.h"
// Dapper Namespace!
namespace Dapper {
  /*------------------------------
  Description: Default Constructor
  Prams: None
  -------------------------------*/
  Engine::Engine()
  {
  }
  /*------------------------------
  Description:Default Destructor
  Prams: None
  -------------------------------*/
  Engine::~Engine()
  {
    DestroySystems();
    is_running = false;
  }

  void Engine::Add(ISystem* system)
  {
    Systems.push_back(system);
    std::string const& stroing = system->Name();
    SystemMap.insert({ std::string_view{stroing.c_str()}, system });
  }

  /*------------------------------
  Description:Inititalize the engine
  Prams: None
  Return: Void
  -------------------------------*/
  void Engine::Init()
  {
    // for all Dsystems in Systems loop through and initailize
    std::ofstream& trace = (*dynamic_cast<Trace*>(Get("Trace"))).GetStream();
    for (ISystem* current : Systems) 
    {
      current->Initialize();
      trace << "Init " << current->Name() << std::endl;
    }
    is_running = true;
  }
  /*------------------------------
  Description:Update the engine
  Prams:
    dt - Delta time.
  Return: Void
  -------------------------------*/
  void Engine::Update(float dt)
  {
    // for all Dsystems in Systems loop through and update
    for (ISystem* current : Systems) {
      if (game_paused && current->UpdateWhilePaused())
      {
        current->Update(delta_time);
      }
      else if (game_paused == false)
      {
        current->Update(delta_time);
      }
    }
  }
  /*------------------------------
  Description:Render the engine
  Prams: None
  Return: Void
  -------------------------------*/
  void Engine::Render()
  {
    render_time = 1;
    for (ISystem* current : Systems) {
      if (render_time)
      {
        current->Render();
      }
    }
  }
  /*------------------------------
  Description:Check if the engine is running
  Prams: None
  Return: bool - true or false if running.
  -------------------------------*/
  bool Engine::IsRunning()
  {
    return is_running;
    
  } 
  /*------------------------------
  Description:To stop the engine running
  Prams: None
  Return: Void
  -------------------------------*/
  void Engine::Stop()
  {
    is_running = false;
  }

  ISystem* Engine::Get(const std::string_view& name)
  {
    auto iter = SystemMap.find(name);
    if (iter != SystemMap.end())
    {
      return iter->second;
    }
    return nullptr;
  }

  float Engine::GetUpdatedt()
  {
    return delta_time;
  }

  float& Engine::GetdtReference()
  {
    return delta_time;
  }

  float& Engine::GetRenderdtReference()
  {
    return render_time;
  }

  void Engine::Setdt(float dt)
  {
    delta_time = dt;
  }



  //PRIVATE

  void Engine::ToggleGamePaused()
  {
    if (game_paused == true)
    {
      game_paused = false;
    }
    else
    {
      game_paused = true;
    }
  }

  bool Engine::IsGamePaused()const
  {
    return game_paused;
  }

  /*------------------------------
   Description:
   Helper function for deleting all systems back to front
   Prams: None
   Return: Void
   -------------------------------*/
  void Engine::DestroySystems()
  {
    std::ofstream& trace = (*dynamic_cast<Trace*>(Get("Trace"))).GetStream();
    for (int i = (int)Systems.size() - 1; i >= 0; i--) {
      trace << "Removed " << Systems[i]->Name() << std::endl;
      SystemMap.erase(Systems[i]->Name());//Adam
      delete Systems[i];
      Systems[i] = NULL;//Adam
    }
  }

} // End Of Dapper Namespace