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

  class BGMController : public ISystem
  {
  public:

    BGMController(Engine& engine_);

    ~BGMController() override;

    void Initialize() override;

    const std::string& Name() const override;

    void Update(float dt) override;

    void Stop();

    void Play();

    void Render() override;

    void BGMSetParameter(std::string_view scene);

    void SetPaused(int paused);

    void CollectedAll(int collected);

  private:

    static inline const std::string name = "BGMController";

    Engine& engine; // The engine

    std::string fileName2 = "/Music/BGM_Controller";

    std::shared_ptr<Sound> music;

    float volume = 0.2;
    bool paused = false;

    virtual bool UpdateWhilePaused()const override;

  };
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


