//------------------------------------------------------------------------------
//
// File Name:	PhysicsManager.cpp
// Author(s):	Isaiah Dickison
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "PhysicsManager.h"
#include "Utility.h"
#include "Renderer.h"
#include "Transform.h"


// Dapper Namespace!
namespace Dapper 
{    
  /*--------------------------------------
  Description: PhysicsManager constuctor
  ---------------------------------------*/
  PhysicsManager::PhysicsManager(Engine& engine) :
    my_engine(engine)
  {
  }

  /*--------------------------------------
  Description: PhysicsManager destuctor
  ---------------------------------------*/
  PhysicsManager::~PhysicsManager()
  {
  }

  /*--------------------------------------
  Description: Initialize function
  ---------------------------------------*/
  void PhysicsManager::Initialize()
  {
    ComponentRegistry::FunctionBuffer PhysicsFunctions{ PhysicsCreate, PhysicsDestroy, PhysicsGet };

    dynamic_cast<Deserializer*>(my_engine.Get("Deserializer"))->RegisterFunction("Physics", PhysicsDeserialize);
    dynamic_cast<ComponentRegistry*>(my_engine.Get("ComponentRegistry"))->Register("Physics", PhysicsFunctions);
  }

  /*--------------------------------------
  Description: Update function
  Params: dt - delta time 
  ---------------------------------------*/
  void PhysicsManager::Update(float dt)
  {
    for (unsigned i = 0; i < physics.size(); i++)
    {
        if (physics[i].GetPause() == false)
        {
            physics[i].Update(dt);
        }
    }
  }

  /*--------------------------------------
  Description: Render function
  ---------------------------------------*/
  void PhysicsManager::Render()
  {
    if (debug_draw) 
    {
      for (unsigned i = 0; i < physics.size(); i++)
      {
        Renderer* drawer = GetSystemFromEngine<Renderer*>(my_engine, "Renderer");

        Physics& phys = physics[i];

        Transform* trans = GetComponentFromParent<Transform>(my_engine, phys.GetParentID(), "Transform");
        glm::vec3 vel(phys.GetVelocity(), 0);
        drawer->DebugLine(trans->GetTranslation(), trans->GetTranslation() + vel / 10.0f, glm::vec4(1));
      }
    }
  }

  /*--------------------------------------
  Description: makes a new physics component
  Return: the id of the new physics component
  ---------------------------------------*/
  int PhysicsManager::GetNew()
  {
    bool temp = physics.size() == physics.capacity();
    Physics new_obj(&my_engine);
    int obj_ID = ID_manager.Create(physics, new_obj);
    physics[ID_manager[obj_ID]].SetID(obj_ID);
    if (temp)
      on_change.SendMessage(*this);
    return obj_ID;
  }

  /*--------------------------------------
  Description: destorys a physics with the given id
  Params: ID - the physics id that you want destroyed
  ---------------------------------------*/
  void PhysicsManager::Destroy(int ID)
  {
    ID_manager.Destroy(physics, ID);
    on_change.SendMessage(*this);
  }

  /*--------------------------------------
  Description: registers to the messanger
  Params: 
  messanger function - the function you want to register
  ---------------------------------------*/
  void PhysicsManager::RegisterOnChange(Messenger<PhysicsManager&>::func MessengerFunction)
  {
    on_change.RegisterMessage(MessengerFunction);
  }

  /*--------------------------------------
  Description: checks if that id exists
  Returns: the found physics or a null pointer
  ---------------------------------------*/
  Physics* PhysicsManager::Has(int ID)
  {
      if (ID_manager.Has(ID))
      {
          return &physics[ID_manager[ID]];
      }
      return nullptr;
  }

  /*--------------------------------------
  Description: Toggles debug draw
  ---------------------------------------*/
  void PhysicsManager::ToggleDebugDraw()
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


  /*--------------------------------------
  Description: deserializes a physics
  Params:
  engine - the patrent engine
  val - the json value
  Return: the pair of the name and id
  ---------------------------------------*/
  std::pair<std::string_view, int> PhysicsDeserialize(Engine& engine, const rapidjson::Value& val)
  {
    PhysicsManager& the_man = *GetSystemFromEngine<PhysicsManager*>(engine, "PhysicsManager");
    Deserializer& deserial = *GetSystemFromEngine<Deserializer*>(engine, "Deserializer");
    int ID = the_man.GetNew();
    Physics& obj = the_man[ID];
    obj.Read(deserial, val);
    

    return { obj.GetName(), ID };
  }

  /*--------------------------------------
  Description: creates a physics
  Params: engine - the parent engine
  Return: the physics created
  ---------------------------------------*/
  Physics* PhysicsCreate(Engine& engine)
  {
    PhysicsManager& the_man = *GetSystemFromEngine<PhysicsManager*>(engine, "PhysicsManager");
    int ID = the_man.GetNew();
    return &the_man[ID];
  }

  /*--------------------------------------
  Description: destroys physics
  Params:
  ID - the id you want to destroy
  engine -  the parent engine
  ---------------------------------------*/
  void PhysicsDestroy(int ID, Engine& engine)
  {
    PhysicsManager& the_man = *GetSystemFromEngine<PhysicsManager*>(engine, "PhysicsManager");
    the_man.Destroy(ID);
  }

  /*--------------------------------------
  Description: Gets a physics using a id
  Params:
  ID - the id you want deleted
  engine - the parent engine
  Return: the found physics or null pointer
  ---------------------------------------*/
  Physics* PhysicsGet(int ID, Engine& engine)
  {
    PhysicsManager& the_man = *GetSystemFromEngine<PhysicsManager*>(engine, "PhysicsManager");

    return the_man.Has(ID);
  }

} // End Of Dapper Namespace


