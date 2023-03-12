//------------------------------------------------------------------------------
//
// File Name:	Deserializer.cpp
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Deserializer.h"
#include <fstream>
#include "rapidjson/istreamwrapper.h"
#include "Deserializer.h"


// Dapper Namespace!
namespace Dapper {

  /*--------------------------------------
  Description: Constructor for a deserializer.
  Param:
  engine - the parent engine.
  ---------------------------------------*/
  Deserializer::Deserializer(Engine& engine) :my_engine(engine)
  {
  }
 
  /*--------------------------------------
  Description: Destructor for a deserializer.
  ---------------------------------------*/
  Deserializer::~Deserializer()
  {
  }

  /*--------------------------------------
  Description: Init for the deserializer.
  Return: void
  ---------------------------------------*/
  void Deserializer::Initialize()
  {

  }

  /*--------------------------------------
  Description: Update for deserializer,
  if deserializer has a request it will 
  read the file.
  Param:
  dt - delta time.
  Return: void
  ---------------------------------------*/
  void Deserializer::Update(float dt)
  {
    dt;
    if (have_request)
    {
      for (unsigned int i = 0; i < file_paths.size(); i++)
      {
        rapidjson::Document doc;
        std::ifstream stream(file_paths[i]);
        rapidjson::IStreamWrapper wrapper(stream);
        doc.ParseStream(wrapper);
        auto member = doc.MemberBegin();
        while (member != doc.MemberEnd())
        {
          Deserialize(member->name.GetString(), member->value);
          ++member;
        }//end of while
      }//end of for loop
      file_paths.clear();
      have_request = false;
    }
  }

  /*--------------------------------------
  Description: Unused render functions
  Return: void
  ---------------------------------------*/
  void Deserializer::Render()
  {
  }

  /*--------------------------------------
  Description: a modular function that runs
  registered deserialzer funtions.
  Param:
  typename - the name of the type you are deserializing.
  jsonVal - the value that you are passing in to read.
  Return: a pair of the component name and index of it.
  ---------------------------------------*/
  std::pair<std::string_view, int> Deserializer::Deserialize(const std::string_view& typeName, const rapidjson::Value& jsonVal) const
  {
    auto iter = my_map.find(typeName);
    if (iter != my_map.end())
    {
      return iter->second(my_engine, jsonVal);
    }
    return std::pair("", -1);//Return invalid index
  }


  /*--------------------------------------
  Description: adds a request to the deserializer
  Param:
  filename - the file you want to read
  Return: void
  ---------------------------------------*/
  void Deserializer::DeserializeFile(std::string filename)
  {
    file_paths.push_back(filename);
    have_request = true;
    //this function only adds a request and deserializes file at desializtion update
  }



  /*--------------------------------------
  Description: Gets the engine
  Param:
  Return: the parent engine
  ---------------------------------------*/
  Engine& Deserializer::GetEngine()
  {
    return my_engine;
  }

  int Deserializer::DeserializePrefab(std::string_view filename)
  {
      rapidjson::Document doc;
      std::ifstream stream(filename.data());
      rapidjson::IStreamWrapper wrapper(stream);
      doc.ParseStream(wrapper);
      auto member = doc.MemberBegin();
      auto thePrefab = Deserialize(member->name.GetString(), member->value);

      return thePrefab.second;
  }



} // End Of Dapper Namespace