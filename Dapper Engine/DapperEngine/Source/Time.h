//------------------------------------------------------------------------------
//
// File Name:	Time.h
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
#include "ISystem.h"
#include <chrono>

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
    class Time : public ISystem
    {
    public:
        Time(float& engine_dt, float& engine_r_dt);
        ~Time()override;

        inline const std::string & Name()const override{ return name; };

        void Initialize()override;

        void Update(float dt) override;

        void Render() override;

        inline float GetUpdatedt(){ return update_time; };

        void PauseToggle();

        inline const bool GetPaused()const{ return paused; };

        inline const float GetFPS()const{ return fps; };

        inline const float GetRenderdt()const { return render_time; };

    private:
        inline virtual bool UpdateWhilePaused()const override{ return true; };

        void CalcFPS(std::chrono::steady_clock::time_point curr_time);

        const std::string name = "Time";
        std::chrono::steady_clock::time_point prev_time;
        std::chrono::steady_clock::time_point start_time;
        std::chrono::steady_clock::time_point prev_render_time;
        float update_rate;
        float frame_rate;
        float& update_time;
        float& render_time;
        float fps = 0;
        unsigned int total_frames = 0;
        bool paused = false;
    };//End of Time class

} // End Of Dapper Namespace