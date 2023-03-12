//------------------------------------------------------------------------------
//
// File Name:	IComponent.h
// Author(s):	Isaiah Dickison
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "Deserializer.h"
#include "rapidjson/document.h"
#include <string>
#include "IJsonWriter.h"

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper
{
  class IComponent
  {
  public:
    IComponent() = default;
    IComponent(IComponent&& iComponent) noexcept = default;
    IComponent(const IComponent& iComponent) = default;

    IComponent& operator=(IComponent&& iComponent) noexcept = default;
    IComponent& operator=(const IComponent& iComponent) = default;
    virtual void Init() = 0;

    virtual void Update(float dt) = 0;

    virtual ~IComponent() = 0 {};

    virtual int GetParentID() const = 0;

    virtual void SetParentID(int ID) = 0;

    virtual const std::string& GetName() const = 0;

    virtual void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) = 0;

    virtual void SetID(int ID) = 0;
    
    //dont implement until level editor is up
    virtual void Write(IJsonWriter& writer) const = 0;

    virtual void ImGuiDisplay(float scale) = 0;

    virtual const int GetID() const = 0;

    virtual void SetPause(bool Condition) = 0;
    virtual bool GetPause() const = 0;
    bool isEnabled = true;
  };



} // End Of Dapper Namespace

//------------------------------------------------------------------------------


