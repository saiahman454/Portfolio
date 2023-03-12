//------------------------------------------------------------------------------
//
// File Name:	TransformManager.h
// Author(s):	Gavin Lu
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
#include "ITypeManager.h"
#include "vector"
#include "Transform.h"
#include "Engine.h"
#include <glm/gtx/transform.hpp>
#include "rapidjson/document.h"
#include "IDManager.h"
#include "Messenger.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper
{
  class TransformManager : public ITypeManager<Transform>
  {
  public:
    TransformManager(Engine& engine_new);
    ~TransformManager()override;

    const std::string& Name()const override;

    void Initialize()override;

    void Update(float dt) override;

    void Render() override;

    void Destroy(int ID);

    int GetNew() override;

    Transform& operator[] (int index)override;

    void RegisterOnChange(Messenger<>::func MessengerFunction);

    Transform* Has(int ID);

    void ToggleDebugDraw();

  private:

    Messenger<> on_change;

    const std::string name = "TransformManager";

    std::vector<Transform> transform;

    Engine& my_engine;

    IDManager ID_manager;

    bool debug_draw = false;
    // Inherited via ITypeManager
    virtual bool UpdateWhilePaused() const override;

  }; //End of TransformManager class

  std::pair<std::string_view, int> TransformDeserialize(Engine& engine, const rapidjson::Value& val);

  Transform* TransformCreate(Engine& engine);
  void TransformDestroy(int ID, Engine& engine);
  Transform* TransformGet(int ID, Engine& engine);

} // End Of Dapper Namespace