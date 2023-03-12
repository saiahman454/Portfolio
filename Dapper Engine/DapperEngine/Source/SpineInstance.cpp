//------------------------------------------------------------------------------
//
// File Name:	SpineInstance.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "SpineInstance.h"

// Dapper Namespace!
namespace Dapper {
  /*--------------------------------------
  Description: Default Constructor
  Params:
  Return: Void
  ---------------------------------------*/
  SpineInstance::SpineInstance()
    : parent_data(nullptr)
    , skeleton(nullptr)
    , animation_state(nullptr)
    , time_scale(0)
  {}

  /*--------------------------------------
  Description: Constructor with Spine Data
  Params: spine_data- the spine data class to set
  Return: Void
  ---------------------------------------*/
  SpineInstance::SpineInstance(SpineData const* spine_data)
  {
    skeleton = new spine::Skeleton(spine_data->GetSkeletonData());
    animation_state = new spine::AnimationState(spine_data->GetAnimationStateData());
    parent_data = spine_data;
    time_scale = 0;
  }
   
  /*--------------------------------------
  Description: Destructor
  Params:
  Return: Void
  ---------------------------------------*/
  SpineInstance::~SpineInstance()
  {
    delete skeleton;
    delete animation_state;
  }
} // End Of Dapper Namespace