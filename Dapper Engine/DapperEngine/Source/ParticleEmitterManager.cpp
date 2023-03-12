//------------------------------------------------------------------------------
//
// File Name:	ParticleEmitterManager.cpp
// Author(s):   Adam Tackett	
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "ParticleEmitterManager.h"
#include "Deserializer.h"
#include "ComponentRegistry.h"

#include "MeshManager.h"
#include "TextureManager.h"
#include "Renderer.h"
#include "ShaderManager.h"
#include "Utility.h"

// Dapper Namespace!
namespace Dapper 
{
    ParticleEmitterManager::ParticleEmitterManager(Engine& engine) : theEngine(engine)
    {
    }

    ParticleEmitterManager::~ParticleEmitterManager()
    {
    }

    const std::string& ParticleEmitterManager::Name() const
    {
        return name;
    }

    void ParticleEmitterManager::Initialize()
    {
        ComponentRegistry::FunctionBuffer ParticleEmitterFunctions
        { ParticleEmitterCreate, ParticleEmitterDestroy, ParticleEmitterGet };

        dynamic_cast<Deserializer*>(theEngine.Get("Deserializer"))->
            RegisterFunction("ParticleEmitter", ParticleEmitterDeserialize);

        dynamic_cast<ComponentRegistry*>
            (theEngine.Get("ComponentRegistry"))
            ->Register("ParticleEmitter", ParticleEmitterFunctions);

        MeshManager* theMeshManager = dynamic_cast<MeshManager*>(theEngine.Get("MeshManager"));
        std::pair<Mesh*, int> theMesh = theMeshManager->HasMesh("SquareMesh");
        if (theMesh.first == NULL)
        {
            int index = theMeshManager->GetNew();
            //Using a reference to get index of mesh we need from manager
            (*theMeshManager)[index].ReadFromFile(*(Deserializer*)theEngine.Get("Deserializer"),
                "Data/SquareMesh.json");
            //give mesh the address containing mesh we just made
            meshID = index;
        }
        else
        {
            meshID = theMesh.second;
        }
    }

    void ParticleEmitterManager::Update(float dt)
    {
        for (auto& ParticlesEmitter : particleEmitters)
        {
            if (ParticlesEmitter.GetPause() == false)
            {
                ParticlesEmitter.Update(dt);
            }
        }
    }

    void ParticleEmitterManager::Render()
    {
        MeshManager* MeshMan = GetSystemFromEngine<MeshManager*>
            (theEngine, "MeshManager");
        Renderer* renderer = GetSystemFromEngine<Renderer*>
            (theEngine, "Renderer");
        ShaderManager* theShaderManager = GetSystemFromEngine<ShaderManager*>
            (theEngine, "ShaderManager");
        int shaderID = (*MeshMan)[meshID].GetShader();
        Shader& theShader = (*theShaderManager)[shaderID];
        

        //Range based forloop through the array
        for (const ParticleEmitter& particleEmit : particleEmitters)
        {
            if (particleEmit.isEnabled)
            {
                std::vector<Particle>const& theParticles = particleEmit.GetParticle();
                //Range based forloop through the particle vector
                for (const Particle& particle : theParticles)
                {
                    if (particle.Life > 0.0f)
                    {
                      //theShader.SetVector3f("offset", particle.Position);
                      //theShader.SetVector4f("color", theParticles.Color);
                        DrawData draw_data;
                        draw_data.position = particle.Position;
                        draw_data.scale = particle.size;
                        draw_data.meshIndex = meshID;
                        draw_data.rotation = 0;
                        draw_data.theColor = particle.Color;
                        draw_data.textureID = particle.textuere_id;
                        renderer->Draw(draw_data);
                    }
                }
                std::vector<Particle>const& trail_particles = particleEmit.GetParticleTrail();
                //Range based forloop through the particle vector
                for (const Particle& particle : trail_particles)
                {
                  if (particle.Life > 0.0f)
                  {
                    //theShader.SetVector3f("offset", particle.Position);
                    //theShader.SetVector4f("color", theParticles.Color);
                    DrawData draw_data;
                    draw_data.position = particle.Position;
                    draw_data.scale = particle.size;
                    draw_data.meshIndex = meshID;
                    draw_data.rotation = 0;
                    draw_data.theColor = particle.Color;
                    draw_data.textureID = particleEmit.GetTextureId();
                    renderer->Draw(draw_data);
                  }
                }
            }
        }
    }

    ParticleEmitter& ParticleEmitterManager::operator[](int ID)
    {
        return particleEmitters[ID_manager[ID]];
    }

    int ParticleEmitterManager::GetNew()
    {
        ParticleEmitter new_obj(&theEngine);
        int obj_ID = ID_manager.Create(particleEmitters, new_obj);
        particleEmitters[ID_manager[obj_ID]].SetID(obj_ID);
        return obj_ID;
    }

    void ParticleEmitterManager::Destroy(int ID)
    {
      ID_manager.Destroy(particleEmitters, ID);
    }

    bool ParticleEmitterManager::UpdateWhilePaused() const
    {
        return true;
    }

    std::pair<std::string_view, int> ParticleEmitterDeserialize
                                (Engine& engine, const rapidjson::Value& val)
    {
        ParticleEmitterManager* manager =
            dynamic_cast<ParticleEmitterManager*>(engine.Get("ParticleEmitterManager"));

        Deserializer* deserial = dynamic_cast<Deserializer*>(engine.Get("Deserializer"));

        int index = manager->GetNew();
        ParticleEmitter& new_Emitter = (*manager)[index];
        new_Emitter.Read(*deserial, val);
        return { new_Emitter.GetName(), index };

    }

    ParticleEmitter* ParticleEmitterCreate(Engine& engine)
    {
        ParticleEmitterManager& the_man = *dynamic_cast<ParticleEmitterManager*>
            (engine.Get("ParticleEmitterManager"));
        int index = the_man.GetNew();
        return &the_man[index];
    }

    void ParticleEmitterDestroy(int ID, Engine& engine)
    {
        ParticleEmitterManager& the_man = *dynamic_cast<ParticleEmitterManager*>
            (engine.Get("ParticleEmitterManager"));
        the_man.Destroy(ID);
    }
    ParticleEmitter* ParticleEmitterGet(int ID, Engine& engine)
    {
        ParticleEmitterManager& the_man = *dynamic_cast<ParticleEmitterManager*>
            (engine.Get("ParticleEmitterManager"));
        return &the_man[ID];
    }
} // End Of Dapper Namespace