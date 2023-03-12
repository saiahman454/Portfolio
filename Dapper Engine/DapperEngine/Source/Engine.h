//------------------------------------------------------------------------------
//
// File Name:	Engine.h
// Author(s):	Isaiah Dickison
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
#include <vector>
#include <unordered_map>
#include <string_view>

#include "ISystem.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class Engine {
  public:
    Engine();
    ~Engine();

    void Add(ISystem* system);

    void Init();

    void Update(float dt);

    void Render();

    bool IsRunning();

    void Stop();

    ISystem* Get(const std::string_view& name);
    
    float GetUpdatedt();

    float& GetdtReference();
    float& GetRenderdtReference();

    void Setdt(float dt);

    void operator=(const Engine&) = delete;
    Engine(const Engine&) = delete;

    void ToggleGamePaused();
    bool IsGamePaused()const;
  private:
    void DestroySystems();
    std::unordered_map<std::string_view, ISystem*> SystemMap;
    std::vector<ISystem*> Systems;
    bool is_running = false;
    float delta_time = 0;
    float render_time = 0;
    bool game_paused = false;

  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


