//------------------------------------------------------------------------------
//
// File Name:	Button.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#include "Button.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Utility.h"
#include "Input.h"
#include "GameStateManager.h"
#include "Engine.h"
#include "Transform.h"
#include "TextureManager.h"
#include "SpriteSource.h"
#include "Audio.h"

// Dapper Namespace!
namespace Dapper {
    Button::Button(int ID_, Engine& engine_)
    : engine(engine_), my_ID(ID_), buttonCon(none)
  {
        //onNoneTex = "./Assets/Texture/UI_Button_Lose_Quit_Idle.png";
        //onHoveredTex = "./Assets/Texture/UI_Button_Lose_Quit_Hover.png";
        //onClickedTex = "./Assets/Texture/UI_Button_Lose_Quit_Clicked.png";
  }

    Button::~Button()
  {
  }


  void Button::Init()
  {
      //Need to add in Button::READ and Write to Implement
      //No start and end object - becuase it would 
      
      //Register function to set sprite for onNone
      //Register function to set sprite forhovered
      //Register function to set sprite for clicked
      // send message for none to set first button image

    // Hover sound
    const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");

    auto d = audio.GetSound(hoverFileName);

    d->SetVolume(hoverVolume);

    // Click sound

    auto d2 = audio.GetSound(clickFileName);

    d2->SetVolume(clickVolume);

  }

  int Button::GetParentID() const
  {
    return parent_ID;
  }

  void Button::SetParentID(int ID)
  {
    parent_ID = ID;
  }

  void Button::Update(float dt)
  {
    dt;
    if (!isEnabled)
    {
        return;
    }
    if (buttonCon == none)
    {
        ChangeTexture(onNoneTex);
        if (MouseOnButton())
        {
            buttonCon = hovered;
            onHovered.SendMessage();
            const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
            auto sound = audio.GetSound(hoverFileName);
            sound->Play();
        }

    }
    else if (buttonCon == hovered)
    {
        ChangeTexture(onHoveredTex);
        Input& input = *dynamic_cast<Input*>(engine.Get("Input"));
        if (!MouseOnButton())
        {
            buttonCon = none;
            onNone.SendMessage();
        }
        else if (input.getMouse(GLFW_MOUSE_BUTTON_1) == mouse_button::MOUSE_TRIGGERED)
        {
            buttonCon = clicked;
            onClicked.SendMessage();
            const Audio& audio = *GetSystemFromEngine<Audio*>(engine, "Audio");
            auto sound = audio.GetSound(clickFileName);
            sound->Play();
        }
    }
    else if (buttonCon == clicked)
    {
        ChangeTexture(onClickedTex);
        Input& input = *dynamic_cast<Input*>(engine.Get("Input"));
        if (input.getMouse(GLFW_MOUSE_BUTTON_1) == mouse_button::MOUSE_RELEASE)
        {
            buttonCon = hovered;
            onReleased.SendMessage();
            onHovered.SendMessage();
        }
        else if (!MouseOnButton())
        {
            buttonCon = none;
            onNone.SendMessage();
        }
    }

  }

  void Button::SetID(int ID)
  {
    my_ID = ID;
  }

  const int Button::GetID() const
  {
    return my_ID;
  }

  void Button::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
      deserial, jsonVal;

      auto obj = jsonVal.GetObject();
      onNoneTex = jsonVal["NoneTex"].GetString();
      onHoveredTex = jsonVal["HoveredTex"].GetString();
      onClickedTex = jsonVal["ClickedTex"].GetString();
      hoverFileName = jsonVal["HoverFileName"].GetString();
      hoverVolume = jsonVal["HoverVolume"].GetFloat();
      clickFileName = jsonVal["ClickFileName"].GetString();
      clickVolume = jsonVal["ClickVolume"].GetFloat();
  }

  void Button::Write(IJsonWriter& writer) const
  {
      // Write all of the values to the json file.
      //writer.WriteKey("Button");
      //writer.StartObject();
      writer.WriteString("NoneTex", onNoneTex.c_str());
      writer.WriteString("HoveredTex", onHoveredTex.c_str());
      writer.WriteString("ClickedTex", onClickedTex.c_str());
      writer.WriteString("HoverFileName", hoverFileName.c_str());
      writer.WriteFloat("HoverVolume", hoverVolume);
      writer.WriteString("ClickFileName", clickFileName.c_str());
      writer.WriteFloat("ClickVolume", clickVolume);
      //Button::Write() //Adam add for sprites
      //writer.EndObject();
  }

  Button::states Button::ButtonCondition() const
  {
      return buttonCon;
  }

  bool Button::MouseOnButton() const
  {
      Input& input = *dynamic_cast<Input*>(engine.Get("Input"));
      glm::vec2 theCoords = input.GetMousePosinWorldCoords();

      Transform* theTransform = GetComponentFromParent<Transform>
          (engine, parent_ID, "Transform");
      glm::vec3 theButton = theTransform->GetTranslation();
      float theWidth = (theTransform->GetScale().x * 0.5);
      float theHeight = (theTransform->GetScale().y * 0.5);
      // input.getMouse(GLFW_MOUSE_BUTTON_1) == mouse_button::MOUSE_RELEASE &&
      //If met we are inside the box and click released
      if (theCoords.x <= theButton.x + theWidth &&
          theCoords.x >= theButton.x - theWidth &&
          theCoords.y <= theButton.y + theHeight &&
          theCoords.y >= theButton.y - theHeight)
      {
          return true;
      }
      return false;
  }

  void Button::ChangeTexture(std::string input)
  {
      TextureManager& tex_man = *GetSystemFromEngine<TextureManager*>
          (engine, "TextureManager");
      SpriteSource* theSpriteSource = GetComponentFromParent<SpriteSource>
          (engine, parent_ID, "SpriteSource");

      if (!input.empty())
      {
          tex_man.LoadNewTexture("./Assets/Texture/" + input + ".png");
          //tex_man.LoadNewTexture(input);
          if (tex_man.Search(input) != -1)
          {
              theSpriteSource->SetTextureID(tex_man.Search(input));
          }
      }
  }

} // End Of Dapper Namespace