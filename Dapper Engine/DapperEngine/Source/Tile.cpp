//------------------------------------------------------------------------------
//
// File Name:	Tile.cpp
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Tile.h"
#include "Utility.h"
#include "Transform.h"
#include "SquareCollider.h"
#include "LevelEditor.h"

// Dapper Namespace!
namespace Dapper {

  /*--------------------------------------
  Description: Tile constructor
  Params:
  engine - the parent engine for the tile
  ---------------------------------------*/
  Tile::Tile(Engine* engine) :my_engine(engine)
  {
  }
  /*--------------------------------------
  Description: Tile destructor
  ---------------------------------------*/
  Tile::~Tile()
  {
  }
  /*--------------------------------------
  Description: initalize function
  ---------------------------------------*/
  void Tile::Init()
  {
  }


  /*--------------------------------------
  Description: Tile update function checks for
  customs uv changes.
  ---------------------------------------*/
  void Tile::Update(float dt)
  {
    if (didchange)
    {
      SpriteSource& source = *GetComponentFromParent<SpriteSource>(*my_engine, parent_ID, "SpriteSource");
      source.SetUVCoordinates(tile_ID);
      glm::vec2 min = source.GetUVAtFrameIndex(tile_ID);
      glm::vec2 UV_size = source.GetUVSize();
      glm::vec2 max = min + UV_size;
      glm::vec2 total_size = dimension * UV_size;
      source.SetCustomUV({ total_size.x, min.x }, { total_size.y, min.y }, min, max);
      didchange = false;

    }
  }


