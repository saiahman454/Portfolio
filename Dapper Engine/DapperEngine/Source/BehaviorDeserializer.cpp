//------------------------------------------------------------------------------
//
// File Name:	BehaviorDeserializer.cpp
// Author(s): Gavin Lu
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "BehaviorDeserializer.h"
#include "BehaviorManager.h"
#include "ComponentRegistry.h"
#include "Utility.h"

// Behavior includes
#include "PlayerController.h"
#include "JellyfishBehavior.h"
#include "FloatingHeartBehavior.h"
#include "BGMBehavior.h"
#include "Swordfish.h"
#include "RestartBehavior.h"
#include "NavButtonBehavior.h"
#include "ExitSceneButtonBehavior.h"
#include "LoadSceneButtonBehavior.h"
#include "QuitButtonBehavior.h"
#include "BreakableWallBehavior.h"
#include "SwordfishMissle.h"
#include "CollectibleBehavior.h"
#include "PlayerCollectiblesUIBehavior.h"
#include "LevelCollectiblesUIBehavior.h"
#include "ProgressionBarBehavior.h"
#include "SplashFadeBehavior.h"
#include "CreditScrollBehavior.h"
#include "MainMenuBackgroundBehavior.h"
#include "FullScreenBehavior.h"
#include "MuteBehavior.h"

// Dapper Namespace!
namespace Dapper 
{

  void BehaviorDestroy(int ID, Engine& engine)
  {

    BehaviorManager& the_man = *dynamic_cast<BehaviorManager*>(engine.Get("BehaviorManager"));
    the_man.Destroy(ID);

  }

  IComponent* BehaviorGet(int ID, Engine& engine)
  {
    BehaviorManager& the_man = *dynamic_cast<BehaviorManager*>(engine.Get("BehaviorManager"));
    return the_man[ID];
  }

  IComponent* PlayerControllerCreate(Engine& engine)
  {
    
    BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
    int id = man->GetNew();
    PlayerController *control = new PlayerController(id, engine);
    (*man)[id] = control;
    return control;

  }

  std::pair<std::string_view, int> PlayerControllerDeserialize(Engine& engine, const rapidjson::Value& val)
  {

    BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
    int id = man->GetNew();
    PlayerController* controller = new PlayerController(id, engine);
    (*man)[id] = controller;
    Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
    controller->Read(des, val);
    return { controller->GetName(), id };

  }

  //adam floating hearts
  IComponent* FloatingHeartBehaviorCreate(Engine& engine)
  {

      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      FloatingHeartBehavior* floatingHeart = new FloatingHeartBehavior(id, &engine);
      (*man)[id] = floatingHeart;
      return floatingHeart;

  }

  std::pair<std::string_view, int> FloatingHeartBehaviorDeserialize(Engine& engine, const rapidjson::Value& val)
  {

      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      FloatingHeartBehavior* floatingHeart = new FloatingHeartBehavior(id, &engine);
      (*man)[id] = floatingHeart;
      Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
      floatingHeart->Read(des, val);
      return { floatingHeart->GetName(), id };
  }

  IComponent* JellyfishBehaviorCreate(Engine& engine)
  {

    BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
    int id = man->GetNew();
    JellyfishBehavior* jellyfish = new JellyfishBehavior(id, &engine);
    (*man)[id] = jellyfish;
    return jellyfish;

  }

  std::pair<std::string_view, int> JellyfishBehaviorDeserialize(Engine& engine, const rapidjson::Value& val)
  {

    BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
    int id = man->GetNew();
    JellyfishBehavior* jellyfish = new JellyfishBehavior(id, &engine);
    (*man)[id] = jellyfish;
    Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
    jellyfish->Read(des, val);
    return { jellyfish->GetName(), id };

  }

  IComponent* SwordfishCreate(Engine& engine)
  {

    BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
    int id = man->GetNew();
    Swordfish* control = new Swordfish(id, engine);
    (*man)[id] = control;
    return control;

  }

  std::pair<std::string_view, int> SwordfishDeserialize(Engine& engine, const rapidjson::Value& val)
  {
    BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
    int id = man->GetNew();
    Swordfish* fish = new Swordfish(id, engine);
    (*man)[id] = fish;
    Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
    fish->Read(des, val);
    return { fish->GetName(), id };

  }

  IComponent* BGMBehaviorCreate(Engine& engine)
  {

    BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
    int id = man->GetNew();
    BGMBehavior* bgm = new BGMBehavior(id, &engine);
    (*man)[id] = bgm;
    return bgm;

  }

