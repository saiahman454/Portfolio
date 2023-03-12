//------------------------------------------------------------------------------
//
// File Name:	TotalCollectableManager.h
// Author(s):	
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "ISystem.h"
#include "TotalCollectable.h"
#include "ITypeManager.h"
#include "IDManager.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
    class TotalCollectableManager : public ITypeManager<TotalCollectable>
    {
    public:
        TotalCollectableManager(Engine& engine);
        ~TotalCollectableManager()override;

        inline const std::string& Name()const override { return name; };

        void Initialize()override;

        void Update(float dt) override;

        void Render() override;

        bool UpdateWhilePaused() const override;

        void Destroy(int ID);

        TotalCollectable& operator[](int index) override;

        int GetNew() override;

    private:
        const std::string name = "TotalCollectableManager";
        Engine& my_engine;
        std::vector<TotalCollectable> totals;
        IDManager ID_manager;


    };//End of TotalCollectableManager class

    std::pair<std::string_view, int> TotalCollectableDeserialize(Engine& engine, const rapidjson::Value& val);

    TotalCollectable* TotalCollectableCreate(Engine& engine);
    void TotalCollectableDestroy(int ID, Engine& engine);
    TotalCollectable* TotalCollectableGet(int ID, Engine& engine);


} // End Of Dapper Namespace