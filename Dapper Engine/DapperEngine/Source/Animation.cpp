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

#include "Animation.h"
#include "AnimationManager.h"
#include "SpriteSource.h"
#include "Utility.h"
#include <iostream>
#include "imgui.h"

// Dapper Namespace!
namespace Dapper {

  /*--------------------------------------
  Description: Default Constructor
  Params:
  Return: Void
  ---------------------------------------*/
  // Constructor
  Animation::Animation(Engine* engine1)
    : frame_index(0)
    , frame_count(0)
    , frame_timer(0.0f)
    , frame_duration(0.0f)
    , is_running(false)
    , is_looping(false)
    , is_done(false)
    , my_ID(-1)
    , parent_ID(-1)
    , engine(engine1)
  {
  }

  /*--------------------------------------
  Description: Constructor with index
  Params: int index - index of the animation.
  Return: Void
  ---------------------------------------*/
  Animation::Animation(int index, Engine* engine1)
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
  Animation::~Animation()
  {
  }

  /*--------------------------------------
  Description: Initializes everything in the animation.
  Params:
  Return: Void
  ---------------------------------------*/
  void Animation::Init()
  {
  }

  /*--------------------------------------
  Description: Gets the parent ID
  Params:
  Return: the parentID.
  ---------------------------------------*/
  int Animation::GetParentID()
  {
    return parent_ID;
  }

  /*--------------------------------------
  Description: Sets the parent ID
  Params: int ID- the ID to set
  Return: void
  ---------------------------------------*/
  void Animation::SetParentID(int ID)
  {
    parent_ID = ID;
  }

  /*--------------------------------------
  Description: Gets the name "Animation"
  Params:
  Return: the name.
  ---------------------------------------*/
  const std::string& Animation::GetName()
  {
    return name;
  }

  /*--------------------------------------
  Description: Update function
  Params: float dt - time between frames.
  Return: Void
  ---------------------------------------*/
  void Animation::Update(float dt)
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

        SpriteSource& spritesource = *GetComponentFromParent<SpriteSource*>(*engine, parent_ID, "SpriteSource");
        spritesource.SetUVCoordinates(frame_index);
        //std::cout << "Fuck you visual studio\n";
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
  void Animation::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
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
  void Animation::Write(rapidjson::Value& jsonVal)
  {
    jsonVal;
  }

  /*--------------------------------------
  Description: Renders an animation.
  Params: Engine& engine - the engine.
  Return: Void
  ---------------------------------------*/
  void Animation::Render()
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
  void Animation::Play()
  {
    frame_timer = frame_duration;
    is_running = true;
    is_done = false;
  }

  /*--------------------------------------
  Description: Gets the status on if the animation is done.
  Params: none
  Return: true if the animation is done, false if not
  ---------------------------------------*/
  bool Animation::IsDone() const
  {
    return is_done;
  }

  /*--------------------------------------
  Description: Gets the animation's ID.
  Params: none
  Return: the animation's ID
  ---------------------------------------*/
  unsigned Animation::GetID() const
  {
    return my_ID;
  }

  /*--------------------------------------
  Description: Gets the animation game object parent's ID.
  Params: none
  Return: the animation game object parent's ID
  ---------------------------------------*/
  unsigned Animation::GetParentID() const
  {
    return parent_ID;
  }

  /*--------------------------------------
  Description: Sets the animation's ID.
  Params: unsigned newID- the new ID of the animation.
  Return: void
  ---------------------------------------*/
  void Animation::SetID(int newID)
  {
    my_ID = newID;
  }
  void Animation::SetFrame(unsigned new_frame)
  {
    // Hi Gavin here, I assumed you meant frame_index not current_frame?
    frame_index = new_frame;
  }
  void Animation::ImGuiDisplay()
  {
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