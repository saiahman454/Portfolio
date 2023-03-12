//------------------------------------------------------------------------------
//
// File Name:	MeshManager.cpp
// Author(s):	Adam Tackett (Luke Mostert)
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "MeshManager.h"

// Dapper Namespace!
namespace Dapper 
{
  MeshManager::MeshManager(Engine& engine): the_engine(engine)
    {
    }

    MeshManager::~MeshManager()
    {
    }

    void MeshManager::Initialize()
    {
        //register the deserialize function
        dynamic_cast<Deserializer*>(the_engine.Get("Deserializer"))->
            RegisterFunction("Mesh", MeshDeserialize);
    }

    void MeshManager::Update(float dt)
    {
        dt;
    }

    void MeshManager::Render()
    {
    }

    int MeshManager::GetNew()
    {
      int ID = (int)mesh_list.size();
      Mesh mesh(ID, the_engine);
      int capacity = mesh_list.capacity();
      int size = mesh_list.size();
      int obj_ID = ID_manager.Create(mesh_list, std::move(mesh));
      if (capacity == size)
      {
        on_reallocate.SendMessage(*this);
      }
      return obj_ID;
    }

    Mesh& MeshManager::operator[](std::string mesh_name)
    {

      for (Mesh& the_mesh : mesh_list)
      {
        if (the_mesh.GetName() == mesh_name)
        {
          return the_mesh;
        }
      }
      throw std::out_of_range("Mesh name does not exist");
    }

    std::pair<Mesh*, int> MeshManager::HasMesh(std::string mesh_name)
    {
      int count = 0;
      for (Mesh& the_mesh : mesh_list)
      {  
        if (the_mesh.GetName() == mesh_name)
        {
          return { &the_mesh, count };
        }
        count++;
      }

      return { nullptr, -1 };
    }

    void MeshManager::RegisterFunction(Messenger<MeshManager&>::func function)
    {
      on_reallocate.RegisterMessage(function);
    }

    bool MeshManager::UpdateWhilePaused() const
    {
        return false;
    }

    std::pair<std::string_view, int> MeshDeserialize(Engine& engine, const rapidjson::Value& val)
    {
      MeshManager* manager = dynamic_cast<MeshManager*>(engine.Get("MeshManager"));
      Deserializer* deserial = dynamic_cast<Deserializer*>(engine.Get("Deserializer"));

      auto mesh = manager->HasMesh(val.GetString());

      if (mesh.first)
      {
        return { mesh.first->GetTypeName(), mesh.second };
      }
      else 
      {
        int index = manager->GetNew();
        Mesh& new_mesh = (*manager)[index];
        new_mesh.ReadFromFile(*deserial,"Data/" + std::string(val.GetString()) + ".json");
        return { new_mesh.GetTypeName(), index };
      }
    } 

} // End Of Dapper Namespace