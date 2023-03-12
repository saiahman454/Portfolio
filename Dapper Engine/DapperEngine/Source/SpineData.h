//------------------------------------------------------------------------------
//
// File Name:	SpineData.h
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
#include <spine/spine.h>
#include <string>
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class AnimationSpine;

  class SpineData
  {
  public:
    // Constructors and Destructors
    SpineData(std::string& filename, AnimationSpine* _parent);
    ~SpineData();

    // Get and Set Functions
    inline spine::Atlas* GetAtlas() const { return atlas; }
    inline spine::SkeletonData* GetSkeletonData() const { return skeleton_data;  }
    inline spine::AnimationStateData* GetAnimationStateData() const{ return animation_state_data; }
    inline spine::Vector<spine::Animation*> GetAnimations() { return animations; }

    // Save the skeleton_data as a skeleton read from a binary file.
    void ReadData(std::string& filename);

  private:
    AnimationSpine* parent;
    spine::Atlas* atlas; // Holds all the information about where things are in the .png (used for texture packing)
    spine::SkeletonData* skeleton_data; // Holds all the data of the bones
    spine::AnimationStateData* animation_state_data; // Holds information about handling different animations
    spine::Vector<spine::Animation*> animations; // All of the animations of the Spine instance
  };

} // End Of Dapper Namespace

//------------------------------------------------------------------------------

