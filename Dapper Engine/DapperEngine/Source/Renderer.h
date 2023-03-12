//------------------------------------------------------------------------------
//
// File Name:	Renderer.h
// Author(s):	Adam Tackett (Luke Mostert)
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "ISystem.h"
#include "Engine.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <map>

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {


  struct DrawData
  {
    glm::vec3 position = { 0,0,0 };
    glm::vec2 scale = { 0,0 };
    int meshIndex = -1;
    int textureID = -1;
    float rotation = 0;
    glm::vec2 theU = { 1, 0 };
    glm::vec2 theV = { 1, 0 };
    glm::vec2 theMinUV = { 0, 0 };
    glm::vec2 theMaxUV = { 1, 1 };
    glm::vec4 theColor = { 1, 1, 1, 1 };
  };


#define GL(function) function;\
  logopenGLErrors(#function, __FILE__, __LINE__);\
  clearopenGLErrors();

  void clearopenGLErrors();
  bool logopenGLErrors(const char* functionName, const char* fileName, int fileLine);


  class Renderer :public ISystem
  {
  public:
    Renderer(Engine& engine1);
    ~Renderer();

    inline const std::string& Name()const override { return name; }

    void Initialize()override;
    void Update(float dt)override;
    void Render()override;
    void Draw(DrawData draw_data);

    void DebugLine(glm::vec2 startPoint, glm::vec2 endPoint, glm::vec4 color);
    void DebugSquare(glm::vec2 position, glm::vec2 scale, glm::vec4 color);

  private:

    const std::string name = "Renderer";
    Engine& engine;
    std::map<float, std::vector<DrawData>> renderOrder;


    // Inherited via ISystem
    virtual bool UpdateWhilePaused() const override;

  };//End Renderer Class




} // End Of Dapper Namespace

//------------------------------------------------------------------------------