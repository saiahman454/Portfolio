//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.cpp
// Author(s):	
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Deserializer.h"
#include "LevelEditor.h"
#include "Utility.h"
#include "GameObjectListBox.h"
#include "ComponentRegistry.h"
#include <algorithm> // std::sort
#include <string_view>


// Dapper Namespace!
namespace Dapper
{
  /*--------------------------------------
  Description: GameObject manager constructor
  Params:
  engine_new - the parent engine for the tile
  ---------------------------------------*/
  GameObjectManager::GameObjectManager(Engine& engine_new) : my_engine(engine_new)
  {
  }
  /*--------------------------------------
  Description: GameObject manager destructor
  ---------------------------------------*/
  GameObjectManager::~GameObjectManager()
  {
  }

  /*--------------------------------------
  Description: Initalize function
  ---------------------------------------*/
  void GameObjectManager::Initialize()
  {
    dynamic_cast<Deserializer*>(my_engine.Get("Deserializer"))->RegisterFunction("GameObject", GameObjectDeserialize);
    dynamic_cast<Deserializer*>(my_engine.Get("Deserializer"))->RegisterFunction("GameObjects", GameObjectArrayDeserialize);
  }

  /*--------------------------------------
  Description: Initalize function
  Params: 
  dt - delta time
  ---------------------------------------*/
  void GameObjectManager::Update(float dt)
  {
    dt;
    int todestroy_size = toDestroyIds.size();
    if (todestroy_size > 0)
    {
        for (auto& toDestroy : toDestroyIds)
        {
            //make sure it exist, scene manager might have deleted already
            if (ID_manager.Has(toDestroy))
            {
                game_objects[ID_manager[toDestroy]].PrepDestroy();
                // Get the name of the game object
                ID_manager.Destroy(game_objects, toDestroy);
            }
        }
        toDestroyIds.clear();

        // Resort the game object vector
        sort_objects = true;
    }
    if (sort_objects == true)
    {
        SortGameObjects();
        sort_objects = false;
    }
  }

  /*--------------------------------------
  Description: Render function
  ---------------------------------------*/
  void GameObjectManager::Render()
  {
  }

  /*--------------------------------------
  Description: searches for a gameobject by name
  Params: name - the name to search with
  Return: the game object found
  ---------------------------------------*/
  GameObject& GameObjectManager::operator[](const std::string& name)
  {
    for (int i = 0; i < (int)game_objects.size(); ++i)
    {
      if (name == game_objects[i].GetName())
      {
        return game_objects[i];
      }
    }
    return game_objects[-1];
  }


  /*--------------------------------------
  Description: gives direct acssess to the vector of
  gameobjects
  Return: The gameobject pointer
  ---------------------------------------*/
  GameObject* GameObjectManager::GetObjectAtIndex(int index)
  {
    if (index < game_objects.size())
    {
      return &game_objects[index];
    }
    return nullptr;
  }


  /*--------------------------------------
  Description: Makes a new game object
  Return: the Id of the gameobject
  ---------------------------------------*/
  int GameObjectManager::GetNew()
  {
    GameObject obj(&my_engine);

    // Register onRename message
    obj.RegisterOnObjectRename(std::bind(&GameObjectManager::OnObjectRename, this));

    int obj_ID = ID_manager.Create(game_objects, obj);
    game_objects[ID_manager[obj_ID]].SetID(obj_ID);
    //game_objects[ID_manager[obj_ID]].AddComponent("Transform", obj_ID);
    return obj_ID;
  }

  /*--------------------------------------
  Description: Makes a new game object with a name
  Params: obj_name - the name of the object
  Return: the Id of the gameobject
  ---------------------------------------*/
  int GameObjectManager::MakeNewWithName(std::string obj_name)
  {
    int ID = GetNew();
    game_objects[ID_manager[ID]].SetName(obj_name);
    ComponentRegistry& reg = *GetSystemFromEngine<ComponentRegistry*>(my_engine, "ComponentRegistry");
    IComponent* comp = reg.Construct("Transform");
    if (comp)
    {
      game_objects[ID_manager[ID]].AddComponent("Transform", comp->GetID());
      comp->SetParentID(game_objects[ID_manager[ID]].GetID());
    }
    return ID;
  }

  /*--------------------------------------
  Description: Deletes a game object with the given ID
  Params: ID - the id you want to delete
  ---------------------------------------*/
  void GameObjectManager::DeleteGameObject(int ID)
  {
    toDestroyIds.push_back(ID);
    //game_objects[ID_manager[ID]].PrepDestroy();
    //ID_manager.Destroy(game_objects, ID);
  }

  /*--------------------------------------
  Description: deletes all gameobjects
  ---------------------------------------*/
  void GameObjectManager::DeleteAllGameObjects()
  {
    for (int i = (int)game_objects.size() - 1; i > -1; --i)
    {
      int id = game_objects[i].GetID();
      DeleteGameObject(id);
    }
  }

