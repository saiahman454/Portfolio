//------------------------------------------------------------------------------
//
// File Name:	BehaviorManager.cpp
// Author(s):	Gavin Lu
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "BehaviorManager.h"
#include "Deserializer.h"
#include "ComponentRegistry.h"
#include "BehaviorDeserializer.h"

// Dapper Namespace!
namespace Dapper
{
  BehaviorManager::BehaviorManager(Engine& engine_new) : my_engine(engine_new)
  {
  }

  BehaviorManager::~BehaviorManager()
  {
  }

  const std::string& BehaviorManager::Name() const
  {
    return name;
  }

  void BehaviorManager::Initialize()
  {
    RegisterAllBehaviors(my_engine);
  }

  void BehaviorManager::Update(float dt)
  {
    for (unsigned i = 0; i < behavior.size(); i++)
    {
      bool test = behavior[i]->GetPause();
        if (test == false)
        {
            (behavior[i])->Update(dt);
        }
    }
  }

  void BehaviorManager::Render()
  {
  }

  int BehaviorManager::GetNew()
  {

    int obj_ID = ID_manager.Create<IComponent*>(behavior, nullptr);

    return obj_ID;
    
  }

  void BehaviorManager::Destroy(int ID)
  {

    delete behavior[ID_manager[ID]];
    ID_manager.Destroy(behavior, ID);
  }

  int BehaviorManager::Add(IComponent* obj)
  {

    int id = (int)behavior.size();
    behavior.push_back(obj);
    return id;

  }

  IComponent*& BehaviorManager::operator[](int index)
  {

    return behavior[ID_manager[index]];

  }

  void BehaviorManager::IncrementNumCollectibles()
  {
      num_collectibles++;
      on_collectible_create.SendMessage();
  }

  void BehaviorManager::DecrementNumCollectibles()
  {
      num_collectibles--;
      on_collectible_destroy.SendMessage();
  }

  int BehaviorManager::RegisterCreateMessage(std::function<void()> function)
  {
      return on_collectible_create.RegisterMessage(function);
  }

  int BehaviorManager::RegisterDestroyMessage(std::function<void()> function)
  {
      return on_collectible_destroy.RegisterMessage(function);
  }

  void BehaviorManager::unRegisterCreateMessage(int theID)
  {
      on_collectible_create.Remove(theID);
  }

  void BehaviorManager::unRegisterDestroyMessage(int theID)
  {
      on_collectible_destroy.Remove(theID);
  }

  bool BehaviorManager::UpdateWhilePaused() const
  {
      return false;
  }
  

} // End Of Dapper Namespace