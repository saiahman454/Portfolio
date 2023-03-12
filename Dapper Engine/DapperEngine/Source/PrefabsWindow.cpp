//------------------------------------------------------------------------------
//
// File Name:	PrefabsWindow.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "PrefabsWindow.h"
#include "Utility.h"
#include "Camera.h"
#include "TransformManager.h"
#include "SaveMenu.h"
#include "SceneManager.h"

// Dapper Namespace!
namespace Dapper {

	PrefabsWindow::PrefabsWindow(LevelEditor& _parent)
		: parent(_parent)
		, engine(_parent.GetEngine())
		, object_manager(GetSystemFromEngine<GameObjectManager*>(engine, "GameObjectManager"))
	{
	}

	void PrefabsWindow::Init()
	{
	}

	void PrefabsWindow::Render()
	{
		float scale = parent.GetScaleFactor();
		// WINDOW TO CREATE NEW GAME OBJECT
		ImGui::SetWindowFontScale(1.5f * scale);
		ImGui::Begin("Create A New Game Object");
		//ImGui::SetWindowPos({ 1250 * scale,100 * scale });
		//ImGui::SetWindowSize({ 350 * scale,800 * scale }, 0);
		ImGui::SetWindowFontScale(1.5f * scale);

		if (ImGui::Button("Create Empty Game Object", { 275 * scale,50 * scale }))
		{
			object_manager->MakeNewWithName("");
		}

		// Prefabs List
		ImGui::Text("Select From Prefabs:");
		ImGui::SetWindowFontScale(1.5f * scale);
		if (ImGui::Button("Basic Tile", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("BasicTile");
		if (ImGui::Button("Fancy Tile", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("FancyTile");
		if (ImGui::Button("Seaweed Environment", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("SeaweedEnvironment");
		if (ImGui::Button("Jellyfish Enemy", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("JellyfishEnemy");
		if (ImGui::Button("Swordfish Enemy", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("SwordfishEnemy");
		if (ImGui::Button("Player", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("Player");
		if (ImGui::Button("Health Pickup", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("HealthPickup");
		if (ImGui::Button("Health Spawner", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("HealthSpawner");
		if (ImGui::Button("Win Trigger", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("WinTrigger");
		if (ImGui::Button("Background", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("Background");
		if (ImGui::Button("Breakable Wall", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("BreakableWall");
		if (ImGui::Button("Collectible", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("Collectible");
		if (ImGui::Button("SwordfishMissle", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("SwordfishMissle");
		if (ImGui::Button("Text Object", { 200 * scale,50 * scale }))
			CreateNewObjectFromPrefab("Text");
		ImGui::End();
	}

	PrefabsWindow::~PrefabsWindow()
	{
	}

	/*--------------------------------------
	  Description: Creates a new prefab given its type name
	  Params:
	  name - name of prefab type
	  scale - resolution size
	  Returns: the ID of the game object created
	 ---------------------------------------*/
	int PrefabsWindow::CreateNewObjectFromPrefab(std::string const& prefab_name)
	{
		// Camera and Transform
		Camera* camera_manager = GetSystemFromEngine<Camera*>(engine, "Camera");
		TransformManager* transform_manager = GetSystemFromEngine<TransformManager*>(engine, "TransformManager");
		SceneManager& scene_manager = *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");

		int ID = object_manager->MakePrefabWithName(prefab_name);
		int transform_ID = (*object_manager)[ID].GetComponentID("Transform");
		(*transform_manager)[transform_ID].setTranslation(camera_manager->getCameraPosition());
		SaveMenu* save_menu  = dynamic_cast<SaveMenu*>(parent.GetWindow("Save Window"));
		std::string_view scene_name = save_menu->GetCurrentScene();
		Scene& scene = scene_manager.GetScene(scene_name);
		scene.Add(ID);
		return ID;
	}
} // End Of Dapper Namespace