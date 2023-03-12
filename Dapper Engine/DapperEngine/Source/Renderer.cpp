//------------------------------------------------------------------------------
//
// File Name:	Renderer.cpp
// Author(s):	Adam Tackett (Luke Mostert)
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <glm\mat4x4.hpp>
//#include <glm\vec3.hpp>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
//#include <glm\gtc\type_ptr.hpp>
//#include <glm/ext.hpp>

#include "Renderer.h"
#include "Window.h"
#include "Camera.h"
#include "Mesh.h"
#include "Input.h"
#include "Texture.h"
#include "Physics.h"
#include "PhysicsManager.h"
#include "CircleCollider.h"
#include "ColliderManager.h"
#include "ComponentRegistry.h"
#include "Utility.h"
#include "MeshManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "TransformManager.h"
#include "SquareCollider.h"

// Dapper Namespace!
namespace Dapper {
  const float toRadians = 3.14159265f / 180.0f;
  /* --------------------------------------
    Description: clears the errors
    Params:
    -------------------------------------- - */
  void clearopenGLErrors()
  {
    while (glGetError() != GL_NO_ERROR);
  }

  /* --------------------------------------
    Description: prints opengl errors
    Params:
    const char* functionName ?name of the function
    const char* fileName ?name of the file
    int fileLine - the line of the file
    Return : bool, true if error with the error message printed
    -------------------------------------- - */
  bool logopenGLErrors(const char* functionName, const char* fileName, int fileLine)
  {
    bool error = false;  // indicates if there has been an openGL error

    if (functionName && fileName)
    {
      GLenum GLError;   // the error code for a possible openGL error

        // while there are errors being logged
      while ((GLError = glGetError()) != GL_NO_ERROR)
      {
        std::cout << "OpenGL Error (" << GLError << "): Function: " << functionName <<
          ", File: " << fileName << ", Line: " << fileLine << std::endl;

        error = true;
      }
    }

    return error;
  }

  /*Texture testTexture = Texture("./Assets/Texture/brick.png");
  Mesh square = Mesh(0, engine);
  Shader squareShader(0);*/

  //Floor For collision Test
  /*Texture testTexture1 = Texture("./Assets/Texture/dirt.png");
  Mesh floor = Mesh(0, engine);
  Shader floorShader(1);*/

  /* --------------------------------------
   Description: constuctor
   Params:
   Engine& engine1 - the engine
   --------------------------------------- */
  Renderer::Renderer(Engine& engine1):engine(engine1)
  {
    
  }

  /* --------------------------------------
   Description: deconstuctor
   Params:
   --------------------------------------- */
  Renderer::~Renderer()
  {

  }

  /* --------------------------------------
   Description: inits everything
   Params:
  
   --------------------------------------- */
  void Renderer::Initialize()
  {
    //glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glAlphaFunc(GL_LEQUAL, 0.0F);
  }

