//------------------------------------------------------------------------------
//
// File Name:	SpriteSource.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "SpriteSource.h"
#include "Renderer.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Utility.h"
#include "glm/common.hpp"

// Dapper Namespace!
namespace Dapper {

  SpriteSource::SpriteSource(Engine* engine)
    : my_engine(engine)
    , rows(1)
    , cols(1)
    , my_ID(-1)
    , parent_ID(-1)
    , texture_ID(-1)
    , hasCustoms(false)
    , customU(1, 0)
    , customV(1, 0)
  {
  }

  SpriteSource::SpriteSource(int index, Engine* engine)
    : my_engine(engine)
    , rows(1)
    , cols(1)
    , my_ID(index)
    , parent_ID(-1)
    , texture_ID(-1)
    , hasCustoms(false)
    , customU(1, 0)
    , customV(1, 0)
  {
  }

  SpriteSource::~SpriteSource()
  {
  }

  //SpriteSource::SpriteSource(SpriteSource&& spriteSource) noexcept 
  //    : my_engine(spriteSource.my_engine)
  //    , rows(spriteSource.rows)
  //    , cols(spriteSource.cols)
  //    , my_ID(spriteSource.my_ID)
  //    , parent_ID(spriteSource.parent_ID)
  //    , texture_ID(spriteSource.texture_ID)
  //    , hasCustoms(spriteSource.hasCustoms)
  //    , customU(spriteSource.customU)
  //    , customV(spriteSource.customV)
  //    //, onReallocate(std::move(spriteSource.onReallocate))
  //    , file_loc(spriteSource.file_loc)
  //    , paused(spriteSource.paused)
  //{
  //  //onReallocate.SendMessage(this);
  //}

  //SpriteSource& SpriteSource::operator=(SpriteSource&& spriteSource) noexcept
  //{
  //    my_engine = spriteSource.my_engine;
  //    rows = spriteSource.rows;
  //    cols = spriteSource.cols;
  //    my_ID = spriteSource.my_ID;
  //    parent_ID = spriteSource.parent_ID;
  //    texture_ID = spriteSource.texture_ID;
  //    hasCustoms = spriteSource.hasCustoms;
  //    customU = spriteSource.customU;
  //    customV = spriteSource.customV;
  //    //onReallocate = std::move(spriteSource.onReallocate);
  //    file_loc = std::move(spriteSource.file_loc);
  //    paused = paused;
  //    //onReallocate.SendMessage(this);
  //    return *this;
  //}

  void SpriteSource::Init()
  {
    // "ehhhhh?? uhhh... hmmmm???? hmmmm...." -Kenny
    TextureManager* texture_manager = GetSystemFromEngine<TextureManager*>(*my_engine, "TextureManager");
    if (texture_ID >= 0)
    {
      //texture = &(*texture_manager)[texture_ID];
    }
  }

  void SpriteSource::Update(float dt)
  {
    hasCustoms = false;
    dt;
  }

  void SpriteSource::Read(Deserializer& deserializer, const rapidjson::Value& jsonVal)
  {
    auto object = jsonVal.GetObject();
    // Read all of the necessary private variables in the class.
    //auto newTexture = object["Texture"].GetObj();

    auto textureData = deserializer.Deserialize("Texture", object["Texture"]);
    rows = object["Rows"].GetInt();
    cols = object["Cols"].GetInt();
    texture_ID = textureData.second;

    deserializer; // Unreferenced parameter
  }

  void SpriteSource::Write(IJsonWriter& writer) const
  {
    TextureManager& tex_man = *GetSystemFromEngine<TextureManager*>
          (*my_engine, "TextureManager");
    Texture& theTex = tex_man[texture_ID];
    writer.WriteKey("SpriteSource");
    writer.StartObject();
    writer.WriteString("Texture", theTex.GetFileLocation().c_str());
    writer.WriteInt("Rows", rows);
    writer.WriteInt("Cols", cols);
    writer.EndObject();
  }

  void SpriteSource::Render(Engine& engine)
  {
    engine;
  }

  void SpriteSource::SetUVCoordinates(unsigned frame_index)
  {
    float u_size = 1.0f / cols;
    float v_size = 1.0f / rows;
    customU = glm::vec2(u_size, float(frame_index % cols) / cols);
    customV = glm::vec2(v_size, floor(frame_index / cols) / rows);

    hasCustoms = true;
  }

  glm::vec2 SpriteSource::GetUVAtFrameIndex(unsigned frame_index)
  {
    float test = float(frame_index % cols) / float(cols);
    // test = glm::round(test);
    float test_y = float(frame_index / cols) / float(rows);
    return {test, test_y };
  }

  std::pair<glm::vec2, glm::vec2> SpriteSource::CalculateUVWithIndex(unsigned index) const
  {
      float u_size = 1.0f / cols;
      float v_size = 1.0f / rows;
      glm::vec2 u = glm::vec2(u_size, float(index % cols) / cols);
      glm::vec2 v = glm::vec2(v_size, floor(index / cols) / rows);
      return std::pair<glm::vec2, glm::vec2>(u, v);
  }

  void SpriteSource::OnTextureReallocation(TextureManager& texture_manager)
  {
    if (texture_ID == -1) return;
    //texture = &texture_manager[texture_ID];
  }

  void SpriteSource::SetCustomUV(glm::vec2 u, glm::vec2 v, glm::vec2 min, glm::vec2 max)
  {
    customU = u;
    customV = v;
    custom_min_uv = min;
    custom_max_uv = max;

    hasCustoms = true;
  }

  void SpriteSource::ImGuiDisplay(float scale)
  {
    scale;
    ImGui::Text("Current file path");
    TextureManager& tex_man = *GetSystemFromEngine<TextureManager*>(*my_engine, "TextureManager");
    if (texture_ID > -1)
    {
    std::string blah = tex_man[texture_ID].GetFileLocation();
    ImGui::Text(blah.c_str());
    }
    else
    {
      ImGui::Text("No texture loaded");
    }
    ImGui::InputText("texture", &file_loc);
    if (ImGui::Button("Load New Texture"))
    {
      if (!file_loc.empty())
      {
        tex_man.LoadNewTexture("./Assets/Texture/" + file_loc + ".png");
        if (tex_man.Search(file_loc) != -1)
        {
          texture_ID = tex_man.Search(file_loc);
        }
        file_loc.clear();
      }
    }

    if (ImGui::InputInt("Rows", &rows, 1))
    {
      if (rows < 1)
      {
        rows = 1;
      }
    }
    if (ImGui::InputInt("cols", &cols, 1))
    {
      if (cols < 1)
      {
        cols = 1;
      }
    }
    ImGui::Text("Teture:ID- %i", texture_ID);

  }

} // End Of Dapper Namespace