  /*--------------------------------------
  Description: writes all the gameobjects to a file
  Params: writer -  custom json writer
  ---------------------------------------*/
  void GameObjectManager::WriteAllGameObjects(IJsonWriter& writer)
  {
    if (game_objects.size() > 1)
    {
      writer.WriteKey("GameObjects");
      writer.StartArray();
      for (GameObject& current : game_objects)
      {
        if (current.GetName() != "Health")
        {
          current.Write(writer);
        }
      }
      writer.EndArray();
    }
  }

  /*--------------------------------------
  Description: gets the gameobject vector
  Return: the vector of gameobjects
  ---------------------------------------*/
  const std::vector<GameObject>& GameObjectManager::GetAllGameObjects()
  {
    return game_objects;
  }

  /*--------------------------------------
  Description: Makes a new game object using a prefab
  prefab_name - the name of the prefab
  Return: the Id of the gameobject
  ---------------------------------------*/
  int GameObjectManager::MakePrefabWithName(std::string_view prefab_name)
  {
    
    std::string filename = "Data/Prefabs/" + std::string(prefab_name) + ".json";
    std::string name = NameObjectFromPrefab(prefab_name);

    Deserializer& deserial = *GetSystemFromEngine<Deserializer*>(my_engine, "Deserializer");
    int obj_ID = deserial.DeserializePrefab(filename);

    game_objects[ID_manager[obj_ID]].SetName(name);

    return obj_ID;
  }

  /*--------------------------------------
  Description: Makes a new game object
  Return: the Id of the gameobject
  ---------------------------------------*/
  int GameObjectManager::FindIndex(const std::string& name)
  {
    for (int i = 0; i < (int)game_objects.size(); ++i)
    {
      if (name == game_objects[i].GetName())
      {
        return i;
      }
    }
    return -1;
  }

  int GameObjectManager::FindObjectWithTag(std::string_view tag) const
  {
      for (const auto& obj : game_objects)
      {
          if (obj.GetFlag() == tag)
          {
              return obj.GetID();
          }
      }
      return -1;
  }

  /*--------------------------------------
  Description: Sorts the game object vector
  Return: None
  ---------------------------------------*/
  void GameObjectManager::SortGameObjects()
  {
      std::sort(game_objects.begin(), game_objects.end(), 
          [](const GameObject& object1, const GameObject& object2) 
          {
              return (object1.GetName() < object2.GetName());
          }
      );
      ID_manager.Resort(game_objects);
  }

  /*--------------------------------------
  Description: Sets sort object bool to true
  Return: None
  ---------------------------------------*/
  void GameObjectManager::OnObjectRename()
  {
      sort_objects = true;
  }

  void GameObjectManager::SetGameObjectPauseState(bool Condition, int iD)
  {
      game_objects[ID_manager[iD]].SetPause(Condition);    
  }

  bool GameObjectManager::HasObject(int gameObject) const
  {
      return ID_manager.Has(gameObject);
  }

  std::string GameObjectManager::NameObjectFromPrefab(std::string_view prefab_name)
  {
      std::string name = "";

      // Automatically name the game object
      int vec_length = game_objects.size();
      int largest_name_value = 0;
      for(GameObject const& object : game_objects)
      {
          // Find largest value at the end of the names that match the prefab name
          const std::string& object_name = object.GetName();
          if (object_name.find(prefab_name) == 0)
          {
              int name_value = atoi(object_name.c_str() + prefab_name.size());
              if (name_value > largest_name_value) largest_name_value = name_value;
          }
          else if (largest_name_value) break;
      }
      largest_name_value++;
      name += prefab_name;
      name += std::to_string(largest_name_value);
      return name;
  }

  /*--------------------------------------
  Description: deseializes one gameobject
  engine - the parent engine
  val - the json object
  Return: a pair with the name and id of the object
  ---------------------------------------*/
  std::pair<std::string_view, int> GameObjectDeserialize(Engine& engine, const rapidjson::Value& val)
  {
    GameObjectManager& the_man = *dynamic_cast<GameObjectManager*>(engine.Get("GameObjectManager"));
    int index = the_man.GetNew();
    GameObject& obj = the_man[index];
    Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
    obj.Read(des, val);
    obj.Initialize();
    return { obj.GetTypeName(), index };

  }

  /*--------------------------------------
  Description: deseializes a array gameobjects
  engine - the parent engine
  val - the json object
  Return: a pair with the name and id of the object
  ---------------------------------------*/
  std::pair<std::string_view, int> GameObjectArrayDeserialize(Engine& engine, const rapidjson::Value& val)
  {
    GameObjectManager& the_man = *dynamic_cast<GameObjectManager*>(engine.Get("GameObjectManager"));
    Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
    auto obj_array = val.GetArray();
    int len = obj_array.Size();
    int obj_man_size = the_man.Size();
    for (int i = 0; i < len; ++i)
    {
      auto obj_json = obj_array[i].GetObj();
      int index = the_man.GetNew();
      GameObject& obj = the_man[index];
      obj.Read(des, obj_json);
      //obj.Initialize();
    }
    for (int i = obj_man_size; i < the_man.Size(); ++i)
    {
        the_man.GetObjectAtIndex(i)->Initialize();
    }
    return { "GameObjects", -1 };
  }

} // End Of Dapper Namespace