//------------------------------------------------------------------------------
//
// File Name:	ComponentRegistry.h
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

#include <string>
#include "ISystem.h"
#include <functional>
#include "IComponent.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  using ConstructFunction = std::function<IComponent*(Engine& engine)>;
  using DestroyFunction = std::function<void(int ID, Engine& engine)>;
  using GetFunction = std::function<IComponent*(int ID, Engine& engine)>;

  class ComponentRegistry : public ISystem
  {
  public:

    struct FunctionBuffer{

      ConstructFunction construct;
      DestroyFunction destroy;
      GetFunction get;

    };


    ComponentRegistry(Engine& engine);

    //all systems need a deconstructor in their own files
    ~ComponentRegistry() {};
    //names for debuging 
    const std::string& Name()const override;
    //Initalize systems
    void Initialize() override;
    //Update function for game loop
    void Update(float dt) override;
    //render teh system in the game loop
    void Render() override;

    void Register(const std::string_view& string, FunctionBuffer funcbuffer);

    IComponent* Construct(const std::string_view& string);

    void Destroy(const std::string_view& string, int ID);

    IComponent* GetComponent(const std::string_view& string, int ID);

    const std::vector<std::string_view>& GetCompList()const;

  private:

    const std::string name = "ComponentRegistry";
    std::unordered_map<std::string_view, FunctionBuffer> function_map;
    Engine& my_engine;
    std::vector<std::string_view> components;


    // Inherited via ISystem
    virtual bool UpdateWhilePaused() const override;

  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


