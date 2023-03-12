//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.h
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
#include "Texture.h"
#include "TextureManager.h"
#include "Engine.h"
#include "IComponent.h"
#include "glm/vec2.hpp"
#include "IJsonWriter.h"
#include "Messenger.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class SpriteSource : public IComponent
  {
  public:
    // Constructors and Destructor
    SpriteSource(Engine* engine);
    SpriteSource(int index, Engine* engine);
    ~SpriteSource() override;
    //SpriteSource(SpriteSource&& spriteSource) noexcept;
    //SpriteSource& operator=(SpriteSource&& sprite) noexcept;
    //SpriteSource(const SpriteSource& spriteSource) = default;
    //SpriteSource& operator=(const SpriteSource& sprite) = default;

    // Init function inherited from IComponent
    void Init() override;

    // Get and Set Functions
    inline int GetParentID() const override { return parent_ID; }
    inline void SetParentID(int newID) { parent_ID = newID; }
    inline const std::string& GetName() const override { return name; }
    inline void SetID(int ID)override { my_ID = ID; }
    inline const int GetID() const override { return my_ID; }

    inline void SetPause(bool Condition)override { paused = Condition; }
    inline bool GetPause() const override { return paused; }

    inline std::pair<glm::vec2, glm::vec2> GetCustomUV() const { return std::pair(customU, customV); }
    inline bool HasCustoms() const { return hasCustoms; }
    inline void SetID(unsigned newID) { my_ID = newID; }
    inline int GetTexture() const { return texture_ID; }
    inline int GetFrameCount() const { return rows * cols; }
    inline void SetCustomMin(glm::vec2 min) { custom_min_uv = min; hasCustoms = true; }
    inline void SetCustomMax(glm::vec2 max) { custom_max_uv = max; hasCustoms = true; }
    inline const glm::vec2 GetCustomMin()const { return custom_min_uv; }
    inline const glm::vec2 GetCustomMax()const { return custom_max_uv; }
    inline const int& GetRows()const { return rows; }
    inline const int& GetCols()const { return cols; }
    inline glm::vec2 GetUVSize() { return { 1.0f / cols , 1.0f / rows }; }
    void SetCustomUV(glm::vec2 u, glm::vec2 v, glm::vec2 min = { 0,0 }, glm::vec2 max = { 1,1 });
    void SetUVCoordinates(unsigned frame_index);
    inline void SetTextureID(int theID) { texture_ID = theID; }//ADAM
    glm::vec2 GetUVAtFrameIndex(unsigned frame_index);
    std::pair<glm::vec2, glm::vec2> CalculateUVWithIndex(unsigned index) const;

    // Update function inherited from IComponent
    void Update(float dt) override;

    // Read function inherited from IComponent
    void Read(Deserializer& deserializer, const rapidjson::Value& jsonVal) override;

    // Write function inherited from IComponent
    void Write(IJsonWriter& writer) const override;

    // Render function
    void Render(Engine& engine);

    // Display function for ImGui
    void ImGuiDisplay(float scale)override;

    // Update texture on texture manager reallocation
    void OnTextureReallocation(TextureManager& texture_manager);
    
    //Messenger<SpriteSource*> onReallocate{};

  private:
    static inline const std::string name = "SpriteSource"; // "SpriteSource"

    glm::vec2 customU;
    glm::vec2 customV;
    glm::vec2 custom_min_uv = { 0,0 };
    glm::vec2 custom_max_uv = { 1,1 };
    bool hasCustoms;
    Engine* my_engine;
    int texture_ID; // The texture of the sprite source
    int rows; // how many rows
    int cols; // how many columns
    int my_ID; // The ID of the sprite source in the sprite source manager
    int parent_ID; // The parent ID
    //Texture* texture; // yo it's a texture //bad
    std::string file_loc;
    bool paused = false;
  };

} // End Of Dapper Namespace

//------------------------------------------------------------------------------


