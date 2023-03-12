//------------------------------------------------------------------------------
//
// File Name:	NavButtonBehavior.h
// Author(s):	Adam Tackett
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

  class NavButtonBehavior : public Button
  {
  public:

     NavButtonBehavior(int ID_, Engine& engine_);

    ~NavButtonBehavior() override;

    void Init() override;

    const std::string& GetName() const override;

    void Update(float dt) override;

    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;

    void Write(IJsonWriter& writer) const override;

    void ImGuiDisplay(float scale)override;

    inline void SetPause(bool Condition)override { paused = Condition; }
    inline bool GetPause() const override { return paused; }

  private:

    static inline const std::string name = "NavButtonBehavior";

    Engine& engine; // The engine

    std::string gameState = "";
    void ChangeGameState() const;
    std::string onNone;
    std::string onHovered;
    std::string onClicked;
    bool paused = false;
  };
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


