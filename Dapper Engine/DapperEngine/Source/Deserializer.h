//------------------------------------------------------------------------------
//
// File Name:	Deserializer.h
// Author(s):	Isaiah Dickison
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "ISystem.h"
#include <vector>
#include <unordered_map>
#include <functional>
#include "rapidjson/document.h"
#include <utility>

#include "Engine.h"
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------


// Dapper Namespace!
namespace Dapper 
{
  using DeserializeFunction = std::function<std::pair<std::string_view, int>(Engine& engine, const rapidjson::Value& val)>;

  class Deserializer : public ISystem{
  public:
    Deserializer(Engine& engine);
    ~Deserializer();

   inline const std::string& Name()const override{ return name; };

    void Initialize()override;

    void Update(float dt) override;

    //not needed in Deserializer
    void Render() override;

    std::pair<std::string_view, int> Deserialize(const std::string_view& typeName,const rapidjson::Value& jsonVal)const;

    inline void RegisterFunction(const std::string_view& typeName, DeserializeFunction func){ my_map.insert({ typeName, func }); };

    void DeserializeFile(std::string filename);

    inline std::unordered_map<std::string_view, DeserializeFunction>& GetMap(){ return my_map; };

    Engine& GetEngine();

    int DeserializePrefab(std::string_view filename);

  private:

    const std::string name = "Deserializer";
    std::vector<std::string> file_paths;
    bool have_request = true;
    std::unordered_map<std::string_view, DeserializeFunction> my_map;
    Engine& my_engine;



    // Inherited via ISystem
    inline bool UpdateWhilePaused() const { return true; }

  };//End of Deserializer class

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


