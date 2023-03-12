//------------------------------------------------------------------------------
//
// File Name:	TileManager.cpp
// Author(s):	
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "TileManager.h"
#include "Utility.h"
#include "TotalCollectableManager.h"

// Dapper Namespace!
namespace Dapper
{
  /*--------------------------------------
  Description: Constructor for a TileManager.
  Param:
  engine - the parent engine.
  ---------------------------------------*/
  TileManager::TileManager(Engine& engine) : my_engine(engine)
  {
  }

  /*--------------------------------------
  Description: Destructor for a TileManager.
  ---------------------------------------*/
  TileManager::~TileManager()
  {
  }

  /*--------------------------------------
  Description: Initialize function
  ---------------------------------------*/
  void TileManager::Initialize()
  {
    ComponentRegistry::FunctionBuffer TileFunctions{ TileCreate, TileDestroy, TileGet };

    dynamic_cast<Deserializer*>(my_engine.Get("Deserializer"))->RegisterFunction("Tile", TileDeserialize);
    dynamic_cast<ComponentRegistry*>(my_engine.Get("ComponentRegistry"))->Register("Tile", TileFunctions);
  }

  /*--------------------------------------
  Description: update function
  Param:
  dt - delta time.
  ---------------------------------------*/
  void TileManager::Update(float dt)
  {
    for (Tile& current : tiles)
    {
        if (current.GetPause() == false)
        {
            current.Update(dt);
        }
    }
  }

  /*--------------------------------------
  Description: Render function
  ---------------------------------------*/
  void TileManager::Render()
  {

  }

  /*--------------------------------------
  Description: makes a new tile component
  Return: the id of the new component
  ---------------------------------------*/
  int TileManager::GetNew()
  {
    Tile new_obj(&my_engine);
    int obj_ID = ID_manager.Create(tiles, new_obj);
    tiles[ID_manager[obj_ID]].SetID(obj_ID);
    return obj_ID;
  }

  /*--------------------------------------
  Description: destroys a tile by given id
  Params: ID - the id to search by
  ---------------------------------------*/
  void TileManager::Destroy(int ID)
  {
    ID_manager.Destroy(tiles, ID);
  }



  /*--------------------------------------
  Description: deserializes a tile
  Params:
  engine - the patrent engine
  val - the json value
  Return: the pair of the name and id
  ---------------------------------------*/
  std::pair<std::string_view, int> TileDeserialize(Engine& engine, const rapidjson::Value& val)
  {
    TileManager& the_man = *GetSystemFromEngine<TileManager*>(engine, "TileManager");
    Deserializer& deserial = *GetSystemFromEngine<Deserializer*>(engine, "Deserializer");
    int ID = the_man.GetNew();
    Tile& obj = the_man[ID];
    obj.Read(deserial, val);

    return { obj.GetName(), ID };
  }


  /*--------------------------------------
  Description: creates a tile
  Params: engine - the parent engine
  Return: the physics created
  ---------------------------------------*/
  Tile* TileCreate(Engine& engine)
  {
    TileManager& the_man = *GetSystemFromEngine<TileManager*>(engine, "TileManager");
    int ID = the_man.GetNew();
    return &the_man[ID];
  }

  /*--------------------------------------
  Description: destroys a tile
  Params:
  ID - the id you want to destroy
  engine -  the parent engine
  ---------------------------------------*/
  void TileDestroy(int ID, Engine& engine)
  {
    TileManager& the_man = *GetSystemFromEngine<TileManager*>(engine, "TileManager");
    the_man.Destroy(ID);
  }

  /*--------------------------------------
  Description: Gets a physics using a id
  Params:
  ID - the id you want deleted
  engine - the parent engine
  Return: the found physics or null pointer
  ---------------------------------------*/
  Tile* TileGet(int ID, Engine& engine)
  {
    TileManager& the_man = *GetSystemFromEngine<TileManager*>(engine, "TileManager");

    return &the_man[ID];
  }




} // End Of Dapper Namespace