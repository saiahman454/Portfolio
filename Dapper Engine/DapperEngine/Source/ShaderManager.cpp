//------------------------------------------------------------------------------
//
// File Name:	ShaderManager.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "ShaderManager.h"
#include <stdexcept>

// Dapper Namespace!
namespace Dapper 
{
    ShaderManager::ShaderManager(Engine& engine): the_engine(engine)
    {
    }

    ShaderManager::~ShaderManager()
    {
    }

    void ShaderManager::Initialize()
    {
        //register the deserialize function
        dynamic_cast<Deserializer*>(the_engine.Get("Deserializer"))->
            RegisterFunction("Shader", ShaderDeserialize);
    }

    void ShaderManager::Update(float dt)
    {
        dt;
    }

    void ShaderManager::Render()
    {
    }

    int ShaderManager::GetNew()
    {
      int ID = (int)shader_list.size();
      Shader shader(ID);
      shader_list.push_back(std::move(shader));
      return ID;
    }

    Shader& ShaderManager::operator[](std::string shader_name)
    {

      for (Shader& the_shader : shader_list)
      {

        if (the_shader.GetName() == shader_name)
        {
          return the_shader;
        }
      }
      throw std::out_of_range("Shader name does not exist");
    }

    std::pair<Shader*, int> ShaderManager::HasShader(std::string shader_name)
    {
      int count = 0;
      for (Shader& the_Shader : shader_list)
      {
        
        if (the_Shader.GetFileName() == shader_name)
        {
          return { &the_Shader, count };
        }
        count++;
      }

      return { nullptr, -1 };
    }

    bool ShaderManager::UpdateWhilePaused() const
    {
        return false;
    }

    std::pair<std::string_view, int> ShaderDeserialize(Engine& engine, const rapidjson::Value& val)
    {
        ShaderManager* manager = dynamic_cast<ShaderManager*>(engine.Get("ShaderManager"));
        Deserializer* deserial = dynamic_cast<Deserializer*>(engine.Get("Deserializer"));

        auto shader = manager->HasShader(val.GetString());

        if (shader.first)
        {
            return { shader.first->GetName(), shader.second };
        }
        else
        {
            int index = manager->GetNew();
            Shader& new_shader = (*manager)[index];
            new_shader.Read(*deserial, val);
            // + operator returns a new string
            new_shader.CreateFromFiles((new_shader.GetFileName() + ".vert").c_str(),
                (new_shader.GetFileName() + ".frag").c_str());
            return { new_shader.GetName(), index };
        }
    }

} // End Of Dapper Namespace