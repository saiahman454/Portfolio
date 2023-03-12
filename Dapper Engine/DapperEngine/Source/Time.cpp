//------------------------------------------------------------------------------
//
// File Name:	Time.cpp
// Author(s):	Isaiah Dickison
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Time.h"
#include "Input.h"
#include <iostream>

// Dapper Namespace!
namespace Dapper
{
  /*--------------------------------------
  Description: Time Constructor
  Params: engine_dt - delta time
  engine_r_dt - render delta time
  ---------------------------------------*/
  Time::Time(float& engine_dt, float& engine_r_dt) :
    update_rate(1 / 120.0f),
    frame_rate(1 / 120.0f),
    update_time(engine_dt),
    render_time(engine_r_dt)
  {
  }

  /*--------------------------------------
  Description: Time destructor
  ---------------------------------------*/
  Time::~Time()
  {
  }

  /*--------------------------------------
  Description: Initialize function
  ---------------------------------------*/
  void Time::Initialize()
  {
    prev_time = std::chrono::steady_clock::now();
  }

  /*--------------------------------------
  Description: calculates dt
  Params: dt - delta time
  ---------------------------------------*/
  void Time::Update(float dt)
  {
    dt;
    auto curr_time = std::chrono::steady_clock::now();
    double time = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(curr_time - prev_time).count();
 
    if (time >= update_rate)
    {
      update_time = (float)time;
      prev_time = curr_time;
    }
    else
    {
      update_time = 0;
    }




  }

  /*--------------------------------------
  Description: calculates render dt
  ---------------------------------------*/
  void Time::Render()
  {
    auto curr_time = std::chrono::steady_clock::now();
    double time = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(curr_time - prev_render_time).count();

    if (time >= frame_rate)
    {
      CalcFPS(curr_time);
      render_time = (float)time;
      prev_render_time = curr_time;
    }
    else
    {
      render_time = 0;
    }

  }

  /*--------------------------------------
  Description: puases the game
  ---------------------------------------*/
  void Time::PauseToggle()
  {
    if (paused == false)
    {
      paused = true;
    }
    else
    {
      paused = false;
    }
  }

  /*--------------------------------------
  Description: calculates the fps of the engine
  Params: curr_time - the current time
  ---------------------------------------*/
  void Time::CalcFPS(std::chrono::steady_clock::time_point curr_time)
  {
    ++total_frames;
    double time = std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>>(curr_time - start_time).count();
    if (time > 0.5)
    {
      fps = total_frames / (float)time;
      start_time = curr_time;
      total_frames = 0;
      //print to console
      //std::cout << fps << std::endl; 
    }
  }

} // End Of Dapper Namespace