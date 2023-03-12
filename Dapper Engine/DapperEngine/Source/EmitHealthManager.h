//------------------------------------------------------------------------------
//
// File Name:	EmitHealthManager.h
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
#include "Mesh.h"
#include "Engine.h"
#include "EmitHealth.h"
#include "IDManager.h"
#include "rapidjson/document.h"
#include <vector>
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
    class EmitHealthManager : public ITypeManager<EmitHealth>
    {
    public:
        EmitHealthManager(Engine& engine);
        ~EmitHealthManager()override;

        const std::string & Name()const override;
        void Initialize()override;
        void Update(float dt) override;
        void Render() override;
        EmitHealth& operator[](int index);
        int GetNew();
        std::pair<EmitHealth*, int> HasEmitHealth(std::string emit_health_name);
        void Destroy(int ID);

    private:
        const static inline std::string name = "EmitHealthManager";
        std::vector<EmitHealth> emit_health_list;
        Engine& the_engine;
        IDManager idManager;

        // Inherited via ITypeManager
        virtual bool UpdateWhilePaused() const override;

    };//End of EmitHealthManager class

    std::pair<std::string_view, int> EmitHealthDeserialize(Engine& engine, const rapidjson::Value& val);

    EmitHealth* EmitHealthCreate(Engine& engine);
    void EmitHealthDestroy(int ID, Engine& engine);
    EmitHealth* EmitHealthGet(int ID, Engine& engine);
} // End Of Dapper Namespace