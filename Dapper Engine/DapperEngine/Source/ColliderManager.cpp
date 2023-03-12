
//------------------------------------------------------------------------------
//
// File Name: ColliderManager.cpp
// Author(s):	Gavin Lu
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "ColliderManager.h"
#include "Deserializer.h"
#include "ComponentRegistry.h"
#include "BehaviorDeserializer.h"
#include "Utility.h"
#include "Renderer.h"
namespace Dapper
{
  /*--------------------------------------
  Description: ColliderManager Constructor
  Return: Void
  ---------------------------------------*/
  ColliderManager::ColliderManager(Engine& engine_new) : my_engine(engine_new)
  {
  }

  ColliderManager::~ColliderManager()
  {
  }
  
  const std::string& ColliderManager::Name() const
  {
    return name;
  }
  
  void ColliderManager::Initialize()
  {

    TransformManager& trans = *GetSystemFromEngine<TransformManager*>(my_engine, "TransformManager");
    trans.RegisterOnChange(std::bind(&ColliderManager::UpdateTransformPointers, this));

    PhysicsManager& phys = *GetSystemFromEngine<PhysicsManager*>(my_engine, "PhysicsManager");
    phys.RegisterOnChange(std::bind(&ColliderManager::UpdatePhysicsPointers, this));

    // Registers Component
    ComponentRegistry::FunctionBuffer SquareColliderFunctions{ SquareColliderCreate, SquareColliderDestroy, SquareColliderGet };

    dynamic_cast<Deserializer*>(my_engine.Get("Deserializer"))->RegisterFunction("SquareCollider", SquareColliderDeserialize);
    dynamic_cast<ComponentRegistry*>(my_engine.Get("ComponentRegistry"))->Register("SquareCollider", SquareColliderFunctions);

    ComponentRegistry::FunctionBuffer CircleColliderFunctions{ CircleColliderCreate, CircleColliderDestroy, CircleColliderGet };

    dynamic_cast<Deserializer*>(my_engine.Get("Deserializer"))->RegisterFunction("CircleCollider", CircleColliderDeserialize);
    dynamic_cast<ComponentRegistry*>(my_engine.Get("ComponentRegistry"))->Register("CircleCollider", CircleColliderFunctions);

  }
  
  void ColliderManager::Update(float dt)
  {

    dt;
    for(unsigned i = 0; i < squares.size(); i++)
    {
      
      for (unsigned j = 0; j < circles.size(); j++)
      {
        
        Transform& transformA = *(squares[i].GetTransformPointer());
        Transform& transformB = *(circles[j].GetTransformPointer());
        Physics& Physics1 = *(squares[i].GetPhysicsPointer());
        Physics& Physics2 = *(circles[j].GetPhysicsPointer());

        AABBvsCircle(squares[i], circles[j], my_engine, Physics1, Physics2, transformA, transformB);
        
      }
      
    }

    for (unsigned i = 0; i < circles.size(); i++)
    {

      for (unsigned j = i + 1; j < circles.size(); j++)
      {

        Transform& transformA = *(circles[i].GetTransformPointer());
        Transform& transformB = *(circles[j].GetTransformPointer());
        Physics& Physics1 = *(circles[i].GetPhysicsPointer());
        Physics& Physics2 = *(circles[j].GetPhysicsPointer());

        CircleVsCircleOptimized(circles[i], circles[j], my_engine, Physics1, Physics2, transformA, transformB);

      }

    }

    for (unsigned i = 0; i < squares.size(); i++)
    {
      SquareCollider& square1 = squares[i];
      Transform& transformA = *(square1.GetTransformPointer());
      Physics& Physics1 = *(square1.GetPhysicsPointer());

      for (unsigned j = i + 1; j < squares.size(); j++)
      {
        SquareCollider& square2 = squares[j];
        Transform& transformB = *(square2.GetTransformPointer());
        Physics& Physics2 = *(square2.GetPhysicsPointer());

        SimpleAABBvsAABB(square1, square2, my_engine, Physics1, Physics2, transformA, transformB);

      }

    }
    
  }
  
  void ColliderManager::Render()
  {
    if (debug_draw) 
    {
      for (unsigned i = 0; i < squares.size(); i++)
      {
        
        Renderer* drawer = GetSystemFromEngine<Renderer*>(my_engine, "Renderer");

        SquareCollider& square = squares[i];

        Transform& trans = *GetComponentFromParent<Transform>(my_engine, square.GetParentID(), "Transform");
        glm::vec3 truepos = trans.GetTranslation();
        drawer->DebugSquare({ truepos.x,truepos.y }, square.GetSize(), glm::vec4(1, 0, 0, 1));

      }
    }
  }
  
  void ColliderManager::DestroyCircle(int ID)
  {
    ID_manager_circle.Destroy(circles, ID);
  }
  
