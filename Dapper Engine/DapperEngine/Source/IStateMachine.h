//------------------------------------------------------------------------------
//
// File Name:	IStateMachine.h
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

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  // templates can't link with the cpp file, so implementation has to be here ;-;

  template<typename EnumType>

  class IStateMachine : public IComponent
  {

  public:

    IStateMachine(EnumType invalid, EnumType initial) :
      curr_state(invalid), next_state(initial) {}

    virtual ~IStateMachine() = 0 {};

    void Update(float dt) override
    {
      if (curr_state != next_state)
      {
        OnExit();
        curr_state = next_state;
        OnInit();
      }
      OnUpdate(dt);
    }

    virtual void OnExit() = 0;

    virtual void OnInit() = 0;

    virtual void OnUpdate(float dt) = 0;
    void SetPause(bool Condition)override { paused = Condition; }
    bool GetPause() const override { return paused; }

    inline EnumType GetState()
    {
      return curr_state;
    }

    inline void SetNextState(EnumType nextState)
    {
      next_state = nextState;
    }

    virtual bool UpdateWhilePaused()const = 0;

  private:

    EnumType curr_state;
    EnumType next_state;
    bool paused = false;
  };
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


