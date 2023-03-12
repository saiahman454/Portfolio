//------------------------------------------------------------------------------
//
// File Name:	Animation.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "AnimationSprite.h"
#include "AnimationManager.h"
#include "SpriteSource.h"
#include "Utility.h"
#include <iostream>
#include "imgui.h"

// Dapper Namespace!
namespace Dapper {
  /*--------------------------------------
  Description: Constructor with index
  Params: int index - index of the animation.
  Return: Void
  ---------------------------------------*/

  AnimationSprite::AnimationSprite(int index, Engine* engine1)
    : frame_index(0)
    , frame_count(0)
    , frame_timer(0.0f)
    , frame_duration(0.0f)
    , is_running(false)
    , is_looping(false)
    , is_done(false)
    , my_ID(index)
    , parent_ID(-1)
    , engine(engine1)
  {
  }

  /*--------------------------------------
  Description: Destructor
  Params:
  Return: Void
  ---------------------------------------*/

  AnimationSprite::~AnimationSprite()
  {
  }

  /*--------------------------------------
  Description: Initializes everything in the animation.
  Params:
  Return: Void
  ---------------------------------------*/

  void AnimationSprite::Init()
  {
      // So it doesn't display the sprite sheet on first frame after creation
      SpriteSource& spritesource = *GetComponentFromParent<SpriteSource>(*engine, parent_ID, "SpriteSource");
      spritesource.SetUVCoordinates(frame_index);
  }


  /*--------------------------------------
  Description: Update function
  Params: float dt - time between frames.
  Return: Void
  ---------------------------------------*/

  void AnimationSprite::Update(float dt)
  {
    // Check if the animation is running.
    if (is_running)
    {
      is_done = false;
      frame_timer -= dt;
      // If the frame timer reaches zero, update the frame conditions.
      if (frame_timer <= 0)
      {
        frame_timer = frame_duration;
        frame_index++;
        // If the animation reaches the end of the sprite sheet:
        if (frame_index >= frame_count)
        {
          // If the animation loops:
          if (is_looping)
          {
            // Loop the animation.
            frame_index = 0;
            is_done = true;
          }
          // If the animation does not loop:
          else
          {
            // End the animation.
            is_running = false;
            is_done = true;
          }
        }

        SpriteSource& spritesource = *GetComponentFromParent<SpriteSource>(*engine, parent_ID, "SpriteSource");
        spritesource.SetUVCoordinates(frame_index);
      }
    }
  }

  /*--------------------------------------
  Description: Reads the animation data from a json file.
  Params:
    Deserializer& deserializer - The deserializer.
    const Value& jsonVal - The json file to be read from.
  Return: Void
  ---------------------------------------*/

  void AnimationSprite::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    auto object = jsonVal.GetObject();

    // Read all of the necessary private variables in the class.
    frame_index = object["CurrentFrame"].GetInt();
    frame_count = object["FrameCount"].GetInt();
    frame_duration = object["FrameDuration"].GetFloat();
    is_looping = object["IsLooping"].GetBool();
    deserial;

    Play();
  }

  /*--------------------------------------
  Description: Writes data to a json file.
  Params: const Value& jsonVal - The json file to be written from.
  Return: Void
  ---------------------------------------*/

  void AnimationSprite::Write(IJsonWriter& writer) const
  {
    writer;
    writer.WriteKey("Animation");
    writer.StartObject();
    writer.WriteString("Type", "Sprite");
    // Will need to change
    // This will write the frame currently displayed in the engine at write time
    // Not the frame we really want to start with- should have separate variable
    writer.WriteInt("CurrentFrame", frame_index);
    writer.WriteInt("FrameCount", frame_count);
    writer.WriteFloat("FrameDuration", frame_duration);
    writer.WriteBool("IsLooping", is_looping);
    writer.EndObject();
  }

  /*--------------------------------------
  Description: Renders an animation.
  Params: Engine& engine - the engine.
  Return: Void
  ---------------------------------------*/

  void AnimationSprite::Render()
  {
  }

  /*--------------------------------------
  Description: Plays a simple animation.
  Params:
    int frame_count1 - the frame count
    float frame_duration1 - the duration of the frame
    bool is_looping1 - if the animation is looping
  Return: Void
  ---------------------------------------*/

  void AnimationSprite::Play()
  {
    frame_timer = frame_duration;
    is_running = true;
    is_done = false;
  }

  void AnimationSprite::ImGuiDisplay(float scale)
  {
    scale;
    int framecount = (int)frame_count;
    if (ImGui::InputInt("Frame Count", &framecount, 1))
    {
      if (framecount > 0)
      {
        frame_count = framecount;
      }
    }
    ImGui::InputFloat("FrameTime", &frame_duration, 0.05f);
    if (ImGui::Button("is Looping?"))
    {
      if (is_looping == false)
      {
        frame_index = 0;
        is_looping = true;
        is_running = true;
      }
      else
      {
        frame_index = 0;
        is_looping = false;
        is_running = true;
      }
    }
    if (is_looping)
    {
      ImGui::SameLine();
      ImGui::Text("True");
    }
    else
    {
      ImGui::SameLine();
      ImGui::Text("False");
    }
    if (ImGui::Button("Start Animation"))
    {
      is_running = true;
    }
  }
} // End Of Dapper Namespace