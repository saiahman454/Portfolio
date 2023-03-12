//------------------------------------------------------------------------------
//
// File Name:	TexturehManager.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "TextureManager.h"
#include <stdexcept>
#include <fstream>
#include <iostream>

// Dapper Namespace!
namespace Dapper
{
  TextureManager::TextureManager(Engine& engine) : the_engine(engine)
  {
  }

  TextureManager::~TextureManager()
  {
  }

  void TextureManager::Initialize()
  {
    //register the deserialize function
    dynamic_cast<Deserializer*>(the_engine.Get("Deserializer"))->
      RegisterFunction("Texture", TextureDeserialize);
    texture_list.reserve(200);//Isiah Genius DuctTape fix all
  }

  void TextureManager::Update(float dt)
  {
    dt;
  }

  void TextureManager::Render()
  {
  }

  int TextureManager::GetNew()
  {
    Texture texture;
    int capacity = texture_list.capacity();
    int size = texture_list.size();
    int obj_ID = ID_manager.Create(texture_list, texture);
    texture_list[ID_manager[obj_ID]].SetID(obj_ID);
    if (capacity == size)
    {
      on_reallocate.SendMessage(*this);
    }
    return obj_ID;
    /*int ID = texture_list.size();
    Texture texture(ID);
    texture_list.push_back(texture);
    return ID;*/
  }

  Texture& TextureManager::operator[](std::string texture_name)
  {

    for (Texture& the_texture : texture_list)
    {
      if (the_texture.GetFileLocation() == texture_name)
      {
        return the_texture;
      }
    }
    throw std::out_of_range("Texture name does not exist");
  }

  std::pair<Texture*, int> TextureManager::HasTexture(std::string texture_name)
  {
    for (Texture& the_texture : texture_list)
    {
      if (the_texture.GetFileLocation() == texture_name)
      {
        return { &the_texture, the_texture.GetID() };
      }
    }

    return { nullptr, -1 };
  }

  void TextureManager::LoadNewTexture(std::string file_path)
  {
    std::ifstream pfile(file_path);
    if (!pfile)
    {
      std::cout << "Failed to find" << file_path << "\n";
      return;
    }
    for (Texture& the_texture : texture_list)
    {
      if (the_texture.GetFileLocation() == file_path)
      {
        return;
      }
    }
    int pos = GetNew();
    texture_list[pos].SetFileLocation(file_path);
    texture_list[pos].LoadTextureA();
  }

  int TextureManager::Search(std::string search_name)
  {
    for (int i = 0;i < (int)texture_list.size(); i++)
    {
      if (search_name == texture_list[i].GetFileLocation().substr(17, search_name.length()))
      {
        return i;
      }
    }

    return -1;
  }

  void TextureManager::RegisterFunction(Messenger<TextureManager&>::func function)
  {
    on_reallocate.RegisterMessage(function);
  }

  bool TextureManager::UpdateWhilePaused() const
  {
      return false;
  }

  std::pair<std::string_view, int> TextureDeserialize(Engine& engine, const rapidjson::Value& val)
  {
    TextureManager* manager = dynamic_cast<TextureManager*>(engine.Get("TextureManager"));
    Deserializer* deserial = dynamic_cast<Deserializer*>(engine.Get("Deserializer"));

    auto texture = manager->HasTexture(val.GetString());

    if (texture.first)
    {
      return { texture.first->GetName(), texture.second };
    }
    else
    {
      int index = manager->GetNew();
      Texture& new_texture = (*manager)[index];
      new_texture.Read(*deserial, val);
      new_texture.LoadTextureA(); //&&&Adam Need condition for non Alpha
      return { new_texture.GetName(), index };
    }

  }

} // End Of Dapper Namespace