//-----------------------------------------------------------------------------
//
// File Name:	main.cpp
// Author(s):	The Dapper Dolphins, (Code review by Isaiah Dickison)
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------


#include <stdio.h>
#include <vector>
#include <iostream>

#include "Engine.h"
//Isystems in update order
#include "Trace.h"
#include "Input.h"
#include "Window.h"
#include "Time.h"
#include "ComponentRegistry.h"
#include "GameStateManager.h"
#include "Deserializer.h"
#include "AnimationManager.h"
#include "SpriteManager.h"
#include "SpriteSourceManager.h"
#include "TileManager.h"
#include "TransformManager.h"
#include "Audio.h"
#include "PhysicsManager.h"
#include "ColliderManager.h"
#include "GameObjectManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "MeshManager.h"
#include "BehaviorManager.h"
#include "EmitHealthManager.h"
#include "ParticleEmitterManager.h"
#include "Camera.h"
#include "CameraAnchorManager.h"
#include "UIManager.h"
#include "HealthManager.h"
#include "BackgroundManager.h"
#include "Renderer.h"
#include "LevelEditor.h"
#include "SceneManager.h"
#include "SpriteTextManager.h"
#include "TotalCollectableManager.h"
#include "Ambience.h"
#include "BGMController.h"
//end of Isystems


void AddAllSystems(Dapper::Engine& engine);
#ifdef _DEBUG
int main()
#else
#include <Windows.h>
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
  PSTR lpCmdLine, INT nCmdShow)
#endif
{
  Dapper::Engine dapper;
  AddAllSystems(dapper);
  dapper.Init();
  while (dapper.IsRunning()) {
    dapper.Update(dapper.GetUpdatedt());
    dapper.Render();
  }
  return 0;
}

void AddAllSystems(Dapper::Engine& engine) {
  engine.Add(new Dapper::Trace);
  engine.Add(new Dapper::Input(engine));
  engine.Add(new Dapper::Window(1600, 900, engine));
  engine.Add(new Dapper::Time(engine.GetdtReference(),engine.GetRenderdtReference()));
  engine.Add(new Dapper::ComponentRegistry(engine));
  engine.Add(new Dapper::GameStateManager(engine, ""));
  Dapper::SceneManager* theScene = new Dapper::SceneManager(engine);
  engine.Add(theScene);
  theScene->LoadScene("SplashScreen");
  engine.Add(new Dapper::Deserializer(engine));
  engine.Add(new Dapper::AnimationManager(engine));
  engine.Add(new Dapper::SpriteManager(engine));
  engine.Add(new Dapper::SpriteTextManager(engine));
  engine.Add(new Dapper::SpriteSourceManager(engine));
  engine.Add(new Dapper::TileManager(engine));
  engine.Add(new Dapper::TransformManager(engine));
  engine.Add(new Dapper::Audio(engine));
  engine.Add(new Dapper::PhysicsManager(engine));
  engine.Add(new Dapper::ColliderManager(engine));
  engine.Add(new Dapper::GameObjectManager(engine));
  engine.Add(new Dapper::TextureManager(engine));
  engine.Add(new Dapper::ShaderManager(engine));
  engine.Add(new Dapper::MeshManager(engine));
  engine.Add(new Dapper::BehaviorManager(engine));
  engine.Add(new Dapper::EmitHealthManager(engine));
  engine.Add(new Dapper::TotalCollectableManager(engine));
  engine.Add(new Dapper::ParticleEmitterManager(engine));
  engine.Add(new Dapper::CameraAnchorManager(engine));
  engine.Add(new Dapper::Camera(engine));
  engine.Add(new Dapper::BackgroundManager(engine));
  engine.Add(new Dapper::UIManager(engine));
  engine.Add(new Dapper::HealthManager(engine));
  engine.Add(new Dapper::LevelEditor(engine));
  engine.Add(new Dapper::Renderer(engine));
  engine.Add(new Dapper::Ambience(engine));
  engine.Add(new Dapper::BGMController(engine));

  //Add more systems to this function as you make them

}