  void ColliderManager::DestroySquare(int ID)
  {
    ID_manager_square.Destroy(squares, ID);
  }
  
  int ColliderManager::GetNewCircle()
  {
    
    CircleCollider newCircle(&my_engine);
    int obj_ID = ID_manager_circle.Create(circles, newCircle);
    circles[ID_manager_circle[obj_ID]].SetID(obj_ID);
    return obj_ID;
    
  }
  
  int ColliderManager::GetNewSquare()
  {
    
    SquareCollider newSquare(&my_engine);
    int obj_ID = ID_manager_square.Create(squares, newSquare);
    squares[ID_manager_square[obj_ID]].SetID(obj_ID);
    return obj_ID;
    
  }
  
  void ColliderManager::ToggleDebugDraw()
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

  CircleCollider& ColliderManager::GetCircle(int ID)
  {
    return circles[ID_manager_circle[ID]];
  }

  SquareCollider& ColliderManager::GetSquare(int ID)
  {
    return squares[ID_manager_square[ID]];
  }
  
  void ColliderManager::UpdateTransformPointers()
  {

    for (int i = 0; i < squares.size(); i++)
    {

      squares[i].SetTransformPointer(GetComponentFromParent<Transform>(my_engine, squares[i].GetParentID(), "Transform"));

    }
    
    for (int i = 0; i < circles.size(); i++)
    {

      circles[i].SetTransformPointer(GetComponentFromParent<Transform>(my_engine, circles[i].GetParentID(), "Transform"));

    }

  }

  void ColliderManager::UpdatePhysicsPointers()
  {

    for (int i = 0; i < squares.size(); i++)
    {

      squares[i].SetPhysicsPointer(GetComponentFromParent<Physics>(my_engine, squares[i].GetParentID(), "Physics"));

    }
    
    for (int i = 0; i < circles.size(); i++)
    {

      circles[i].SetPhysicsPointer(GetComponentFromParent<Physics>(my_engine, circles[i].GetParentID(), "Physics"));

    }

  }
  
  bool ColliderManager::UpdateWhilePaused() const
  {
      return false;
  }
  
  std::pair<std::string_view, int> SquareColliderDeserialize(Engine& engine, const rapidjson::Value& val)
	{
		ColliderManager& manager = *GetSystemFromEngine<ColliderManager*>(engine, "ColliderManager");
		Deserializer& deserial = *GetSystemFromEngine<Deserializer*>(engine, "Deserializer");
		int ID = manager.GetNewSquare();
		SquareCollider& toReturn = manager.GetSquare(ID);
		toReturn.Read(deserial, val);
		return { toReturn.GetName(), ID };
	}
  
  std::pair<std::string_view, int> CircleColliderDeserialize(Engine& engine, const rapidjson::Value& val)
	{
		ColliderManager& manager = *GetSystemFromEngine<ColliderManager*>(engine, "ColliderManager");
		Deserializer& deserial = *GetSystemFromEngine<Deserializer*>(engine, "Deserializer");
		int ID = manager.GetNewCircle();
		CircleCollider& toReturn = manager.GetCircle(ID);
		toReturn.Read(deserial, val);
		return { toReturn.GetName(), ID };
	}
  
  SquareCollider* SquareColliderCreate(Engine& engine)
  {
    ColliderManager& the_man = *GetSystemFromEngine<ColliderManager*>
      (engine, "ColliderManager");
    int index = the_man.GetNewSquare();
    return &the_man.GetSquare(index);
  }

  void SquareColliderDestroy(int ID, Engine& engine)
  {
    ColliderManager& the_man = *GetSystemFromEngine<ColliderManager*>
      (engine, "ColliderManager");
    the_man.DestroySquare(ID);
  }

  SquareCollider* SquareColliderGet(int ID, Engine& engine)
  {
    ColliderManager& the_man = *GetSystemFromEngine<ColliderManager*>
      (engine, "ColliderManager");
    return &the_man.GetSquare(ID);
  }
  
  CircleCollider* CircleColliderCreate(Engine& engine)
  {
    ColliderManager& the_man = *GetSystemFromEngine<ColliderManager*>
      (engine, "ColliderManager");
    int index = the_man.GetNewCircle();
    return &the_man.GetCircle(index);
  }

  void CircleColliderDestroy(int ID, Engine& engine)
  {
    ColliderManager& the_man = *GetSystemFromEngine<ColliderManager*>
      (engine, "ColliderManager");
    the_man.DestroyCircle(ID);
  }

  CircleCollider* CircleColliderGet(int ID, Engine& engine)
  {
    ColliderManager& the_man = *GetSystemFromEngine<ColliderManager*>
      (engine, "ColliderManager");
    return &the_man.GetCircle(ID);
  }
  
}