//------------------------------------------------------------------------------
//
// File Name:	FullScreenBehavior.h
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "IStateMachine.h"
#include "Input.h"
#include "Physics.h"
#include "Transform.h"
#include "Audio.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class FullScreenBehavior : public IComponent
  {
  public:

    // Constructor and Destructor
	  FullScreenBehavior(int ID_, Engine& engine_);
    ~FullScreenBehavior() override;

    // Get and Set Functions
    inline const int GetID() const override { return my_ID; }
    inline int GetParentID() const override { return parent_ID; }
    inline void SetParentID(int ID) override { parent_ID = ID; }
    inline const std::string& GetName() const override { return name; }
    inline void SetID(int ID)override { my_ID = ID; }

    // Init function
    void Init() override;

    // Update function
    void Update(float dt) override;

    // Read function inherited from IComponent
    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;

    // Write function inherited from IComponent
    void Write(IJsonWriter& writer) const override;

    // Display function for ImGui
    void ImGuiDisplay(float scale)override;

    inline void SetPause(bool Condition)override { paused = Condition; }
    inline bool GetPause() const override { return paused; }

  private:
    bool MouseOnButton() const;
    void ChangeTexture(std::string input);

    static inline const std::string name = "FullScreenBehavior";
    int my_ID; // The ID of the component in the behavior manager
    int parent_ID = -1; // The ID of the parent game object
    Engine& engine; // The engine

    bool isOn = false;
    std::string buttonOn;
    std::string buttonOff;
    bool paused = false;
  };
  
} // End Of Dapper Namespace