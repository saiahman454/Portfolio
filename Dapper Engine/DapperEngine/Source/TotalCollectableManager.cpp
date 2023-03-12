//------------------------------------------------------------------------------
//
// File Name:	TotalCollectableManager.cpp
// Author(s):	
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "TotalCollectableManager.h"
#include "Utility.h"
#include "ComponentRegistry.h"

// Dapper Namespace!
namespace Dapper 
{
    TotalCollectableManager::TotalCollectableManager(Engine& engine): my_engine(engine)
    {
    }

    TotalCollectableManager::~TotalCollectableManager()
    {
    }

    void TotalCollectableManager::Initialize()
    {
      ComponentRegistry::FunctionBuffer TotalCollectableFunctions{ TotalCollectableCreate, TotalCollectableDestroy, TotalCollectableGet };

      dynamic_cast<Deserializer*>(my_engine.Get("Deserializer"))->RegisterFunction("TotalCollectable", TotalCollectableDeserialize);
      dynamic_cast<ComponentRegistry*>(my_engine.Get("ComponentRegistry"))->Register("TotalCollectable", TotalCollectableFunctions);
    }

    void TotalCollectableManager::Update(float dt)
    {
        dt;
        for (TotalCollectable& current : totals)
        {
          if (current.GetPause() == false)
          {
            current.Update(dt);
          }
        }
    }

    void TotalCollectableManager::Render()
    {
    }

    bool TotalCollectableManager::UpdateWhilePaused() const
    {
      return false;
    }

    void TotalCollectableManager::Destroy(int ID)
    {
      ID_manager.Destroy(totals, ID);
    }

    TotalCollectable& TotalCollectableManager::operator[](int index)
    {
      return totals[ID_manager[index]];
    }

    int TotalCollectableManager::GetNew()
    {
      TotalCollectable new_obj(&my_engine);
      int obj_ID = ID_manager.Create(totals, new_obj);
      totals[ID_manager[obj_ID]].SetID(obj_ID);
      return obj_ID;
    }


    std::pair<std::string_view, int> TotalCollectableDeserialize(Engine& engine, const rapidjson::Value& val)
    {
      TotalCollectableManager& the_man = *GetSystemFromEngine<TotalCollectableManager*>(engine, "TotalCollectableManager");
      Deserializer& deserial = *GetSystemFromEngine<Deserializer*>(engine, "Deserializer");
      int ID = the_man.GetNew();
      TotalCollectable& obj = the_man[ID];
      obj.Read(deserial, val);
      return { obj.GetName(), ID };
    }

    TotalCollectable* TotalCollectableCreate(Engine& engine)
    {
      TotalCollectableManager& the_man = *GetSystemFromEngine<TotalCollectableManager*>(engine, "TotalCollectableManager");
      int ID = the_man.GetNew();
      return &the_man[ID];
    }

    void TotalCollectableDestroy(int ID, Engine& engine)
    {
      TotalCollectableManager& the_man = *GetSystemFromEngine<TotalCollectableManager*>(engine, "TotalCollectableManager");
      the_man.Destroy(ID);
    }

    TotalCollectable* TotalCollectableGet(int ID, Engine& engine)
    {
      TotalCollectableManager& the_man = *GetSystemFromEngine<TotalCollectableManager*>(engine, "TotalCollectableManager");
      return &the_man[ID];
    }


} // End Of Dapper Namespace