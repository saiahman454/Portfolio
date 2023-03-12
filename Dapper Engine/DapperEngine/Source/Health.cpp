//------------------------------------------------------------------------------
//
// File Name:	Health.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Health.h"
#include "ComponentRegistry.h"
#include "Utility.h"
#include "UIManager.h"
#include "imgui.h"
#include "SceneManager.h"

// Dapper Namespace!
namespace Dapper
{
  Health::Health(Engine* theEngine) :
    engine(theEngine),
    healthCounter(3),
    maxHealth(3),
    paused(false)
  {
  }

  Health::~Health()
  {
    while (heartTracker.size())
    {
      DecreaseHealth();
    }
  }

  void Health::Init()
  {
    //healthCounter = 3;
    //maxHealth = 5;
    tempINIT = true;
    //Allocating new array breaks component TEMP fix
  }

  void Health::Update(float dt)
  {
      SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(*engine, "SceneManager");
      std::string level;
      if (scene_man.IsSceneLoaded("Level1"))
      {
        level = "Level1";
      }
      else if (scene_man.IsSceneLoaded("Level2"))
      {
        level = "Level2";
      }
      else if (scene_man.IsSceneLoaded("Level3"))
      {
        level = "Level3";
      }
      Scene& scene = scene_man.GetScene(level);
    //Allocating new array breaks component TEMP fix
    if (tempINIT == true)
    {
      Deserializer* deserial = dynamic_cast<Deserializer*>
        (engine->Get("Deserializer"));
      int heartToPush = deserial->DeserializePrefab("Data/Prefabs/Health.json");
      GameObjectManager& gameObMan = *GetSystemFromEngine<GameObjectManager*>
        (*engine, "GameObjectManager");
      GameObject& theHeart = gameObMan[heartToPush];
      scene.Add(heartToPush);
      //theHeart.SetName("Health" + std::to_string(counter));
      heartTracker.push_back(heartToPush);
      counter++;
      tempINIT = false;
    }

    while (heartTracker.size() < healthCounter)
    {
      Deserializer* deserial = dynamic_cast<Deserializer*>
        (engine->Get("Deserializer"));
      GameObjectManager& gameObMan = *GetSystemFromEngine<GameObjectManager*>
        (*engine, "GameObjectManager");
      int heartToPush2 = deserial->DeserializePrefab("Data/Prefabs/Health.json");
      scene.Add(heartToPush2);
      GameObject& theHeart = gameObMan[heartToPush2];
      //theHeart.SetName("Health"+ std::to_string(counter));
      int heartsID = theHeart.GetComponentID("UI");
      UIManager& theUIMan = *GetSystemFromEngine<UIManager*>
        (*engine, "UIManager");
      theUIMan[heartsID].SetOffset(glm::vec3(-700 + (100 * (int)heartTracker.size()), 350.0, -0.5));

      heartTracker.push_back(heartToPush2);
      ++counter;
    }
  }

  void Health::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial;
    auto object = jsonVal.GetObject();
    auto health = object["Lifes"].GetInt();
    healthCounter = health;
    auto topHealth = object["MaxLifes"].GetInt();
    maxHealth = topHealth;
  }

  void Health::Write(IJsonWriter& writer) const
  {
    writer.WriteKey("Health");
    writer.StartObject();
    writer.WriteInt("Lifes", healthCounter);
    writer.WriteInt("MaxLifes", maxHealth);
    writer.EndObject();
  }

  void Health::ImGuiDisplay(float scale)
  {
    ImGui::PushItemWidth(300 * scale);
    ImGui::InputInt("Current health", &maxHealth, 1);
    ImGui::PopItemWidth();
    if (ImGui::Button("Increase Health", { 250 * scale,50 * scale }))
    {
      IncreaseHealth();
    }
    if (ImGui::Button("Decrease Health", { 250 * scale,50 * scale }))
    {
      DecreaseHealth();
    }
  }

  void Health::IncreaseHealth()
  {
    if (healthCounter < maxHealth)
    {
      healthCounter += 1;
    }
  }

  void Health::DecreaseHealth()
  {
    if (!heartTracker.empty())
    {
      healthCounter -= 1;

      GameObjectManager& gameObMan = *GetSystemFromEngine<GameObjectManager*>
        (*engine, "GameObjectManager");
      int idHeart = heartTracker.back();
      gameObMan.DeleteGameObject(idHeart);
      heartTracker.pop_back();
      --counter;
    }
  }

} // End Of Dapper Namespace