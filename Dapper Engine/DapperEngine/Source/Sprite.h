//------------------------------------------------------------------------------
//
// File Name:	Sprite.h
// Author(s):	Natalie Bouley
//						
//						
//
// Copyright 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "Mesh.h"
#include "Texture.h"
#include "MeshManager.h"
#include "IComponent.h"
#include "Engine.h"
#include "SpriteSource.h"
#include <glm/vec4.hpp>

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class Sprite : public IComponent
  {
  public:
    // Constructors and Destructor
    Sprite(Engine* engine_);
    Sprite(int index, Engine* engine_);
    ~Sprite() override;
    Sprite(Sprite&& sprite) noexcept;
    Sprite(const Sprite& sprite) = default;

    Sprite& operator=(Sprite&& sprite) noexcept;
    Sprite& operator=(const Sprite& sprite) = default;

    // Get and Set functions
    inline int GetParentID() const override { return parentID; }
    inline void SetParentID(int ID) { parentID = ID; }
    inline const std::string& GetName() const override { return name; }
    inline int GetMesh() const { return meshID; }
    inline void SetID(int newID)override { my_ID = newID; }
    inline const int GetID() const override { return my_ID; }

    inline void SetPause(bool Condition)override { paused = Condition; }
    inline bool GetPause() const override { return paused; }

    inline glm::vec4 GetTint() const { return tint; }
    inline void SetTint(glm::vec4 new_tint) { tint = new_tint; }
    inline void SetMesh(int meshid) { meshID = meshid; }

    // Update function inherited from IComponent
    void Update(float dt) override;

    // Init function inherited from IComponent
    void Init() override;

    // Read function inherited from IComponent
    void Read(Deserializer& deserializer, const rapidjson::Value& jsonVal) override;

    // Write function inherited from IComponent
    void Write(IJsonWriter& writer) const override;

    // Render function
    void Render(Engine& engine);

    // Update the mesh on reallocation
    void OnMeshReallocation(MeshManager& mesh_manager);

    // Display function for ImGui
    void ImGuiDisplay(float scale)override;

  private:
    void OnSpriteSourceRealloc(SpriteSource* spc)
    {
      spriteSource = spc;
    }

    static const inline std::string name = "Sprite"; // "Sprite"
    int parentID; // The ID of the parent Game Object
    int my_ID; // The ID number of the sprite
    glm::vec4 tint; // The tint of the sprite
    int meshID; // The meshID
    Mesh* mesh; // The mesh
    Engine* engine; // eggin
    bool paused = false;
    SpriteSource* spriteSource;
  };



} // End Of Dapper Namespace

//------------------------------------------------------------------------------
