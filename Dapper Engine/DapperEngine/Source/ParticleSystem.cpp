//------------------------------------------------------------------------------
//
// File Name:	ParticleSystem.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "ParticleSystem.h"
#include "MeshManager.h"
#include "TextureManager.h"

#include "Window.h"
#include "Camera.h"
#include "Renderer.h"
#include "ShaderManager.h"

#include <glm\mat4x4.hpp>
#include "glm/glm.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include "Utility.h"

#include "ParticleEmitter.h"

// Dapper Namespace!
namespace Dapper
{
/*--------------------------------------
Description: ParticleSystem constructor
Params:
unsigned int theAmount - The number of particle systems to add
---------------------------------------*/
  ParticleSystem::ParticleSystem(unsigned int theAmount, Engine& engine1)
    : amount(theAmount), engine(engine1)
  {
  }

/*--------------------------------------
Description: ParticleSystem destructor
---------------------------------------*/
  ParticleSystem::~ParticleSystem()
  {
  }

/*--------------------------------------
Description: ParticleSystem Initalization Loads the textures for the particles.
---------------------------------------*/
  void ParticleSystem::Initialize()
  {
    MeshManager* theMeshManager = dynamic_cast<MeshManager*>(engine.Get("MeshManager"));
    std::pair<Mesh*, int> theMesh = theMeshManager->HasMesh("SquareMesh");
    if (theMesh.first == NULL)
    {
      int index = theMeshManager->GetNew();
      //Using a reference to get index of mesh we need from manager
      (*theMeshManager)[index].ReadFromFile(*(Deserializer*)engine.Get("Deserializer"),
        "Data/SquareMesh.json");
      //give mesh the address containing mesh we just made
      meshID = index;
    }
    else
    {
      meshID = theMesh.second;
    }

    // Use the texture that the system now has
    // The following code should be commented out when different textures for particles is supported:
    //TextureManager* theTextureManager = dynamic_cast<TextureManager*>
    //  (engine.Get("TextureManager"));
    //std::pair<Texture*, int> theTexture2 =
    //  theTextureManager->HasTexture("./Assets/Texture/UI_Img_03.png");
    //if (!theTexture2.first)
    //{
    //  theTextureManager->LoadNewTexture("./Assets/Texture/UI_Img_03.png");
    //}
    //std::pair<Texture*, int> theTexture = theTextureManager->HasTexture("./Assets/Texture/UI_Img_03.png");
    //textureID = theTexture.second;

    // create this->amount default particle instances
    particles.reserve(amount);
  }

/*--------------------------------------
Description: ParticleSystem Update all particles virtual call
Params:
float dt - The time.
---------------------------------------*/
  void ParticleSystem::Update(float dt)
  {
    UpdateParticle(dt);
  }

/*--------------------------------------
Description: Render all the particles by calling draw.
---------------------------------------*/
  void ParticleSystem::Render()
  {
    MeshManager* MeshMan = GetSystemFromEngine<MeshManager*>
      (engine, "MeshManager");
    Renderer* renderer = GetSystemFromEngine<Renderer*>
      (engine, "Renderer");
    ShaderManager* theShaderManager = GetSystemFromEngine<ShaderManager*>
      (engine, "ShaderManager");
    int shaderID = (*MeshMan)[meshID].GetShader();
    Shader& theShader = (*theShaderManager)[shaderID];

    //Range based forloop through the array
    for (const std::vector<Particle>& theParticleArray : particles)
    {
      //Range based forloop through the particle vector
      for (const Particle& theParticles : theParticleArray)
      {
        if (theParticles.Life > 0.0f)
        {
            //ParticleEmitter* theEmitter = GetComponentFromParent<ParticleEmitter>
            //    (engine, parentID, "ParticleEmitter");

          //theShader.SetVector3f("offset", particle.Position);
          //theShader.SetVector4f("color", theParticles.Color);
          glm::vec2 scale(25);
          DrawData draw_data;
          draw_data.position = theParticles.Position;
          draw_data.scale = scale;
          draw_data.meshIndex = meshID;
          draw_data.rotation = 0;
          draw_data.theColor = theParticles.Color;
          draw_data.textureID = textureID;
          renderer->Draw(draw_data);
        }
      }
    }

    //Don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

/*--------------------------------------
Description: ParticleSystem Update all particles
Params:
float dt - The time.
---------------------------------------*/
  void ParticleSystem::UpdateParticle(float dt)
  {
    for (unsigned int i = 0; i < particles.size(); ++i)
    {
      for (unsigned int j = 0; j < particles[i].size(); ++j)
      {
        Particle& p = particles[i][j];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
          p.Position -= glm::vec3(p.Velocity.x * dt, p.Velocity.y * dt, 0);
          p.Color.a -= dt * 0.075f; //Fade
        }
        else
        {
          std::swap(p, particles[i].back());
          particles[i].pop_back();
          --j;
        }
      }
    }
  }

/*--------------------------------------
Description: Grab particles.
Params:
int howManyBro - How many particles to get.
---------------------------------------*/
  int ParticleSystem::GiveMeParticles(int howManyBro)
  {
    //Create Vector of particles with X#
    std::vector<Particle> makeParticles(howManyBro);
    //prevent copy from being made with std::move
    particles.push_back(std::move(makeParticles));
    //return index that was made
    return (int)particles.size() - 1;
  }

/*--------------------------------------
Description: Reset the particle //Not currently used
Params:
Particle& particle - The particle to reset
glm::vec3 translation - The position to place it at
glm::vec2 offset - How much to offset it by
---------------------------------------*/
  void ParticleSystem::respawnParticle(Particle& particle, glm::vec3 translation,
    glm::vec2 offset)
  {
    offset;
    particle;
    float random = ((rand() % 100) - 50) / 10.0f;
    float rColor = 0.5f + ((rand() % 100) / 100.0f);

    particle.Position = translation + random + glm::vec3(offset, 0);
    particle.Position = glm::vec3(0.0f) + random + glm::vec3(offset, 0);

    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 10.0f;
    particle.Velocity = glm::vec2(random * 0.1f, random * 0.1f);
  }

/*--------------------------------------
Description: Whether if this should be updating while paused
Return: Always true
---------------------------------------*/
  bool ParticleSystem::UpdateWhilePaused() const
  {
    return true;
  }

} // End Of Dapper Namespace