//------------------------------------------------------------------------------
//
// File Name:	SpriteSourceManager.h
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
#include "ITypeManager.h"
#include "Engine.h"
#include "SpriteSource.h"
#include <vector>
#include "IDManager.h"
#include "TextureManager.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper
{
  class SpriteSourceManager : public ITypeManager<SpriteSource>
  {
  public:
    // Constructor and Destructor
    SpriteSourceManager(Engine& engine1);
    ~SpriteSourceManager()override;

    // Return the name "SpriteSourceManager"
    const std::string& Name()const override;

    // Initialize the manager
    void Initialize()override;

    // Update the manager
    void Update(float dt) override;

    // Overriden render function from ISystem
    void Render() override;

    // Destroy a SpriteSource at a given ID
    void Destroy(int ID);

    // Add a new sprite source to the manager
    int GetNew() override;

    // Operator overload to return a sprite source under a specific name
    SpriteSource& operator[](std::string sprite_source_name);

    // Operator overload to return a sprite source under a specific index
    SpriteSource& operator[](int index);

    // Returns a specified sprite source if it exists under the passed in name.
    std::pair<SpriteSource*, int> HasSpriteSource(std::string spritesource_name);

  private:
    // "SpriteSourceManager"
    const std::string name = "SpriteSourceManager";

    // Vector full of sprites
    std::vector<SpriteSource> spritesources;

    // Reference to the engine
    Engine& engine;

    IDManager ID_manager;

    // Inherited via ITypeManager
    virtual bool UpdateWhilePaused() const override;

    void OnTextureReallocation(TextureManager& texture_manager);

  };//End of SpriteSourceManager class

  // Non-member functions
  std::pair<std::string_view, int> SpriteSourceDeserialize(Engine& engine, const rapidjson::Value& val);
  SpriteSource* SpriteSourceCreate(Engine& engine);
  void SpriteSourceDestroy(int ID, Engine& engine);
  SpriteSource* SpriteSourceGet(int ID, Engine& engine);

} // End Of Dapper Namespace