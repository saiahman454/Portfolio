//------------------------------------------------------------------------------
//
// File Name:	SkeletonInstance.h
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
#include "SpineData.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class SpineInstance
  {
  public:
    // Constructors and Destructors
    SpineInstance();
    SpineInstance(SpineData const* spine_data);
    ~SpineInstance();

    // Get and Set Functions
    inline spine::Skeleton* GetSkeleton(){ if (skeleton) return skeleton; return nullptr; }
    inline spine::AnimationState* GetAnimationState() { if (animation_state) return animation_state; return nullptr; }
    inline void SetSkeleton(spine::Skeleton* skeleton_) { skeleton = skeleton_; }
    inline void SetAnimationState(spine::AnimationState* state) { animation_state = state; }
    inline void SetSpineData(const SpineData* data) { parent_data = data; }

  private:
    SpineData const* parent_data; // Pointer to its data
    spine::Skeleton* skeleton; // Skeleton: holds all the bones (describe how the mesh deforms)
    spine::AnimationState* animation_state; // AnimationState: holds the current animations and the ones that are queued to play
    float time_scale; // Scale the update speed of the animation

  };


} // End Of Dapper Namespace

//------------------------------------------------------------------------------