  std::pair<std::string_view, int> BGMBehaviorDeserialize(Engine& engine, const rapidjson::Value& val)
  {
    BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
    int id = man->GetNew();
    BGMBehavior* bgm = new BGMBehavior(id, &engine);
    (*man)[id] = bgm;
    Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
    bgm->Read(des, val);
    return { bgm->GetName(), id };

  }

  IComponent* RestartBehaviorCreate(Engine& engine)
  {

    BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
    int id = man->GetNew();
    RestartBehavior* resart = new RestartBehavior(id, engine);
    (*man)[id] = resart;
    return resart;

  }

  std::pair<std::string_view, int> RestartBehaviorDeserialize(Engine& engine, const rapidjson::Value& val)
  {
    BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
    int id = man->GetNew();
    RestartBehavior* resart = new RestartBehavior(id, engine);
    (*man)[id] = resart;
    Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
    resart->Read(des, val);
    return { resart->GetName(), id };

  }

  IComponent* BreakableWallBehaviorCreate(Engine& engine)
  {
      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      BreakableWallBehavior* control = new BreakableWallBehavior(id, &engine);
      (*man)[id] = control;
      return control;
  }

  std::pair<std::string_view, int> BreakableWallBehaviorDeserialize(Engine& engine, const rapidjson::Value& val)
  {
      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      BreakableWallBehavior* wall = new BreakableWallBehavior(id, &engine);
      (*man)[id] = wall;
      Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
      wall->Read(des, val);
      return { wall->GetName(), id };
  }

  IComponent* CollectibleBehaviorCreate(Engine& engine)
  {
      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      CollectibleBehavior* collectible = new CollectibleBehavior(id, &engine);
      (*man)[id] = collectible;
      // Increase total number of collectibles
      man->SetNumCollectibles(man->GetNumCollectibles() + 1);
      return collectible;
  }

  std::pair<std::string_view, int> CollectibleBehaviorDeserialize(Engine& engine, const rapidjson::Value& val)
  {
      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      CollectibleBehavior* collectible = new CollectibleBehavior(id, &engine);
      (*man)[id] = collectible;
      Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
      collectible->Read(des, val);
      return { collectible->GetName(), id };
  }

  IComponent* PlayerCollectiblesUIBehaviorCreate(Engine& engine)
  {

      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      PlayerCollectiblesUIBehavior* behavior = new PlayerCollectiblesUIBehavior(id, &engine);
      (*man)[id] = behavior;
      return behavior;

  }

  std::pair<std::string_view, int> PlayerCollectiblesUIBehaviorDeserialize(Engine& engine, const rapidjson::Value& val)
  {
      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      PlayerCollectiblesUIBehavior* behavior = new PlayerCollectiblesUIBehavior(id, &engine);
      (*man)[id] = behavior;
      Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
      behavior->Read(des, val);
      return { behavior->GetName(), id };
  }

  IComponent* LevelCollectiblesUIBehaviorCreate(Engine& engine)
  {

      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      LevelCollectiblesUIBehavior* behavior = new LevelCollectiblesUIBehavior(id, &engine);
      (*man)[id] = behavior;
      return behavior;

  }

  std::pair<std::string_view, int> LevelCollectiblesUIBehaviorDeserialize(Engine& engine, const rapidjson::Value& val)
  {
      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      LevelCollectiblesUIBehavior* behavior = new LevelCollectiblesUIBehavior(id, &engine);
      (*man)[id] = behavior;
      Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
      behavior->Read(des, val);
      return { behavior->GetName(), id };
  }

  IComponent* ProgressionBarBehaviorCreate(Engine& engine)
  {

      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      ProgressionBarBehavior* behavior = new ProgressionBarBehavior(id, &engine);
      (*man)[id] = behavior;
      return behavior;

  }

  std::pair<std::string_view, int> ProgressionBarBehaviorDeserialize(Engine& engine, const rapidjson::Value& val)
  {
      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      ProgressionBarBehavior* behavior = new ProgressionBarBehavior(id, &engine);
      (*man)[id] = behavior;
      Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
      behavior->Read(des, val);
      return { behavior->GetName(), id };
  }

  IComponent* MainMenuBackgroundBehaviorCreate(Engine& engine)
  {

      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      MainMenuBackgroundBehavior* behavior = new MainMenuBackgroundBehavior(id, &engine);
      (*man)[id] = behavior;
      return behavior;

  }

  std::pair<std::string_view, int> MainMenuBackgroundBehaviorDeserialize(Engine& engine, const rapidjson::Value& val)
  {
      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      MainMenuBackgroundBehavior* behavior = new MainMenuBackgroundBehavior(id, &engine);
      (*man)[id] = behavior;
      Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
      behavior->Read(des, val);
      return { behavior->GetName(), id };
  }

