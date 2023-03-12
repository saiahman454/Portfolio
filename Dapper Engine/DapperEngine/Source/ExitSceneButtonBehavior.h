//------------------------------------------------------------------------------
//
// File Name:	LoadSceneButtonBehavior.h
// Author(s):	Natalie Bouley
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Button.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class ExitSceneButtonBehavior : public Button
  {
  public:

      ExitSceneButtonBehavior(int ID_, Engine& engine_);

    ~ExitSceneButtonBehavior() override;

    void Init() override;

    const std::string& GetName() const override;

    void Update(float dt) override;

    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;

    void Write(IJsonWriter& writer) const override;

    void ImGuiDisplay(float scale)override;

    inline void SetPause(bool Condition)override { paused = Condition; }
    inline bool GetPause() const override { return paused; }

    void OnSceneExit() const;

  private:

    static inline const std::string name = "ExitSceneButtonBehavior";

    Engine& engine; // The engine

    void ChangeGameState() const;
    std::string onNone;
    std::string onHovered;
    std::string onClicked;
    bool paused = false;
  };
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


