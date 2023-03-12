//------------------------------------------------------------------------------
//
// File Name:	SpineAnimationSprite.h
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
#include "IAnimation.h"
#include "SpineData.h"
#include "SpineInstance.h"
#include <string>
#include "ParticleSystem.h"
#include "IJsonWriter.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class AnimationSpine : public IAnimation
  {
  public:
    // Constructor and Destructor
    AnimationSpine(int ID, Engine* engine_);
    ~AnimationSpine();

    // Get and Set Functions
    inline int GetParentID() const override { return parent_ID; }
    inline Engine* GetEngine() { return engine; }
    inline SpineInstance* GetInstance() { return instance; }
    inline SpineData* GetData() { return data; }
    inline void SetParentID(int ID) override { parent_ID = ID; }
    inline const std::string& GetName() const override { return name; }
    inline const int GetID() const override { return my_ID; }
    inline void SetSpineData(SpineData* data_) { data = data_; }
    inline void SetID(int ID) override { my_ID = ID; }
    inline void SetMeshID(int meshID) { mesh_ID = meshID; }

    // Update function
    void Update(float dt) override;

    // Play the spine animation
    void Play();

    // Initialize the spine animation
    void Init() override;

    // Use this to read the filepath name 
    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;

    // Write function inherited from IComponent
    void Write(IJsonWriter& writer) const override;

    // Display function for ImGui
    void ImGuiDisplay(float scale) override;

    inline void SetPause(bool Condition)override { paused = Condition; }
    inline bool GetPause() const override { return paused; }

  private:

    static inline const std::string name = "AnimationSpine";
    SpineData* data; // All of the data associated with the Spine animation
    SpineInstance* instance; // The actual instance of the Spine animation. Skeleton, AnimationState, etc.
    std::string filename; // The central name of the Spine animation
    bool is_looping; // If the animation is looping
    int mesh_ID; // The ID of the Spine Animation's mesh
    int my_ID; // The ID of the component in its manager
    int parent_ID; // ID of the parent game object
    std::vector<float> vertices; // The vertices to change every frame
    std::vector<unsigned> faces; // The frames to change every frame
    Engine* engine; // Pointer to the engine
    bool paused = false;
  };

} // End Of Dapper Namespace