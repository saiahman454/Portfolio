//------------------------------------------------------------------------------
//
// File Name:	JellyfishBehavior.h
// Author(s):	Natalie Bouley
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

#include "IStateMachine.h"
#include "Input.h"
#include "Physics.h"
#include "Transform.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class JellyfishBehavior : public IComponent
  {
  public:

    // Constructor and Destructor
    JellyfishBehavior(int ID_, Engine* engine_);
    ~JellyfishBehavior() override;

    // Get and Set Functions
    inline const int GetID() const override { return my_ID; }
    inline int GetParentID() const override { return parent_ID; }
    inline void SetParentID(int ID)override { parent_ID = ID; }
    inline const std::string& GetName() const override { return name; }
    inline void SetID(int ID)override { my_ID = ID; }

    // Update function
    void Update(float dt) override;

    // Read function inherited from IComponent
    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;

    // Write function inherited from IComponent
    void Write(IJsonWriter& writer) const override;

    // Display function for ImGui
    void ImGuiDisplay(float scale)override;

    // Player Collision Resolution
    void ResolutionFunction(IComponent* object);

    // Init function
    void Init() override;

    // Set and find the next pathpoint of the jellyfish
    void FindNextPathpoint(glm::vec2 translation);

    inline void SetPause(bool Condition)override { paused = Condition; }
    inline bool GetPause() const override { return paused; }

  private:

    static inline const std::string name = "JellyfishBehavior";
    int my_ID; // The ID of the component in the behavior manager
    int parent_ID = -1; // The ID of the parent game object
    Engine* engine; // The engine
    float speed = 0.0f; // The speed of the jellyfish
    glm::vec2 pathpoint0 = { 0, 0 }; // The first point in the path of the jellyfish
    glm::vec2 pathpoint1 = { 0, 0 }; // The second point in the path of the jellyfish
    int next_pathpoint = 0;
    float detect_radius = 20.0f;
    std::string fileName = "";
    float volume = 0.5;
    bool paused = false;
  };
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


