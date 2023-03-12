//------------------------------------------------------------------------------
//
// File Name:	BehaviorManager.h
// Author(s):	Gavin Lu
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
#include "ITypeManager.h"
#include "vector"
#include "IComponent.h"
#include "Engine.h"
#include <glm/gtx/transform.hpp>
#include "rapidjson/document.h"
#include "IDManager.h"
#include "Messenger.h"

//------------------------------------------------------------------------------


// Dapper Namespace!
namespace Dapper
{

  class BehaviorManager : public ITypeManager<IComponent*>
  {
  public:
    BehaviorManager(Engine& engine_new);
    ~BehaviorManager()override;

    const std::string& Name()const override;

    void Initialize()override;

    void Update(float dt) override;

    void Render() override;

    int GetNew() override;

    void Destroy(int ID);

    int Add(IComponent* obj);

    IComponent*& operator[] (int index)override;

    inline int GetNumCollectibles() const { return num_collectibles; }
    inline void SetNumCollectibles(int _num_collectibles) { num_collectibles = _num_collectibles; }
    void IncrementNumCollectibles();
    void DecrementNumCollectibles();

    int RegisterCreateMessage(std::function<void()> function);
    int RegisterDestroyMessage(std::function<void()> function);

    void unRegisterCreateMessage(int theID);
    void unRegisterDestroyMessage(int theID);
  private:

    const static inline std::string name = "BehaviorManager";

    std::vector<IComponent*> behavior;
    Messenger<> on_collectible_create{};
    Messenger<> on_collectible_destroy{};

    Engine& my_engine;
    IDManager ID_manager;

    int num_collectibles = 0;

    // Inherited via ITypeManager
    virtual bool UpdateWhilePaused() const override;

  }; //End of BehaviorManager class

} // End Of Dapper Namespace