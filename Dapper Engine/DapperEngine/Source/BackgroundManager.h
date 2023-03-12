//------------------------------------------------------------------------------
//
// File Name:	BackgroundManager.h
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "ITypeManager.h"
#include "IDManager.h"
#include <vector>
#include "Background.h"
#include "Engine.h"
#include "rapidjson/document.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper
{
  class BackgroundManager : public ITypeManager<Background>
  {
  public:
      BackgroundManager(Engine& engine_new);
    ~BackgroundManager()override;

    const std::string& Name()const override;
    void Initialize()override;
    void Update(float dt) override;
    void Render() override;
    void Destroy(int ID);
    int GetNew() override;

    Background& operator[] (int index)override;


  private:
    const static inline std::string name = "BackgroundManager";

    std::vector<Background> background;

    Engine& my_engine;

    IDManager ID_manager;


    // Inherited via ITypeManager
    virtual bool UpdateWhilePaused() const override;

  }; //End of BackgroundManager class

  std::pair<std::string_view, int> BackgroundDeserialize(Engine& engine, const rapidjson::Value& val);

  Background* BackgroundCreate(Engine& engine);
  void BackgroundDestroy(int ID, Engine& engine);
  Background* BackgroundGet(int ID, Engine& engine);

} // End Of Dapper Namespace