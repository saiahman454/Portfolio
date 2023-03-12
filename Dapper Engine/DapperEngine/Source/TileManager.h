//------------------------------------------------------------------------------
//
// File Name:	TileManager.h
// Author(s):	
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
#include "ISystem.h"
#include "ITypeManager.h"
#include "Tile.h"
#include "IDManager.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper
{
  class TileManager : public ITypeManager<Tile>
  {
  public:
    TileManager(Engine& engine);

    ~TileManager();

    inline const std::string& Name()const override{ return name; };

    void Initialize()override;

    void Update(float dt) override;

    void Render() override;

    inline Tile& operator[](int index){ return tiles[ID_manager[index]]; };

    int GetNew()override;

    void Destroy(int ID);


  private:
    static const inline std::string name = "TileManager";
    std::vector<Tile> tiles;
    IDManager ID_manager;
    Engine& my_engine;




    // Inherited via ITypeManager
    inline virtual bool UpdateWhilePaused() const override { return true; };

  };//End of TileManager class

  std::pair<std::string_view, int> TileDeserialize(Engine& engine, const rapidjson::Value& val);

  Tile* TileCreate(Engine& engine);
  void TileDestroy(int ID, Engine& engine);
  Tile* TileGet(int ID, Engine& engine);
} // End Of Dapper Namespace