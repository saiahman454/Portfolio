//------------------------------------------------------------------------------
//
// File Name:	Sprite.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Sprite.h"
#include "Renderer.h"
#include "GameObjectManager.h"
#include "TransformManager.h"
#include "SpriteSourceManager.h"
#include "Utility.h"
#include "imgui.h"
#include <math.h>

// Dapper Namespace!
namespace Dapper {

  /*--------------------------------------
  Description: Default Constructor
  Params:
  Return: Void
  ---------------------------------------*/
  Sprite::Sprite(Engine* engine_)
    : tint{1.0f, 1.0f, 1.0f, 1.0f}
    , meshID(-1)
    , my_ID(-1)
    , parentID(-1)
    , mesh(nullptr)
    , engine(engine_)
  {
  }

  /*--------------------------------------
  Description: Constructor with index
  Params: int index - index of the sprite.
  Return: Void
  ---------------------------------------*/
  Sprite::Sprite(int index, Engine* engine_)
    : tint{ 1.0f, 1.0f, 1.0f, 1.0f }
    , meshID(-1)
    , my_ID(index)
    , parentID(-1)
    , mesh(nullptr)
    , engine(engine_)
  {
  }

  /*--------------------------------------
    Description: Destructor
    Params:
    Return: Void
    ---------------------------------------*/
  Sprite::~Sprite()
  {
  }

  /*--------------------------------------
  Description: Move Constructor
  Params: sprite - the other sprite
  Return: Void
  ---------------------------------------*/
  Sprite::Sprite(Sprite&& sprite) noexcept
      : parentID(sprite.parentID)
      , my_ID (sprite.my_ID)
      , meshID (sprite.meshID)
      , tint (sprite.tint)
      , engine (sprite.engine)
      , spriteSource(sprite.spriteSource)
  {
      IComponent::operator=(sprite);
      sprite.parentID = 0;
      sprite.my_ID = 0;
      sprite.meshID = 0;
  }

  Sprite& Sprite::operator=(Sprite&& sprite) noexcept
  {
      IComponent::operator=(sprite);
      parentID = sprite.parentID;
      my_ID = sprite.my_ID;
      meshID = sprite.meshID;
      tint = sprite.tint;
      engine = sprite.engine;
      //spriteSource = sprite.spriteSource;
      return *this;
  }

  /*--------------------------------------
  Description: Initializes everything in the sprite.
  Params:
  Return: Void
  ---------------------------------------*/
  void Sprite::Init()
  {
    MeshManager* mesh_manager = GetSystemFromEngine<MeshManager*>(*engine, "MeshManager");
    if (meshID == -1) 
    {
      auto mersh = mesh_manager->HasMesh("SquareMesh");
      if (mersh.first)
      {
        meshID = mersh.second;
      }
    }
    mesh = &(*mesh_manager)[meshID];
    //spriteSource = GetComponentFromParent<SpriteSource>(*engine, parentID, "SpriteSource");
    //spriteSource->onReallocate.RegisterMessage([this](SpriteSource* rhs) { spriteSource = rhs; });
    //spriteSource->onReallocate.RegisterMessage(std::bind(&Sprite::OnSpriteSourceRealloc, this, std::placeholders::_1));
  }

  /*--------------------------------------
  Description: Update function
  Params: float dt - time between frames.
  Return: Void
  ---------------------------------------*/
  void Sprite::Update(float dt)
  {
    dt;
  }

  /*--------------------------------------
  Description: Reads the sprite data from a json file.
  Params:
    Deserializer& deserializer - The deserializer.
    const Value& jsonVal - The json file to be read from.
  Return: Void
  ---------------------------------------*/
  void Sprite::Read(Deserializer& deserializer, const rapidjson::Value& jsonVal)
  {
    auto object = jsonVal.GetObject();
    // Read all of the necessary private variables in the class.
    MeshManager* theMeshManager = GetSystemFromEngine<MeshManager*>
      (deserializer.GetEngine(), "MeshManager");

    auto meshInfo = deserializer.Deserialize("Mesh", object["Mesh"]);
    meshID = meshInfo.second;

    auto tint_array = object["Tint"].GetArray();
    tint.r = tint_array[0].GetFloat();
    tint.g = tint_array[1].GetFloat();
    tint.b = tint_array[2].GetFloat();
    tint.a = tint_array[3].GetFloat();
  }

