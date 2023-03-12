//------------------------------------------------------------------------------
//
// File Name:	CollectibleBehavior.h
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
#include "Messenger.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class CollectibleBehavior : public IComponent
  {
  public:

    // Constructor and Destructor
    CollectibleBehavior(int ID_, Engine* engine_);
    ~CollectibleBehavior() override;

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

    // Player Collision Resolution
    void ResolutionFunction(IComponent* object);

    inline void SetPause(bool Condition)override { paused = Condition; }
    inline bool GetPause() const override { return paused; }

    inline bool IsCollectable() const { return is_collectable; }
    inline void Collect() { is_collectable = false; }

  private:
    
    static inline const std::string name = "CollectibleBehavior";
    int my_ID; // The ID of the component in the behavior manager
    int parent_ID = -1; // The ID of the parent game object
    Engine* engine; // The engine
    bool paused = false; // Paused bool
    float killTimer = 0.0; //So particles work
    bool is_collectable = true;

    // Future implentation: sounds when wall is destroyed
    std::string fileName = "";
    float volume = 0.5;

  };
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


