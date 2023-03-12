//------------------------------------------------------------------------------
//
// File Name:	ProgressionBarBehavior.cpp
// Author(s):	Natalie Bouley
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "ProgressionBarBehavior.h"
#include "ComponentRegistry.h"
#include "Utility.h"
#include "UIManager.h"
#include "imgui.h"
#include "Utility.h"
#include "PlayerController.h"
#include "GameObjectManager.h"
#include "UI.h"
#include "Transform.h"
#include <algorithm>

// Dapper Namespace!
namespace Dapper
{
  ProgressionBarBehavior::ProgressionBarBehavior(int ID, Engine* theEngine) :
    engine(theEngine),
    paused(false),
    my_ID(ID),
    parent_ID(-1)
  {
  }

  ProgressionBarBehavior::~ProgressionBarBehavior()
  {
  }

  void ProgressionBarBehavior::Init()
  {
    UI& icon_UI = *GetComponentFromParent<UI>(*engine, parent_ID, "UI");

    // Get the player's player controller and transform
    GameObjectManager& gameobject_manager = *GetSystemFromEngine<GameObjectManager*>(*engine, "GameObjectManager");
    int player_ID = gameobject_manager.FindObjectWithTag("Player");
    PlayerController& player_controller = *GetComponentFromParent<PlayerController>(*engine, player_ID, "PlayerController");
    Transform& player_transform = *GetComponentFromParent<Transform>(*engine, player_ID, "Transform");
    player_start_pos = player_transform.GetTranslation().x;

    // Slider- a separate game object that looks like a little line. Does not have the behavior
    int slider_ID = gameobject_manager.FindObjectWithTag("Slider");
    assert(slider_ID != -1);
    Transform& slider_transform = *GetComponentFromParent<Transform>(*engine, slider_ID, "Transform");
    UI& slider_UI = *GetComponentFromParent<UI>(*engine, slider_ID, "UI");
    slider_width = slider_transform.GetScale().x;
    slider_min_pos = slider_UI.GetOffset().x - slider_width / 2;
    slider_max_pos = slider_min_pos + slider_width; // Left side + width

    // Get the ID of the win trigger item
    // Trigger- a separate game object that is the trigger to finish the game. Only used here to set the player_end_pos
    int trigger_ID = gameobject_manager.FindObjectWithTag("WinTrigger1");
    if (trigger_ID != -1)
    {
      Transform& win_trigger_transform = *GetComponentFromParent<Transform>(*engine, trigger_ID, "Transform");
      player_end_pos = win_trigger_transform.GetTranslation().x;
    }
  }

  void ProgressionBarBehavior::Update(float dt)
  {
    // Get the bar's UI and Transform components
    Transform& icon_transform = *GetComponentFromParent<Transform>(*engine, parent_ID, "Transform");
    UI& icon_ui = *GetComponentFromParent<UI>(*engine, parent_ID, "UI");

    // Get the player's player controller and transform
    GameObjectManager& gameobject_manager = *GetSystemFromEngine<GameObjectManager*>(*engine, "GameObjectManager");
    int player_ID = gameobject_manager.FindObjectWithTag("Player");
    // Get the player transform
    Transform& player_transform = *GetComponentFromParent<Transform>(*engine, player_ID, "Transform");
    player_curr_pos = player_transform.GetTranslation().x;

    // Update the scale and UI position
    float relative_scale = (player_curr_pos - player_start_pos) / (player_end_pos - player_start_pos);
    float icon_new_ui_pos = slider_min_pos + (relative_scale * slider_width);

    icon_new_ui_pos = std::clamp(icon_new_ui_pos, slider_min_pos, slider_max_pos);

    icon_ui.SetOffset(glm::vec3(icon_new_ui_pos, icon_ui.GetOffset().y, 0.0f));
  }

  void ProgressionBarBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    deserial;
  }

  void ProgressionBarBehavior::Write(IJsonWriter& writer) const
  {
    writer.WriteKey("ProgressionBarBehavior");
    writer.StartObject();
    writer.EndObject();
  }

  void ProgressionBarBehavior::ImGuiDisplay(float scale)
  {
  }

} // End Of Dapper Namespace