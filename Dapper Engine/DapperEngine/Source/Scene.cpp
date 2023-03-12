//------------------------------------------------------------------------------
//
// File Name:	Scene.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Scene.h"
#include "Deserializer.h"
#include "Utility.h"
#include "Input.h"
#include "GameObjectManager.h"
#include "JsonWriter.h"
#include <rapidjson/document.h>
#include<rapidjson/istreamwrapper.h>
#include <fstream>
#include <iostream>


// Dapper Namespace!
namespace Dapper {
  Scene::Scene(Engine& my_engine) :
    my_engine(&my_engine),
    theName(),
    isPaused(false)
  {
  }

  Scene::Scene(Scene&& move)noexcept :
    my_engine(move.my_engine),
    theName(std::move(move.theName)),
    isPaused(false),
    heldGameobjects(std::move(move.heldGameobjects))
  {
  }

  Scene::~Scene()
  {
    UnLoad();
  }

  const std::string& Scene::GetName() const
  {
    return theName;
  }

  void Scene::setPaused(bool condition)
  {
    isPaused = condition;
    GameObjectManager* theManager = GetSystemFromEngine<GameObjectManager*>
      (*my_engine, "GameObjectManager");

    for (int current : heldGameobjects)
    {
      if (theManager->HasObject(current))
      {
        theManager->SetGameObjectPauseState(condition, current);
      }
    }
  }

  void Scene::setEnableScene(bool condition)
  {
      isEnabled = condition;

    GameObjectManager* theManager = GetSystemFromEngine<GameObjectManager*>
      (*my_engine, "GameObjectManager");

    for (int current : heldGameobjects)
    {
      if (theManager->HasObject(current))
      {
        (*theManager)[current].setEnabled(condition);
      }
    }
  }

  void Scene::Read(std::string_view filePath)
  {
    rapidjson::Document doc;
    std::ifstream stream(filePath.data());
    if (stream.is_open())
    {
      rapidjson::IStreamWrapper wrapper(stream);
      doc.ParseStream(wrapper);
      auto member = doc.MemberBegin();
      Deserializer* theDeserialize = GetSystemFromEngine<Deserializer*>
        (*my_engine, "Deserializer");
      GameObjectManager* theManager = GetSystemFromEngine<GameObjectManager*>
        (*my_engine, "GameObjectManager");
      const std::vector<GameObject>& allObjs = theManager->GetAllGameObjects();
      int howManyObjs = allObjs.size();

      while (member != doc.MemberEnd())
      {
        theDeserialize->Deserialize(member->name.GetString(), member->value);
        ++member;
      }//end of while

      int howManyAfter = allObjs.size();
      for (int i = howManyObjs; i < howManyAfter; i++)
      {
        heldGameobjects.push_back(allObjs[i].GetID());//should be fine
      }
    }
    else
    {
      //change later
      std::cout << "Failed to read " << filePath << "!File doesn't exist.\n";
    }
    setPaused(false);//Fixes Pause issue Adam
  }

  void Scene::UnLoad()
  {
    GameObjectManager* theManager = GetSystemFromEngine<GameObjectManager*>
      (*my_engine, "GameObjectManager");
    if (!theManager)
    {
      //Handels game closing
      return;
    }

    //delete all objects in the scene
    int howMany = heldGameobjects.size();
    for (int i = 0; i < howMany; ++i)
    {
      theManager->DeleteGameObject(heldGameobjects[i]);
    }

    heldGameobjects.clear();
  }

  void Scene::SetName(std::string_view name)
  {
    theName = name;
  }

  void Scene::Save(const std::string& name)
  {
    GameObjectManager& the_man = *GetSystemFromEngine<GameObjectManager*>(*my_engine, "GameObjectManager");
    JsonWriter writer;
    writer.StartObject();
    WriteAllSceneObjects(writer);
    writer.EndObject();
    std::string file_path;
    std::string param = name == "" ? theName : name;
    file_path = "./Data/Scenes/" + param + ".json";

    writer.WriteToFile(file_path.c_str());
  }

  void Scene::Add(int obj_id)
  {
    heldGameobjects.push_back(obj_id);
  }

  void Scene::Remove(int obj_id)
  {
    for (int i = 0; i < heldGameobjects.size(); ++i)
    {
      if (heldGameobjects[i] == obj_id)
      {
        heldGameobjects.erase(heldGameobjects.begin() + i);
        return;
      }
    }
  }

  void Scene::WriteAllSceneObjects(IJsonWriter& writer)
  {
    GameObjectManager& the_man = *GetSystemFromEngine<GameObjectManager*>(*my_engine, "GameObjectManager");
    writer.WriteKey("GameObjects");
    writer.StartArray();
    for (unsigned i = 0; i < heldGameobjects.size(); ++i)
    {
      if (the_man[heldGameobjects[i]].GetFlag() != "Extra")
      {
        the_man[heldGameobjects[i]].Write(writer);
      }
    }
    writer.EndArray();
  }

  bool Scene::InChargeOfObject(int objectId)
  {
    int howBig = heldGameobjects.size();
    for (int i = 0; i < howBig; i++)
    {
      if (objectId == heldGameobjects[i])
      {
        return true;
      }
    }
    return false;
  }

  Scene& Scene::operator=(Scene&& move)noexcept
  {
    theName = std::move(move.theName);
    isPaused = move.isPaused;
    heldGameobjects = std::move(move.heldGameobjects);
    return *this;
  }

} // End Of Dapper Namespace