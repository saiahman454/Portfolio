//------------------------------------------------------------------------------
//
// File Name:	BGMBehavior.h
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

  class BGMBehavior : public IComponent
  {
  public:

    BGMBehavior(int ID_, Engine* engine_);

    ~BGMBehavior() override;

    void Init() override;

    int GetParentID() const override;

    void SetParentID(int ID) override;

    const std::string& GetName() const override;

    void Update(float dt) override;

    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;

    void Write(IJsonWriter& writer) const override;

    void ImGuiDisplay(float scale)override;

    void SetID(int ID)override;

    const int GetID() const override;

    void Stop();

    inline void SetPause(bool Condition)override { paused = Condition; }
    inline bool GetPause() const override { return paused; }

  private:

    static inline const std::string name = "BGMBehavior";

    int my_ID; // The ID of the component in the behavior manager

    int parent_ID = -1; // The ID of the parent game object

    Engine* engine; // The engine

    std::string fileName = "/Music/BGM_Controller";

    std::shared_ptr<Sound> music;

    float volume = 0.5;
    bool paused = false;
  };
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