  /*--------------------------------------
  Description: Writes data to a json file.
  Params: const Value& jsonVal - The json file to be written from.
  Return: Void
  ---------------------------------------*/
  void Sprite::Write(IJsonWriter& writer) const
  {
    writer.WriteKey("Sprite");
    writer.StartObject();

    MeshManager& theMeshManager = *GetSystemFromEngine<MeshManager*>(*engine, "MeshManager");
    writer.WriteString("Mesh", theMeshManager[meshID].GetName().c_str());

    writer.WriteVec4("Tint", tint.r, tint.g, tint.g, tint.a);
    writer.EndObject();
  }

  /*--------------------------------------
  Description: Renders a sprite.
  Params: Engine& engine - the engine.
  Return: Void
  ---------------------------------------*/
  void Sprite::Render(Engine& engine)
  {
     if (isEnabled == false)
     {
         return;
     }
    // Grab the transform and game object.
    GameObjectManager* gameObjectManager = dynamic_cast<GameObjectManager*>(engine.Get("GameObjectManager"));
    TransformManager* transformManager = dynamic_cast<TransformManager*>(engine.Get("TransformManager"));
    SpriteSource* spriteSource = GetComponentFromParent<SpriteSource>(engine, parentID, "SpriteSource");

    GameObject& gameObject = (*gameObjectManager)[parentID];
    int transformID = gameObject.GetComponentID("Transform");
    Transform& transform = (*transformManager)[transformID];

    // Grab the position and scale from the game object's transform.
    glm::vec3 position = transform.GetTranslation();
    glm::vec2 scale = transform.GetScale();

    // Call the renderer's draw from the engine.
    Renderer* renderer = dynamic_cast<Renderer*>(engine.Get("Renderer"));

    // If using custom UVs, draw accordingly.
    // Otherwise, draw normally.
    if (spriteSource->HasCustoms())
    {
      auto theUV = spriteSource->GetCustomUV();
      DrawData draw_data;
      draw_data.position = position;
      draw_data.scale = scale;
      draw_data.meshIndex = meshID;
      draw_data.textureID = spriteSource->GetTexture();
      draw_data.rotation = transform.GetRotation();
      draw_data.theU = theUV.first;
      draw_data.theV = theUV.second;
      draw_data.theMinUV = spriteSource->GetCustomMin();
      draw_data.theMaxUV = spriteSource->GetCustomMax();
      draw_data.theColor = { tint.r, tint.g, tint.b, tint.a };
      renderer->Draw(draw_data);
    }
    else
    {
      DrawData draw_data;
      draw_data.position = position;
      draw_data.scale = scale;
      draw_data.meshIndex = meshID;
      draw_data.textureID = spriteSource->GetTexture();
      draw_data.rotation = transform.GetRotation();
      draw_data.theColor = { tint.r, tint.g, tint.b, tint.a };
      renderer->Draw(draw_data);
    }

  }

  void Sprite::OnMeshReallocation(MeshManager& mesh_manager)
  {
    if (meshID == -1) return;
    mesh = &mesh_manager[meshID];
  }

  void Sprite::ImGuiDisplay(float scale)
  {
    scale;
    ImGui::InputFloat4("Tint", &tint[0]);
    glm::clamp(tint, 0.0f, 1.0f);
    //if (tint.r > 1)
    //{
    //  tint.r = 1;
    //}
    //if (tint.r < 0)
    //{
    //  tint.r = 0;
    //}

    //alpha = (float)fmod(alpha, 1);
  }

} // End Of Dapper Namespace