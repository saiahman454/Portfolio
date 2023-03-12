//------------------------------------------------------------------------------
//
// File Name:	IAnimation.h
// 
// Author(s):	Natalie Bouley
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

  class IAnimation: public IComponent
  {

  public:

    virtual ~IAnimation() = 0 {};

    virtual void Update(float dt) override = 0;

    virtual void Play() = 0;

    virtual void Init() = 0;

  private:

  };

} // End Of Dapper Namespace

//------------------------------------------------------------------------------


