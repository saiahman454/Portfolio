//------------------------------------------------------------------------------
//
// File Name:	TransformManager.cpp
// Author(s):	Gavin Lu
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "TransformManager.h"
#include "Deserializer.h"
#include "ComponentRegistry.h"
#include "Renderer.h"
#include "Utility.h"

// Dapper Namespace!
namespace Dapper
{

  TransformManager::TransformManager(Engine& engine_new) : my_engine(engine_new)
  {
  }

  TransformManager::~TransformManager()
  {
  }

  const std::string& TransformManager::Name() const
  {
    return name;
  }

  void TransformManager::Initialize()
  {
    ComponentRegistry::FunctionBuffer TransformFunctions{TransformCreate, TransformDestroy, TransformGet};

    dynamic_cast<Deserializer*>(my_engine.Get("Deserializer"))->RegisterFunction("Transform", TransformDeserialize);
    dynamic_cast<ComponentRegistry*>(my_engine.Get("ComponentRegistry"))->Register("Transform",TransformFunctions);
  
  }

  void TransformManager::Update(float dt)
  {
    for (unsigned i = 0; i < transform.size(); i++)
    {
        if (transform[i].GetPause() == false)
        {
            (transform[i]).Update(dt);
        }
    }
  }
  
  void TransformManager::Render()
  {
    if (debug_draw)
    {
      for (unsigned i = 0; i < transform.size(); i++)
      {
        Renderer* drawer = GetSystemFromEngine<Renderer*>(my_engine, "Renderer");

        Transform& trans = transform[i];

        //Transform* trans = GetComponentFromParent<Transform>(my_engine, phys.GetParentID(), "Transform");
        drawer->DebugSquare(trans.GetTranslation(), trans.GetScale(), glm::vec4(0, 1, 0, 1));
      }
    }
  }

  void TransformManager::Destroy(int ID)
  {

    ID_manager.Destroy(transform, ID);
    on_change.SendMessage();

  }

  int TransformManager::GetNew()
  {
    Transform trans;
    bool temp = transform.size() == transform.capacity();
    int obj_ID = ID_manager.Create(transform, trans);
    transform[ID_manager[obj_ID]].SetID(obj_ID);
    //transform[ID_manager[obj_ID]].Init();

    if(temp)
      on_change.SendMessage();

    return obj_ID;
    
  }

  Transform& TransformManager::operator[](int ID)
  {
    return transform[ID_manager[ID]];

  }

  void TransformManager::RegisterOnChange(Messenger<>::func MessengerFunction)
  {

    on_change.RegisterMessage(MessengerFunction);

  }

  Transform* TransformManager::Has(int ID)
  {
    if (ID_manager.Has(ID))
    {
      return &transform[ID_manager[ID]];
    }
    return nullptr;
  }

  void TransformManager::ToggleDebugDraw()
  {
    if (debug_draw == true)
    {
      debug_draw = false;
    }
    else
    {
      debug_draw = true;
    }
  }

  bool TransformManager::UpdateWhilePaused() const
  {
      return false;
  }

  std::pair<std::string_view, int> TransformDeserialize(Engine& engine, const rapidjson::Value& val)
  {
    TransformManager& the_man = *dynamic_cast<TransformManager*>(engine.Get("TransformManager"));
    int index = the_man.GetNew();
    Transform& obj = the_man[index];
    Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
    obj.Read(des, val);
    return { obj.GetName(), index };
  }

  Transform* TransformCreate(Engine& engine)
  {

    TransformManager& the_man = *dynamic_cast<TransformManager*>(engine.Get("TransformManager"));
    int index = the_man.GetNew();
    return &the_man[index];

  }

  void TransformDestroy(int ID, Engine& engine)
  {

    TransformManager& the_man = *dynamic_cast<TransformManager*>(engine.Get("TransformManager"));
    the_man.Destroy(ID);
    

  }

  Transform* TransformGet(int ID, Engine& engine)
  {

    TransformManager& the_man = *dynamic_cast<TransformManager*>(engine.Get("TransformManager"));
    return the_man.Has(ID);

  }



} // End Of Dapper Namespace