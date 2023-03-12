//------------------------------------------------------------------------------
//
// File Name:	Tile.h
// Author(s):	Isaiah Dickison
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
//------------------------------------------------------------------------------
// Include Files:
#include "SpriteSource.h"
#include "imgui.h"
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class Tile : public IComponent
  {
  public:
    //Constructor
    Tile(Engine* engine);
    //Destructor
    ~Tile();

    // Inherited via IComponent
    void Init() override;

    void Update(float dt) override;

    inline int GetParentID() const override { return parent_ID; }

    inline void SetParentID(int ID) override { parent_ID = ID; }

    inline const std::string& GetName() const override { return name; }

    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;

    inline void SetID(int id) override { ID = id; }

    void Write(IJsonWriter& writer) const override;

    void ImGuiDisplay(float scale) override;

    inline const int GetID() const override { return ID; }
    //end of IComponent

    void SetPause(bool Condition)override { paused = Condition; }
    bool GetPause() const override { return paused; }

    //temporary math function
    ImVec2 makenumber(float width, float hieght);

    void IncreaseX();
    void IncreaseY();
    void DecreaseX();
    void DecreaseY();

  private:
    static const inline std::string name = "Tile";
    int parent_ID = -1;
    int ID = -1;
    Engine* my_engine;
    int tile_ID = 0;
    glm::vec2 dimension = {1,1};
    bool didchange = true;
    bool tile_set_bool = false;
    bool testbool = false;
    bool paused = false;
  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


