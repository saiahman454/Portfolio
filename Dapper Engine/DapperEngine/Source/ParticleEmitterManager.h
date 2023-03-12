//------------------------------------------------------------------------------
//
// File Name:	ParticleEmitterManager.h
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "ITypeManager.h"
#include "Engine.h"
#include "ParticleEmitter.h"
#include <rapidjson/document.h>
#include "IDManager.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
    class ParticleEmitterManager : public ITypeManager<ParticleEmitter>
    {
    public:
        ParticleEmitterManager(Engine &engine);
        ~ParticleEmitterManager()override;

        const std::string & Name()const override;
        void Initialize()override;
        void Update(float dt) override;
        void Render() override;

        ParticleEmitter& operator[](int index);
        int GetNew();
        void Destroy(int ID);

    private:
        const std::string name = "ParticleEmitterManager";
        std::vector<ParticleEmitter> particleEmitters;
        Engine& theEngine;
        IDManager ID_manager;
        int meshID = -1;


        // Inherited via ITypeManager
        virtual bool UpdateWhilePaused() const override;

    };//End of ParticleEmitterManager class

    std::pair<std::string_view, int> ParticleEmitterDeserialize
    (Engine& engine, const rapidjson::Value& val);

    ParticleEmitter* ParticleEmitterCreate(Engine& engine);
    void ParticleEmitterDestroy(int ID, Engine& engine);
    ParticleEmitter* ParticleEmitterGet(int ID, Engine& engine);

} // End Of Dapper Namespace