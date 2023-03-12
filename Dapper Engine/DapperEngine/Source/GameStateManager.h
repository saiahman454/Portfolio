//------------------------------------------------------------------------------
//
// File Name:	GameStateManager.h
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
//------------------------------------------------------------------------------
#include "ISystem.h"
#include "Engine.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class GameStateManager : public ISystem {
  public:
    GameStateManager(Engine& my_engine, const std::string& filename);

    ~GameStateManager() override;

    inline const std::string& Name()const override{ return name; };

    void Initialize() override;

    void Update(float dt) override;

    //not needed in GSM
    void Render() override;

    void ChangeGameState(const std::string& filename);

    inline void GameStateReload(){ is_changing = true; };

    inline std::string& GetGameState(){ return current; };

  private:
    std::string current = "";
    bool is_changing = false;
    const std::string name = "GameStateManager";
    Engine& my_engine;

    inline bool IsGameStateChanging(){ return is_changing; };
    void LoadGameState(const std::string& filename);

    // Inherited via ISystem
    inline virtual bool UpdateWhilePaused() const override { return true; };
  };

} // End Of Dapper Namespace

//------------------------------------------------------------------------------


