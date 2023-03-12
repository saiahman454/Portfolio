//------------------------------------------------------------------------------
//
// File Name:	Stub.h
// Author(s):	
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "IComponent.h"
#include "Engine.h"
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class TotalCollectable :public IComponent
  {
  public:
    TotalCollectable(Engine* engine);

    void Init() override;
    void Update(float dt) override;
    int GetParentID() const override;
    void SetParentID(int ID) override;
    const std::string& GetName() const override;
    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;
    void SetID(int ID) override;
    void Write(IJsonWriter& writer) const override;
    void ImGuiDisplay(float scale) override;
    const int GetID() const override;
    void SetPause(bool Condition) override;
    bool GetPause() const override;

  private:

    static inline const std::string name = "TotalCollectable";
    Engine* my_engine;
    int ID;
    int my_parent_id;
    std::string file_path;
    int level = -1;
  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