  template <typename T> 
  IComponent* BehaviorCreate(Engine& engine)
  {

      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      T* theBehavior = new T(id, engine);
      (*man)[id] = theBehavior;
      return theBehavior;
  }

  template <typename T>
  std::pair<std::string_view, int> BehaviorDeserialize(Engine& engine, const rapidjson::Value& val)
  {
      BehaviorManager* man = GetSystemFromEngine<BehaviorManager*>(engine, "BehaviorManager");
      int id = man->GetNew();
      T* theBehavior = new T(id, engine);
      (*man)[id] = theBehavior;
      Deserializer& des = *dynamic_cast<Deserializer*>(engine.Get("Deserializer"));
      theBehavior->Read(des, val);
      return { theBehavior->GetName(), id };
  }


  void RegisterBehavior(Engine& engine, std::string_view name, ConstructFunction construct, DeserializeFunction func)
  {

    ComponentRegistry::FunctionBuffer BehaviorFunctions{ construct, BehaviorDestroy, BehaviorGet };

    dynamic_cast<Deserializer*>(engine.Get("Deserializer"))->RegisterFunction(name, func);
    dynamic_cast<ComponentRegistry*>(engine.Get("ComponentRegistry"))->Register(name, BehaviorFunctions);

  }

  template <typename T>
  void RegisterBehavior(Engine& engine, const char* name)
  {

      ComponentRegistry::FunctionBuffer BehaviorFunctions{ BehaviorCreate<T>, BehaviorDestroy, BehaviorGet };

      dynamic_cast<Deserializer*>(engine.Get("Deserializer"))->RegisterFunction(name, BehaviorDeserialize<T>);
      dynamic_cast<ComponentRegistry*>(engine.Get("ComponentRegistry"))->Register(name, BehaviorFunctions);
  }

  void RegisterAllBehaviors(Engine& engine) 
  {

    RegisterBehavior(engine, "PlayerController", PlayerControllerCreate, PlayerControllerDeserialize);
    RegisterBehavior(engine, "Swordfish", SwordfishCreate, SwordfishDeserialize);
    RegisterBehavior(engine, "JellyfishBehavior", JellyfishBehaviorCreate, JellyfishBehaviorDeserialize);
    RegisterBehavior(engine, "FloatingHeartBehavior", FloatingHeartBehaviorCreate, FloatingHeartBehaviorDeserialize);
    RegisterBehavior(engine, "BGMBehavior", BGMBehaviorCreate, BGMBehaviorDeserialize);
    RegisterBehavior(engine, "RestartBehavior", RestartBehaviorCreate, RestartBehaviorDeserialize);
    RegisterBehavior(engine, "BreakableWallBehavior", BreakableWallBehaviorCreate, BreakableWallBehaviorDeserialize);
    RegisterBehavior(engine, "CollectibleBehavior", CollectibleBehaviorCreate, CollectibleBehaviorDeserialize);
    RegisterBehavior(engine, "PlayerCollectiblesUIBehavior", PlayerCollectiblesUIBehaviorCreate, PlayerCollectiblesUIBehaviorDeserialize);
    RegisterBehavior(engine, "LevelCollectiblesUIBehavior", LevelCollectiblesUIBehaviorCreate, LevelCollectiblesUIBehaviorDeserialize);
    RegisterBehavior(engine, "ProgressionBarBehavior", ProgressionBarBehaviorCreate, ProgressionBarBehaviorDeserialize);
    RegisterBehavior(engine, "MainMenuBackgroundBehavior", MainMenuBackgroundBehaviorCreate, MainMenuBackgroundBehaviorDeserialize);
    RegisterBehavior<NavButtonBehavior>(engine, "NavButtonBehavior");
    RegisterBehavior<QuitButtonBehavior>(engine, "QuitButtonBehavior");
    RegisterBehavior<LoadSceneButtonBehavior>(engine, "LoadSceneButtonBehavior");
    RegisterBehavior<ExitSceneButtonBehavior>(engine, "ExitSceneButtonBehavior");
    RegisterBehavior<SwordfishMissle>(engine, "SwordfishMissle");
    RegisterBehavior<SplashFadeBehavior>(engine, "SplashFadeBehavior");
    RegisterBehavior<CreditScrollBehavior>(engine, "CreditScrollBehavior");
    RegisterBehavior<FullScreenBehavior>(engine, "FullScreenBehavior");
    RegisterBehavior<MuteBehavior>(engine, "MuteBehavior");
  }

} // End Of Dapper Namespace