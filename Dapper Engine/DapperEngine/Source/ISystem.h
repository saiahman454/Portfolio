//------------------------------------------------------------------------------
//
// File Name:	Isystem.h
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

//------------------------------------------------------------------------------
#include <string>
// Dapper Namespace!
namespace Dapper {
  class ISystem
  {
  public:
    //all systems need a deconstructor in their own files
    virtual ~ISystem() = 0 {};
    //names for debuging 
    virtual const std::string& Name()const = 0;
    //Initalize systems
    virtual void Initialize() = 0;
    //Update function for game loop
    virtual void Update(float dt) = 0;
    //render teh system in the game loop
    virtual void Render() = 0;
    //return a bool if the system need to update while paused
    virtual bool UpdateWhilePaused()const = 0;
  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


