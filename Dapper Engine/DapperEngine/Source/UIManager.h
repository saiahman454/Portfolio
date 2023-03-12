//------------------------------------------------------------------------------
//
// File Name:	UI.h
// Author(s):	Adam Tackett (Luke Mostert)
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "ITypeManager.h"
#include "Mesh.h"
#include "Engine.h"
#include "UI.h"
#include "IDManager.h"
#include "rapidjson/document.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper
{
  class UIManager: public ITypeManager<UI>
  {
  public:
    UIManager(Engine& engine);
    ~UIManager()override;

    inline const std::string& Name()const override { return name; }
    void Initialize()override;
    void Update(float dt) override;
    void Render() override;
    inline UI& operator[](int index) { return UI_list[idManager[index]]; }
    int GetNew();
    std::pair<UI*, int> HasUI(std::string camera_name);
    void Destroy(int ID);

  private:
    const std::string name = "UIManager";
    std::vector<UI> UI_list;
    Engine& the_engine;
    IDManager idManager;


    virtual bool UpdateWhilePaused() const override;

  };//End of CameraAnchorManager class

  std::pair<std::string_view, int> UIDeserialize(Engine& engine, const rapidjson::Value& val);

  UI* UICreate(Engine& engine);
  void UIDestroy(int ID, Engine& engine);
  UI* UIGet(int ID, Engine& engine);
} // End Of Dapper Namespace