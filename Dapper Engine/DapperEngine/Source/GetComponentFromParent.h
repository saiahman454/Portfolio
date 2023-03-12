//------------------------------------------------------------------------------
//
// File Name:	Stub.h
// Author(s):	
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Engine.h"
#include "ComponentRegistry.h"
#include "GameObjectManager.h"
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  template<typename T>
  T GetComponentFromParent(Engine& engine, int parentID, std::string comp_name)
  {
    ComponentRegistry* reg = dynamic_cast<ComponentRegistry*>(engine.Get("ComponentRegistry"));
    GameObjectManager* the_man = dynamic_cast<GameObjectManager*>(engine.Get("GameObjectManager"));
    GameObject& obj = (*the_man)[parentID];
    int newcompID = obj.GetComponentID(comp_name);
    return dynamic_cast<T>(reg->GetComponent(comp_name, newcompID));
  }
  
  template<typename T>
  T GetSystemFromEngine(Engine& engine, std::string system_name)
  {
      T systemToGet = dynamic_cast<T>(engine.Get(system_name));
      return systemToGet;
  }

} // End Of Dapper Namespace

//------------------------------------------------------------------------------


