//------------------------------------------------------------------------------
//
// File Name:	PhysicsManager.h
// Author(s):	Isaiah Dickison
// Author(s):	Isaiah Dickison
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

#include "ITypeManager.h"
#include "Physics.h"
#include "IDManager.h"
#include "Messenger.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
    class PhysicsManager : public ITypeManager<Physics>
    {
    public:

        PhysicsManager(Engine& engine);
        ~PhysicsManager() override;

        inline const std::string & Name()const override{ return name; };

        inline Physics& operator[] (int index) override { return physics[ID_manager[index]]; };

        void Initialize()override;

        void Update(float dt) override;

        void Render() override; 

        int GetNew() override;

        void Destroy(int ID);

        inline Engine& getEngine(){ return my_engine; };

        void RegisterOnChange(Messenger<PhysicsManager&>::func MessengerFunction);

        Physics* Has(int ID);

        void ToggleDebugDraw();

    private:


        Messenger<PhysicsManager&> on_change;
        const std::string name = "PhysicsManager";
        Engine& my_engine;
        std::vector<Physics> physics;
        IDManager ID_manager;
        bool debug_draw = false;

        // Inherited via ITypeManager
        inline virtual bool UpdateWhilePaused() const override{ return false; };

    };//End of PhysicsManager class
    std::pair<std::string_view, int> PhysicsDeserialize(Engine& engine, const rapidjson::Value& val);
    
    Physics* PhysicsCreate(Engine& engine);
    void PhysicsDestroy(int ID, Engine& engine);
    Physics* PhysicsGet(int ID, Engine& engine);
    


} // End Of Dapper Namespace