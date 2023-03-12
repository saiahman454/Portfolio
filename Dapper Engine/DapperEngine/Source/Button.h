//------------------------------------------------------------------------------
//
// File Name:	Button.h
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "IComponent.h"
#include "Messenger.h"


//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

  class Button : public IComponent
  {
  public:

     Button(int ID_, Engine& engine_);

    ~Button() override;

    void Init() override;

    int GetParentID() const override;

    void SetParentID(int ID);

    void Update(float dt) override;

    void SetID(int ID)override;

    const int GetID() const override;

    void Read(Deserializer& deserial, const rapidjson::Value& jsonVal) override;
    void Write(IJsonWriter& writer) const override;

    //mouse condition
    enum states
    {
        none,
        hovered,
        clicked
    };

    states ButtonCondition() const;

  private:
    Engine& engine; // The engine

    int my_ID; // The ID of the component in the behavior manager

    int parent_ID = -1; // The ID of the parent game object
    states buttonCon;
    bool MouseOnButton() const;
    std::string onNoneTex;
    std::string onHoveredTex;
    std::string onClickedTex;
    void ChangeTexture(std::string input);
    bool paused = false;
    std::string hoverFileName = "";
    float hoverVolume = 0.5;
    std::string clickFileName = "";
    float clickVolume = 0.5;

  protected:
      Messenger<> onNone;
      Messenger<> onHovered;
      Messenger<> onClicked;
      Messenger<> onReleased;
  };
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------