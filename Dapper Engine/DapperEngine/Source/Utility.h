//------------------------------------------------------------------------------
//
// File Name:	Utility.h
// Author(s):	Isaiah Dickison
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
  T *GetComponentFromParent(Engine& engine, int parentID, const std::string_view& comp_name)
  {
    ComponentRegistry* reg = dynamic_cast<ComponentRegistry*>(engine.Get("ComponentRegistry"));
    GameObjectManager* the_man = dynamic_cast<GameObjectManager*>(engine.Get("GameObjectManager"));
    GameObject& obj = (*the_man)[parentID];
    int newcompID = obj.GetComponentID(comp_name);
    return static_cast<T*>(reg->GetComponent(comp_name, newcompID));
  }
  
  template<typename T>
  T GetSystemFromEngine(Engine& engine, const std::string_view& system_name)
  {
      T systemToGet = dynamic_cast<T>(engine.Get(system_name));
      return systemToGet;
  }



  template<typename T>
  void DestroyManagedType(std::unordered_map<unsigned int, unsigned int>& address_to_index,
    std::unordered_map<unsigned int, unsigned int>& index_to_address,
    std::vector<T>& vector, unsigned int ID)
  {
    // get the index of the item to delete
    unsigned int index = address_to_index[ID];
    // get the address of the back item for later use
    unsigned int back_address = index_to_address[vector.size() - 1];
    // remove item from the vector by copying over it with the back item
    vector[index] = vector.back();
    // pop the back item
    vector.pop_back();
    // update index_to_address using the index we just removed, and the previously acquired address
    index_to_address[index] = back_address;
    // update address_to_index using the address previously acquired and the new index
    address_to_index[back_address] = index;

    index_to_address.erase(vector.size());
    address_to_index.erase(ID);
  }

  float RandomRange(float min, float max);


} // End Of Dapper Namespace

//------------------------------------------------------------------------------