  /* --------------------------------------
   Description: updates what needs to be updated
   Params:
   --------------------------------------- */
  void Renderer::Update(float /*dt*/)
  {
    //(glClearColor(0.0f, 0.0f, 1.0f, 1.0f));
    //(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  }

  /* --------------------------------------
   Description: renders things to the screen
   Params:
   --------------------------------------- */
  void Renderer::Render()
  {
    Window* mainWindow = dynamic_cast<Window*>(engine.Get("Window"));
    //const auto to avoid copies from renderOrer
    for (auto& vectorDrawData : renderOrder)
    {
        for (const auto& drawData : vectorDrawData.second)
        {
            MeshManager* MeshMan = GetSystemFromEngine<MeshManager*>(engine, "MeshManager");
            Mesh& mesh = (*MeshMan)[drawData.meshIndex];
            // Get the camera and window.
            Camera* camera = dynamic_cast<Camera*>(engine.Get("Camera"));

            // Graphics things 
            glm::vec3 position3 = { drawData.position.x, drawData.position.y, 0.0f };
            glm::mat4 view = camera->calculateViewMatrix();
            glm::mat4 device = camera->calculateDeviceMatrix();

            // Matrix calculations.
            glm::mat4 model(1);
            model = glm::translate(glm::mat4(1.f), position3)
            * glm::rotate(glm::mat4(1.f), glm::radians(drawData.rotation), glm::vec3(0.0, 0.0, 1.0))
            * glm::scale(glm::mat4(1.f), glm::vec3(drawData.scale, 1.0f));


            ShaderManager* theShaderManager = GetSystemFromEngine<ShaderManager*>(engine, "ShaderManager");
            int shaderID = (*MeshMan)[drawData.meshIndex].GetShader();
            Shader& theShader = (*theShaderManager)[shaderID];

            theShader.UseShader();
            theShader.SetMatrix4f("device", device);
            theShader.SetMatrix4f("model", model);
            theShader.SetMatrix4f("view", view);
            theShader.SetVector4f("color", drawData.theColor); // add blend color
            theShader.SetVector2f("customU", drawData.theU);
            theShader.SetVector2f("customV", drawData.theV);
            theShader.SetVector2f("customMinUV", drawData.theMinUV);
            theShader.SetVector2f("customMaxUV", drawData.theMaxUV);

            // Use texture
            if (drawData.textureID > -1)
            {
                TextureManager* texMan = GetSystemFromEngine<TextureManager*>(engine, "TextureManager");
                Texture& tex = (*texMan)[drawData.textureID];
                tex.UseTexture();
            }
            // Draw the mesh
            mesh.RenderMesh();
        }
        vectorDrawData.second.clear();
    }
    //std::map<float, std::vector<DrawData>> renderOrder;

    //DebugLine(glm::vec2(0), glm::vec2(100));

    glUseProgram(0);

    //mainWindow->swapBuffers();
  }

  /* --------------------------------------
   Description: draws a square
   Params:
   const glm::vec2& position - posision of the draw
   const glm::vec2& scale - the size of the draw
   Mesh& mesh - the mesh used
   Texture* texture - the texture used
   --------------------------------------- */
  void Renderer::Draw(DrawData draw_data)
  {
      Camera* camera = dynamic_cast<Camera*>(engine.Get("Camera"));
      glm::vec2 cameraPos = camera->getCameraPosition();
      glm::vec2 cameraSize = camera->GetCameraSize();
      //slightly larger than camera to account for large object"Walls"
      cameraSize.x = cameraSize.x + 800;
      cameraSize.y = cameraSize.y + 800;
      //check if drawdata is in camera view
      //draw_data.position
      CollisionData theCollision = SimpleAABB(cameraPos, cameraSize, 
          draw_data.position, draw_data.scale);
      if (theCollision.has_collided)
      {
            renderOrder[draw_data.position.z].push_back(draw_data);
      }
      //float is the depth z value
      //vector is all the draws at location z

    //glEnable(GL_ALPHA_TEST);
    //glEnable(GL_BLEND);
    //glAlphaFunc(GL_LEQUAL, 0.0F);

    //glDisable(GL_BLEND);
    //glDisable(GL_ALPHA_TEST);
  }

  //from point and to point get mesh draw line
  void Renderer::DebugLine(glm::vec2 startpoint, glm::vec2 endpoint, glm::vec4 color)
  {

  // #ifdef _RELEASE
    MeshManager* MeshMan = GetSystemFromEngine<MeshManager*>(engine, "MeshManager");
    auto meshCheck = MeshMan->HasMesh("LineMesh");
    
    if (!meshCheck.first)
    {
    Deserializer* deserial = GetSystemFromEngine<Deserializer*>(engine, "Deserializer");
    meshCheck.second = MeshMan->GetNew();
    meshCheck.first = &(*MeshMan)[meshCheck.second];
    meshCheck.first->ReadFromFile(*deserial, "Data/LineMesh.json");
    }
    

    // Get the camera and window.
    Camera* camera = dynamic_cast<Camera*>(engine.Get("Camera"));

    // Graphics things 
    glm::vec3 position3 = { startpoint, 1000.0f };
    glm::mat4 view = camera->calculateViewMatrix();
    glm::mat4 device = camera->calculateDeviceMatrix();

    // Matrix calculations.
    glm::mat4 model(1);
    model = glm::scale(model, glm::vec3(endpoint - startpoint, 0.0f));
    model = glm::translate(glm::mat4(1), position3) * model;
    //model = glm::rotate(model, (rotation * toRadians), glm::vec3(0.0, 0.0, 1.0));


    ShaderManager* theShaderManager = GetSystemFromEngine<ShaderManager*>(engine, "ShaderManager");
    int shaderID = (*MeshMan)[meshCheck.second].GetShader();
    Shader& theShader = (*theShaderManager)[shaderID];

    DrawData debugLines;
    debugLines.position = position3;
    debugLines.scale = glm::vec2(endpoint - startpoint);
    debugLines.meshIndex = meshCheck.second;
    debugLines.textureID = -1;
    debugLines.theColor = { color };
    Draw(debugLines);

    //theShader.UseShader();
    //theShader.SetMatrix4f("device", device);
    //theShader.SetMatrix4f("model", model);
    //theShader.SetMatrix4f("view", view);
    //theShader.SetVector4f("color", color);
    
    //theShader.SetVector2f("customU", theU);
    //theShader.SetVector2f("customV", theV);

    //glLineWidth(1.0f);
    //meshCheck.first->RenderMesh();
    //glDrawArrays(GL_LINES, startPoint.x, 2);
  // #endif // DEBUG

  }

  void Renderer::DebugSquare(glm::vec2 position, glm::vec2 scale, glm::vec4 color)
  {
//#ifdef _DEBUG
    glm::vec2 bottomLeft = position - scale / 2.0f;
    glm::vec2 topLeft(0);
    topLeft.x = position.x - scale.x / 2.0f;
    topLeft.y = position.y + scale.y / 2.0f;
    glm::vec2 topRight = position + scale / 2.0f;
    glm::vec2 bottomRight(0);
    bottomRight.x = position.x + scale.x / 2.0f;
    bottomRight.y = position.y - scale.y / 2.0f;
    DebugLine(bottomLeft, topLeft, color);
    DebugLine(bottomLeft, bottomRight, color);
    DebugLine(topRight, topLeft, color);
    DebugLine(topRight, bottomRight, color);
//#endif
  }

  bool Renderer::UpdateWhilePaused() const
  {
      return true;
  }



} // End Of Dapper Namespace