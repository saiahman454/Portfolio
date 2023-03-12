//------------------------------------------------------------------------------
//
// File Name:	BGMBehavior.cpp
// Author(s):	Gavin Lu
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "BGMBehavior.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Utility.h"
#include "SceneManager.h"
#include <iostream>

// Dapper Namespace!
namespace Dapper {
  BGMBehavior::BGMBehavior(int ID_, Engine* engine_)
    : my_ID(ID_)
    , engine(engine_),
      paused(false)
  {
  }

  BGMBehavior::~BGMBehavior()
  {
    music->Stop();
  }


  void BGMBehavior::Init()
  {

    const Audio& audio = *GetSystemFromEngine<Audio*>(*engine, "Audio");

    music = audio.GetSound(fileName);

    music->SetVolume(volume);

    music->Play();

    /*
    SceneManager& scene_man = *GetSystemFromEngine< SceneManager*>(*engine, "SceneManager");
    std::function<void(void)> func = std::bind(&BGMBehavior::Stop, this);
    scene_man.RegisterOnChange(func);
    */

  }

  void BGMBehavior::Stop()
  {

    music->Stop();

  }

  int BGMBehavior::GetParentID() const
  {
    return parent_ID;
  }

  void BGMBehavior::SetParentID(int ID)
  {
    parent_ID = ID;
  }

  const std::string& BGMBehavior::GetName() const
  {
    return name;
  }

  void BGMBehavior::Update(float dt)
  {
    dt;

  }

  void BGMBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial;
    // Read the data from the json: Speed (float), Pathpoint1 (vec2), Pathpoint2 (vec2)

    auto obj = jsonVal.GetObject();

    auto fname = obj["FileName"].GetString();

    this->fileName = fname;

    auto vol = obj["Volume"].GetFloat();
    this->volume = vol;

  }

  void BGMBehavior::Write(IJsonWriter& writer) const
  {
    // Write all of the values to the json file.
    writer.WriteKey("BGMBehavior");
    writer.StartObject();
    writer.WriteString("FileName", fileName.c_str());
    writer.WriteFloat("Volume", volume);
    writer.EndObject();
  }

  void BGMBehavior::ImGuiDisplay(float scale)
  {
    // Display editable values as float inputs.
    scale;

    if (ImGui::InputFloat("Volume", &volume, 0.05f)) {


      music->SetVolume(volume);

    }

  }

  void BGMBehavior::SetID(int ID)
  {
    my_ID = ID;
  }

  const int BGMBehavior::GetID() const
  {
    return my_ID;
  }

} // End Of Dapper Namespace