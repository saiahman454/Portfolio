//------------------------------------------------------------------------------
//
// File Name:	ColliderManager.h
// Author(s):	Gavin Lu
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "ITypeManager.h"
#include "vector"
#include "IComponent.h"
#include "Engine.h"
#include <glm/gtx/transform.hpp>
#include "rapidjson/document.h"
#include "IDManager.h"
#include "Transform.h"
#include "Physics.h"
#include "TransformManager.h"
#include "ICollider.h"
#include "PhysicsManager.h"
#include "SquareCollider.h"
#include "CircleCollider.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
    class ColliderManager : public ISystem
    {
    public:
      ColliderManager(Engine& engine_new);
      ~ColliderManager()override;

      const std::string& Name()const override;
      void Initialize()override;
      void Update(float dt) override;
      void Render() override;
      int GetNewCircle();
      int GetNewSquare();
      void DestroyCircle(int ID);
      void DestroySquare(int ID);
      void ToggleDebugDraw();
      CircleCollider& GetCircle(int ID);
      SquareCollider& GetSquare(int ID);

    private:
      void UpdateTransformPointers();
      void UpdatePhysicsPointers();
    
      const static inline std::string name = "ColliderManager";
      std::vector<SquareCollider> squares;
      std::vector<CircleCollider> circles;
      Engine& my_engine;
      IDManager ID_manager_circle;
      IDManager ID_manager_square;
      bool debug_draw = false;

      // Inherited via ITypeManager
      virtual bool UpdateWhilePaused() const override;

    };//End of EmitHealthManager class

    std::pair<std::string_view, int> SquareColliderDeserialize(Engine& engine, const rapidjson::Value& val);
    std::pair<std::string_view, int> CircleColliderDeserialize(Engine& engine, const rapidjson::Value& val);
    SquareCollider* SquareColliderCreate(Engine& engine);
    void SquareColliderDestroy(int ID, Engine& engine);
    SquareCollider* SquareColliderGet(int ID, Engine& engine);
    CircleCollider* CircleColliderCreate(Engine& engine);
    void CircleColliderDestroy(int ID, Engine& engine);
    CircleCollider* CircleColliderGet(int ID, Engine& engine);
    
} // End Of Dapper Namespace