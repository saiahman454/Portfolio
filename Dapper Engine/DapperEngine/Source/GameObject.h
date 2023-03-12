//------------------------------------------------------------------------------
//
// File Name:	GameObject.h
// Author(s):	Isaiah Dickison
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
//------------------------------------------------------------------------------
// Include Files:
//#include <string>
#include "Deserializer.h"
#include "Engine.h"
#include "IJsonWriter.h"
#include "Messenger.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper
{
  class GameObject {
  public:
    //constuctor
    GameObject(Engine* engine);
    //destructor
    ~GameObject();
    void PrepDestroy();

    //Name
    inline const std::string& GetTypeName()const { return typeName; }
    inline const std::string& GetName()const { return name; }
    inline void SetName(std::string obj_name) 
    { 
        name = obj_name; 
        onObjectRename.SendMessage();
    }

    //ID
    inline void SetID(int id) { ID = id; }
    inline int GetID()const { return ID; }

    //Components
    int GetComponentID(const std::string_view& compname)const;
    void SetComponentID(const std::string_view& compname, int ID);
    void RemoveComponent(const std::string_view comp_name);
    void AddComponent(const std::string_view comp_name, int ID);

    //Init
    void Initialize();

    //Json
    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal);
    void Write(IJsonWriter& writer);

    //Pausing for Scenes
    void SetPause(bool Condition);
    inline bool GetPause() { return paused; }

    //Flag
    inline std::string GetFlag() const { return flag; }
    inline void SetFlag(const char* new_flag) { flag = new_flag; }

    //Map
    inline const std::unordered_map<std::string_view, int>& GetMap() { return components; }

    // Messaging
    inline void RegisterOnObjectRename(Messenger<>::func function)
    {
        onObjectRename.RegisterMessage(function);
    }

    void setEnabled(bool condition);
    inline bool isEnabled() const { return enabled; }

  private:
    std::string name; //object name
    const inline static std::string typeName = "GameObject"; //type name
    std::unordered_map<std::string_view, int> components;//list of compoents 
    Engine* my_engine;//parent engine
    int ID;
    std::string flag = "";
    Messenger<> onObjectRename; // Sort game object list on rename
    bool paused = false;
    bool enabled = true;
  };


} // End Of Dapper Namespace

//------------------------------------------------------------------------------


