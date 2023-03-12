//------------------------------------------------------------------------------
//
// File Name:	BackgroundManager.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "BackgroundManager.h"
#include "Deserializer.h"
#include "ComponentRegistry.h"

// Dapper Namespace!
namespace Dapper
{

    BackgroundManager::BackgroundManager(Engine& engine_new) : my_engine(engine_new)
  {
  }

    BackgroundManager::~BackgroundManager()
  {
  }

  const std::string& BackgroundManager::Name() const
  {
    return name;
  }

  void BackgroundManager::Initialize()
  {
    ComponentRegistry::FunctionBuffer BackgroundFunctions{ BackgroundCreate,
        BackgroundDestroy, BackgroundGet};

    dynamic_cast<Deserializer*>(my_engine.Get("Deserializer"))->
        RegisterFunction("Background", BackgroundDeserialize);

    dynamic_cast<ComponentRegistry*>(my_engine.Get("ComponentRegistry"))->
        Register("Background", BackgroundFunctions);
  
  }

  void BackgroundManager::Update(float dt)
  {
    for (unsigned i = 0; i < background.size(); i++)
    {
        if (background[i].GetPause() == false)
        {
            (background[i]).Update(dt);
        }

    }
  }

  void BackgroundManager::Render()
  {
  }

  void BackgroundManager::Destroy(int ID)
  {

    ID_manager.Destroy(background, ID);

  }

  int BackgroundManager::GetNew()
  {
    Background theBack(&my_engine);
    int obj_ID = ID_manager.Create(background, theBack);
    background[ID_manager[obj_ID]].SetID(obj_ID);
    //background[ID_manager[obj_ID]].Init();
    return obj_ID; 
  }

  Background& BackgroundManager::operator[](int ID)
  {
    return background[ID_manager[ID]];
  }

  bool BackgroundManager::UpdateWhilePaused() const
  {
      return false;
  }



  std::pair<std::string_view, int> BackgroundDeserialize(Engine& engine, 
                                                    const rapidjson::Value& val)
  {
    BackgroundManager& the_man = *dynamic_cast<BackgroundManager*>
        (engine.Get("BackgroundManager"));

    int index = the_man.GetNew();
    Background& obj = the_man[index];
    Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
    obj.Read(des, val);
    return { obj.GetName(), index };
  }

  Background* BackgroundCreate(Engine& engine)
  {
      BackgroundManager& the_man = *dynamic_cast<BackgroundManager*>
          (engine.Get("BackgroundManager"));
    int index = the_man.GetNew();
    return &the_man[index];

  }

  void BackgroundDestroy(int ID, Engine& engine)
  {

      BackgroundManager& the_man = *dynamic_cast<BackgroundManager*>
          (engine.Get("BackgroundManager"));
    the_man.Destroy(ID);

  }

  Background* BackgroundGet(int ID, Engine& engine)
  {

      BackgroundManager& the_man = *dynamic_cast<BackgroundManager*>
          (engine.Get("BackgroundManager"));
    return &the_man[ID];

  }

} // End Of Dapper Namespace