//------------------------------------------------------------------------------
//
// File Name:	Stub.cpp
// Author(s):	
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Stub.h"
#include "TotalCollectable.h"
#include "Utility.h"
#include "SceneManager.h"
#include "SpriteText.h"
#include <fstream>
#include "rapidjson/istreamwrapper.h"

// Dapper Namespace!
namespace Dapper {



  TotalCollectable::TotalCollectable(Engine* engine) :
    my_engine(engine), ID(-1), my_parent_id(-1)
  {
  }

  void TotalCollectable::Init()
  {

  }

  void TotalCollectable::Update(float dt)
  {
    SceneManager& scenes = *GetSystemFromEngine< SceneManager*>(*my_engine, "SceneManager");
    if (scenes.IsSceneLoaded("LevelPassed1"))
    {
      file_path = "./data/Level1Data.json";
      level = 1;
    }
    if (scenes.IsSceneLoaded("LevelPassed2"))
    {
      file_path = "./data/Level2Data.json";
      level = 2;
    }
    if (scenes.IsSceneLoaded("WinGame"))
    {
      file_path = "./data/Level3Data.json";
      level = 3;
    }
    rapidjson::Document doc;
    std::ifstream stream(file_path);
    rapidjson::IStreamWrapper wrapper(stream);
    doc.ParseStream(wrapper);
    SpriteText& text = *GetComponentFromParent<SpriteText>(*my_engine, my_parent_id, "SpriteText");
    int collected = doc.MemberBegin()->value.GetInt();
    std::string info;
    switch (level)
    {
    case(1):
    {
      info = std::to_string(collected) + "/69";
      break;
    }
    case(2):
    {
      info = std::to_string(collected) + "/39";
      break;
    }
    case(3):
    {
      info = std::to_string(collected) + "/45";
      break;
    }
    default:
      info = "";
      break;
    }
    text.SetText(info);

  }

  int TotalCollectable::GetParentID() const
  {
    return my_parent_id;
  }

  void TotalCollectable::SetParentID(int ID)
  {
    my_parent_id = ID;
  }

  const std::string& TotalCollectable::GetName() const
  {
    return name;
  }

  void TotalCollectable::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
  }

  void TotalCollectable::SetID(int ID_)
  {
    ID = ID_;
  }

  void TotalCollectable::Write(IJsonWriter& writer) const
  {
    writer.WriteKey("TotalCollectable");
    writer.StartObject();
    writer.EndObject();
  }

  void TotalCollectable::ImGuiDisplay(float scale)
  {
  }

  const int TotalCollectable::GetID() const
  {
    return ID;
  }

  void TotalCollectable::SetPause(bool Condition)
  {
  }

  bool TotalCollectable::GetPause() const
  {
    return false;
  }


} // End Of Dapper Namespace