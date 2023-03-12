//------------------------------------------------------------------------------
//
// File Name:	ComponentRegistry.cpp
// Author(s):	Gavin Lu
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "ComponentRegistry.h"

// Dapper Namespace!
namespace Dapper {
  ComponentRegistry::ComponentRegistry(Engine& engine) : my_engine(engine)
  {
  }
  const std::string& ComponentRegistry::Name() const
  {
    return name;
  }

  void ComponentRegistry::Initialize()
  {
  }

  void ComponentRegistry::Update(float dt)
  {

    dt;

  }

  void ComponentRegistry::Render()
  {
  }

  void ComponentRegistry::Register(const std::string_view& string, FunctionBuffer funcbuffer)
  {

    function_map.insert(std::make_pair(string, funcbuffer));
    components.push_back(string);
  }

  IComponent* ComponentRegistry::Construct(const std::string_view& string)
  {

    auto var = function_map.find(string);
    if (var != function_map.end())
    {

      return var->second.construct(my_engine);

    }

    return nullptr;

  }

  void ComponentRegistry::Destroy(const std::string_view& string, int ID)
  {

    auto var = function_map.find(string);
    if (var != function_map.end())
    {

      var->second.destroy(ID, my_engine);

    }

  }

  IComponent* ComponentRegistry::GetComponent(const std::string_view& string, int ID)
  {
    if (ID != -1) 
    {
      auto var = function_map.find(string);
      if (var != function_map.end())
      {

        return var->second.get(ID, my_engine);

      }
    }
    return nullptr;

  }

  const std::vector<std::string_view>& ComponentRegistry::GetCompList()const
  {
    return components;
  }

  bool ComponentRegistry::UpdateWhilePaused() const
  {
      return false;
  }

} // End Of Dapper Namespace