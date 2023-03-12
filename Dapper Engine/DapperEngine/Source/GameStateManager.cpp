//------------------------------------------------------------------------------
//
// File Name:	GameStateManager.cpp
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "GameStateManager.h"
#include <fstream>
#include "Deserializer.h"
#include "Utility.h"
#include "Input.h"

// Dapper Namespace!
namespace Dapper {
  /*--------------------------------------
  Description: Constructor for a GameStateManager.
  Param:
  engine - the parent engine.
  filename - starting file to load
  ---------------------------------------*/
  GameStateManager::GameStateManager(Engine& my_engine, const std::string& filename) :current(filename), my_engine(my_engine)
  {
  }

  /*--------------------------------------
  Description: Destructor for a GameStateManager
  ---------------------------------------*/
  GameStateManager::~GameStateManager()
  {
  }

  /*--------------------------------------
  Description: Initialize function
  ---------------------------------------*/
  void GameStateManager::Initialize()
  {
    is_changing = true;
  }

  /*--------------------------------------
  Description: update function
  Param:
  dt - delta time.
  ---------------------------------------*/
  void GameStateManager::Update(float dt)
  {
    dt;
    if (is_changing)
    {
      GameObjectManager& obj_man = *GetSystemFromEngine<GameObjectManager*>(my_engine, "GameObjectManager");
      obj_man.DeleteAllGameObjects();
      LoadGameState(current);
    }
  }

  /*--------------------------------------
  Description: Render function
  ---------------------------------------*/
  void GameStateManager::Render()
  {
    //leave alone plz <3
  }

  /*--------------------------------------
  Description: changes the gamestate from a file
  for this file only
  Params:
  filename - the gamestate you are going to
  ---------------------------------------*/
  void GameStateManager::ChangeGameState(const std::string& filename)
  {
    std::string pfile = "./Data/GameStates/" + filename + ".json";
    if (pfile != current)
    {
      is_changing = true;
      current = pfile;
    }
  }

  /*--------------------------------------
  Description: loads a new gamestate
  for other files
  Params:
  filename - the gamestate you are going to
  ---------------------------------------*/
  void GameStateManager::LoadGameState(const std::string& filename)
  {
    std::ifstream pfile(filename);
    if (pfile)
    {
      Deserializer& deserial = *dynamic_cast<Deserializer*>(my_engine.Get("Deserializer"));
      deserial.DeserializeFile(filename);
      //&&& write this after json training load all objects
    }
    else
    {
      current.clear();
    }
    is_changing = false;

  }


} // End Of Dapper Namespace