//------------------------------------------------------------------------------
//
// File Name:	SpriteSourceManager.cpp
// Author(s):	Natalie Bouley
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "SpriteSourceManager.h"
#include "ComponentRegistry.h"
#include "Utility.h"
#include <stdexcept>

// Dapper Namespace!
namespace Dapper
{
  SpriteSourceManager::SpriteSourceManager(Engine& engine1)
    : engine(engine1)
  {
  }

  SpriteSourceManager::~SpriteSourceManager()
  {
  }

  const std::string& SpriteSourceManager::Name() const
  {
    return name;
  }

  void SpriteSourceManager::Initialize()
  {
    // Register the proper functions.
    ComponentRegistry::FunctionBuffer SpriteSourceFunctions
    { SpriteSourceCreate, SpriteSourceDestroy, SpriteSourceGet };

    TextureManager* texture_manager = GetSystemFromEngine<TextureManager*>(engine, "TextureManager");
    texture_manager->RegisterFunction(std::bind(&SpriteSourceManager::OnTextureReallocation, this, std::placeholders::_1));

    dynamic_cast<Deserializer*>(engine.Get("Deserializer"))->
      RegisterFunction("SpriteSource", SpriteSourceDeserialize);
    dynamic_cast<ComponentRegistry*>(engine.Get("ComponentRegistry"))->
      Register("SpriteSource", SpriteSourceFunctions);
    spritesources.reserve(200);//Isiah Genius DuctTape fix all
  }

  void SpriteSourceManager::Update(float dt)
  {
    dt;
  }

  void SpriteSourceManager::Render()
  {
    // Go through the vector and render every spritesource.
    int spritesLength = (int)spritesources.size();
    for (int i = 0; i < spritesLength; i++)
    {
      spritesources[i].Render(engine);
    }
  }

  void SpriteSourceManager::Destroy(int ID)
  {
    ID_manager.Destroy(spritesources, ID);
  }

  int SpriteSourceManager::GetNew()
  {
    bool realloc = false;
    if (spritesources.size() == spritesources.capacity())
    {
      realloc = true;
    }
    SpriteSource new_obj(&engine);
    int obj_ID = ID_manager.Create(spritesources, std::move(new_obj));
    spritesources[ID_manager[obj_ID]].SetID(obj_ID);
    
    if (realloc)
    {
      for (auto& source : spritesources)
      {
        //source.onReallocate.SendMessage(&source);
      }
    }

    return obj_ID;
  }

  SpriteSource& SpriteSourceManager::operator[](std::string spritesource_name)
  {
    // Go through the manager and check for the sprite source's name
    for (SpriteSource& spritesource : spritesources)
    {
      if (spritesource.GetName() == spritesource_name)
      {
        return spritesource;
      }
    }
    throw std::out_of_range("SpriteSource name does not exist!");
  }

  SpriteSource& SpriteSourceManager::operator[](int ID)
  {
    return spritesources[ID_manager[ID]];
  }

  std::pair<SpriteSource*, int> SpriteSourceManager::HasSpriteSource(std::string sprite_source)
  {
    return std::pair<SpriteSource*, int>();
  }

  bool SpriteSourceManager::UpdateWhilePaused() const
  {
      return false;
  }

  void SpriteSourceManager::OnTextureReallocation(TextureManager& texture_manager)
  {
    int size = spritesources.size();
    for (int i = 0; i < size; ++i)
    {
      spritesources[i].OnTextureReallocation(texture_manager);
    }
  }

  std::pair<std::string_view, int> SpriteSourceDeserialize(Engine& engine, const rapidjson::Value& val)
  {
    // Grab the spritesource manager, new spritesource, and deserializer
    SpriteSourceManager& manager = *dynamic_cast<SpriteSourceManager*>(engine.Get("SpriteSourceManager"));
    int index = manager.GetNew();
    SpriteSource& spritesource = manager[index];
    Deserializer& deserializer = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));

    // Read the sprite.
    spritesource.Read(deserializer, val);
    return { spritesource.GetName(), index };
  }

  SpriteSource* SpriteSourceCreate(Engine& engine)
  {
    SpriteSourceManager& the_man = *dynamic_cast<SpriteSourceManager*>(engine.Get("SpriteSourceManager"));
    int index = the_man.GetNew();
    return &the_man[index];
  }

  void SpriteSourceDestroy(int ID, Engine& engine)
  {
    // Destroy the sprite at the specified location.
    SpriteSourceManager& the_man = *dynamic_cast<SpriteSourceManager*>(engine.Get("SpriteSourceManager"));
    the_man.Destroy(ID);
  }

  SpriteSource* SpriteSourceGet(int ID, Engine& engine)
  {
    // Return the sprite at the specified location.
    SpriteSourceManager& the_man = *dynamic_cast<SpriteSourceManager*>(engine.Get("SpriteSourceManager"));
    return &the_man[ID];
  }
} // End Of Dapper Namespace