  /*--------------------------------------
  Description: Tile read function
  Params: 
  deserial - the desesializer (unused)
  jsonVal -  the json object to read from file
  ---------------------------------------*/
  void Tile::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial;
    auto object = jsonVal.GetObject();
    tile_ID = object["TileID"].GetInt();
    auto dim = object["Dimension"].GetArray();
    dimension.x = dim[0].GetFloat();
    dimension.y = dim[1].GetFloat();
  }

  /*--------------------------------------
  Description: Tile write function
  Params: Writer - customs writer to write to
  ---------------------------------------*/
  void Tile::Write(IJsonWriter& writer) const
  {
    writer;
    writer.WriteKey("Tile");
    writer.StartObject();
    writer.WriteInt("TileID", tile_ID);
    writer.WriteVec2("Dimension", dimension.x, dimension.y);
    writer.EndObject();
  }

  /*--------------------------------------
  Description: Draws the cyan grid over a tile sheet
  Params: 
  size - size of the grid and image
  rows - number of rows
  cols - number of columns
  offset -  the offset of the window
  ---------------------------------------*/
  void DrawGrid(float size, int rows, int cols, ImVec2 offset)
  {
    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 off = { offset.x + window_pos.x ,offset.y + window_pos.y };
    auto thingy = ImGui::GetWindowDrawList();
    for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; j++)
      {
        ImVec2 pos(size * i, size * j);
        
        thingy->AddRect(
          { pos.x + off.x,pos.y + off.y },
          { (pos.x + size) + off.x ,(pos.y + size) + off.y },
          IM_COL32(0, 255, 255, 255));
      }
    }
  }

  /*--------------------------------------
  Description: Display for level editor
  Params: scale - resolusion size
  ---------------------------------------*/
  void Tile::ImGuiDisplay(float scale)
  {
    ImVec2 cursor = ImGui::GetCursorPos();
    ImVec2 button_size{ 200 * scale,35 * scale };
    if (ImGui::InputInt("TileID", &tile_ID, 1))
    {
      didchange = true;
    }

    if (ImGui::Button("Dimention X +", button_size))
    {
      IncreaseX();
      didchange = true;
    }
    ImGui::SameLine(button_size.x + cursor.x + (4 * scale));
    if (ImGui::Button("Dimention Y +", button_size))
    {
      IncreaseY();
      didchange = true;
    }
    ImGui::PushItemWidth(404 * scale);
    if (ImGui::InputFloat2("Dimensions", &dimension.x))
    {
      didchange = true;
    }
    ImGui::PopItemWidth();
    if (ImGui::Button("Dimention X -", button_size))
    {
      DecreaseX();
      didchange = true;
    }
    ImGui::SameLine(button_size.x + cursor.x + (4 * scale));
    if (ImGui::Button("Dimention Y -", button_size))
    {
      DecreaseY();
      didchange = true;
    }

    if (ImGui::Button("Select Tile", { scale * 300, scale * 50 }))
    {
      if (tile_set_bool)
      {
        tile_set_bool = false;
      }
      else
      {
        tile_set_bool = true;
      }
    }

    if (tile_set_bool)
    {
      ImGui::SetWindowFontScale(2.0f * scale);
      ImGui::Begin("Tile Select", &tile_set_bool);
      ImGui::SetWindowSize({ 550 * scale,550 * scale });

      SpriteSource& source = *GetComponentFromParent<SpriteSource>(*my_engine, parent_ID, "SpriteSource");
      int cols = source.GetCols();
      int rows = source.GetRows();

      TextureManager& tex_man = *GetSystemFromEngine<TextureManager*>(*my_engine, "TextureManager");
      Texture& texture = tex_man[source.GetTexture()];
      ImVec2 img_size(512 * scale, 512 * scale);
      ImVec2 temp_name = makenumber(img_size.x, img_size.y);
      ImGui::SetCursorPos(temp_name);
      ImGui::Image(static_cast<char*>(0) + texture.GetOpenGLID(), img_size);

      ImVec2 win_pos = ImGui::GetWindowPos();
      DrawGrid((512 / rows) * scale, rows, cols, temp_name);
      ImVec2 test = ImGui::GetMousePos();
      if (test.x > temp_name.x + win_pos.x && test.x < temp_name.x + img_size.x + win_pos.x)
      {
        if (test.y > temp_name.y + win_pos.y && test.y < temp_name.y + img_size.y + win_pos.y)
        {
          LevelEditor& level = *GetSystemFromEngine<LevelEditor*>(*my_engine, "LevelEditor");
          const std::vector<mouse_button>& gui_input = level.GetGuiMouseInput();
          if (gui_input[GLFW_MOUSE_BUTTON_LEFT] == mouse_button::MOUSE_TRIGGERED)
          {
            int poggers = 8 * floor((test.y - win_pos.y) / ((512 / rows) * scale));
            poggers += floor((test.x - win_pos.x) / ((512 / rows) * scale));
            tile_ID = poggers;
            didchange = true;
          }
          auto thingy = ImGui::GetWindowDrawList();
          ImVec2 pos(floor((test.x - win_pos.x -(32*scale)) / ((512*scale) / rows)), floor((test.y - win_pos.y - (32 * scale)) / ((512*scale) / rows)));
          thingy->AddRect(
            { temp_name.x + (pos.x*64*scale) + win_pos.x ,temp_name.y + (pos.y * 64 * scale) + win_pos.y },
            { ((pos.x * 64 * scale) + (64*scale)) + temp_name.x + win_pos.x,((pos.y*64 * scale) + (64 * scale)) + temp_name.y + win_pos.y },
            IM_COL32(255, 0, 0, 255));
        }

      }
      ImGui::End();
    }

  }

  /*--------------------------------------
  Description: Temporary math function
  Params: 
  width - width
  hieght - hieght
  ---------------------------------------*/
  ImVec2 Tile::makenumber(float width, float hieght)
  {
    ImVec2 img_size = { width,hieght };
    ImVec2 wind_size = ImGui::GetWindowSize();
    return { (wind_size.x - img_size.x) * 0.5f, (wind_size.y - img_size.y) * 0.5f };

  }

  /*--------------------------------------
  Description: increases tile size horazontaly
  ---------------------------------------*/
  void Tile::IncreaseX()
  {
    Transform& trans = *GetComponentFromParent<Transform>(*my_engine, parent_ID, "Transform");
    glm::vec2 size = trans.GetScale();
    trans.setScale({ size.x + 64.0f, size.y });
    glm::vec3 pos = trans.GetTranslation();
    trans.setTranslation({ pos.x + 32.0f,pos.y , pos.z });
    SquareCollider* collider = GetComponentFromParent<SquareCollider>(*my_engine, parent_ID, "SquareCollider");
    if (collider)
    {
      glm::vec2 colid_size = collider->GetSize();
      collider->SetSize({ colid_size.x + 64.0f, colid_size.y });
    }
    dimension.x += 1;
    didchange = true;
  }
  /*--------------------------------------
  Description: increases tile size vertically
  ---------------------------------------*/
  void Tile::IncreaseY()
  {
    Transform& trans = *GetComponentFromParent<Transform>(*my_engine, parent_ID, "Transform");
    glm::vec2 size = trans.GetScale();
    trans.setScale({ size.x , size.y + 64.0f });
    glm::vec3 pos = trans.GetTranslation();
    trans.setTranslation({ pos.x ,pos.y - 32.0f, pos.z });
    SquareCollider* collider = GetComponentFromParent<SquareCollider>(*my_engine, parent_ID, "SquareCollider");
    if (collider)
    {
      glm::vec2 colid_size = collider->GetSize();
      collider->SetSize({ colid_size.x , colid_size.y + 64.0f });
    }
    dimension.y += 1;
    didchange = true;
  }

  /*--------------------------------------
  Description: decreases tile size horazontaly
  ---------------------------------------*/
  void Tile::DecreaseX()
  {
    Transform& trans = *GetComponentFromParent<Transform>(*my_engine, parent_ID, "Transform");
    glm::vec2 size = trans.GetScale();
    trans.setScale({ size.x - 64.0f, size.y });
    glm::vec3 pos = trans.GetTranslation();
    trans.setTranslation({ pos.x - 32.0f,pos.y , pos.z });
    SquareCollider* collider = GetComponentFromParent<SquareCollider>(*my_engine, parent_ID, "SquareCollider");
    if (collider)
    {
      glm::vec2 colid_size = collider->GetSize();
      collider->SetSize({ colid_size.x - 64.0f, colid_size.y });
    }
    dimension.x -= 1;
    didchange = true;
  }
  /*--------------------------------------
  Description: decreases tile size vertically
  ---------------------------------------*/
  void Tile::DecreaseY()
  {
    Transform& trans = *GetComponentFromParent<Transform>(*my_engine, parent_ID, "Transform");
    glm::vec2 size = trans.GetScale();
    trans.setScale({ size.x , size.y - 64.0f });
    glm::vec3 pos = trans.GetTranslation();
    trans.setTranslation({ pos.x ,pos.y + 32.0f, pos.z });
    SquareCollider* collider = GetComponentFromParent<SquareCollider>(*my_engine, parent_ID, "SquareCollider");
    if (collider)
    {
      glm::vec2 colid_size = collider->GetSize();
      collider->SetSize({ colid_size.x , colid_size.y - 64.0f });
    }
    dimension.y -= 1;
    didchange = true;
  }
} // End Of Dapper Namespace