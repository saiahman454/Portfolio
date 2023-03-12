//------------------------------------------------------------------------------
//
// File Name:	Ambience.h
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
#include "Audio.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class Ambience : public ISystem
  {
  public:

    Ambience(Engine& engine_);

    ~Ambience() override;

    void Initialize() override;

    const std::string& Name() const override;

    void Update(float dt) override;

    void Stop();

    void Render() override;

    void AmbienceSetParameter(std::string_view scene);

    void SetPaused(int paused);

  private:

    static inline const std::string name = "Ambience";

    Engine& engine; // The engine

    std::string fileName = "/Ambience/Ambience_Controller";
    std::string fileName2 = "/Music/BGM_Controller";

    std::shared_ptr<Sound> music;
    std::shared_ptr<Sound> ambience;

    float volume = 0.3;
    bool paused = false;

    virtual bool UpdateWhilePaused()const override;

  };
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


