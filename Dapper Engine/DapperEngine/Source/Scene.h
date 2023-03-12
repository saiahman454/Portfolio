//------------------------------------------------------------------------------
//
// File Name:	Scene.h
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "ISystem.h"
#include "Engine.h"
#include "IJsonWriter.h"
#include "GameObject.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class Scene{
  public:
    Scene(Engine& my_engine);
    Scene(Scene&& move)noexcept;
    ~Scene();
    Scene(const Scene&) = default;
    const std::string& GetName() const;
    void setPaused(bool condition);
    inline bool IsPaused() const { return isPaused; }
    void setEnableScene(bool condition);
    inline bool IsEnabled() const { return isEnabled; }
    void Read(std::string_view filePath);
    void UnLoad();
    void SetName(std::string_view name);
    void Save(const std::string& name = "");
    void Add(int obj_id);
    void Remove(int obj_id);
    void WriteAllSceneObjects(IJsonWriter& writer);
    bool InChargeOfObject(int objectId);
    Scene& operator=(Scene&& move)noexcept;
  private:
    Engine* my_engine;
    std::string theName;
    bool isPaused;
    std::vector<int> heldGameobjects;
    bool isEnabled = true;
  };

} // End Of Dapper Namespace

//------------------------------------------------------------------------------


