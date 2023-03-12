//------------------------------------------------------------------------------
//
// File Name:	HealthManager.h
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
#include "Health.h"
#include "IDManager.h"
#include "rapidjson/document.h"
#include <vector>
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
    class HealthManager : public ITypeManager<Health>
    {
    public:
        HealthManager(Engine& engine);
        ~HealthManager()override;

        const std::string & Name()const override;
        void Initialize()override;
        void Update(float dt) override;
        void Render() override;
        Health& operator[](int index);
        int GetNew();
        std::pair<Health*, int> HasHealth(std::string health_name);
        void Destroy(int ID);

    private:
        const static inline std::string name = "HealthManager";
        std::vector<Health> health_list;
        Engine& the_engine;
        IDManager idManager;

        // Inherited via ITypeManager
        virtual bool UpdateWhilePaused() const override;

    };//End of HealthManager class

    std::pair<std::string_view, int> HealthDeserialize(Engine& engine, const rapidjson::Value& val);

    Health* HealthCreate(Engine& engine);
    void HealthDestroy(int ID, Engine& engine);
    Health* HealthGet(int ID, Engine& engine);
} // End Of Dapper Namespace