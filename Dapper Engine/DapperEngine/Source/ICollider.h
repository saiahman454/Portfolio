//------------------------------------------------------------------------------
//
// File Name:	ICollider.h
// 
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

#include "IComponent.h"
#include "Transform.h"
#include "Physics.h"
#include "Messenger.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  // templates can't link with the cpp file, so implementation has to be here ;-;

  class ICollider: public IComponent
  {

  public:

    using ResolutionFunc = std::function<void(IComponent*)>;

    ICollider(Engine* eng) :
      engine(eng) {}

    virtual ~ICollider() = 0 {};

    virtual void SetTransformPointer(Transform* _trans)
    {

      trans = _trans;

    }
    
    virtual void SetPhysicsPointer(Physics* _phys) 
    {

      phys = _phys;

    }

    virtual Transform* GetTransformPointer() const
    {

      return trans;

    }

    virtual Physics* GetPhysicsPointer() const
    {

      return phys;

    }

    virtual Engine* GetEngine() const
    {

      return engine;

    }


    void RegisterOnChange(Messenger<IComponent*>::func MessengerFunction)
    {

      collisionResolutions.RegisterMessage(MessengerFunction);

    }

    void ColliderSendMessage(IComponent* comp)
    {

      collisionResolutions.SendMessage(comp);

    }

  private:

    Transform* trans = nullptr;
    Physics* phys = nullptr;
    Engine* engine;

    Messenger<IComponent*> collisionResolutions;

  };

} // End Of Dapper Namespace

//------------------------------------------------------------------------------


