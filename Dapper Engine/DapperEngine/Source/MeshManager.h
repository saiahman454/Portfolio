//------------------------------------------------------------------------------
//
// File Name:	MeshManager.h
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
#include "Camera.h"
#include "Messenger.h"
#include "rapidjson/document.h"
#include "IDManager.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
    class MeshManager : public ITypeManager<Mesh>
    {
    public:
        MeshManager(Engine& engine);
        ~MeshManager()override;

        inline const std::string& Name()const override { return name; }

        void Initialize()override;

        void Update(float dt) override;

        void Render() override;

        Mesh& operator[](int index) { return mesh_list[ID_manager[index]]; }

        int GetNew();

        Mesh& operator[](std::string mesh_name);

        std::pair<Mesh*, int> HasMesh(std::string mesh_name);

        void RegisterFunction(Messenger<MeshManager&>::func function);

    private:
        const inline static std::string name = "MeshManager";
        std::vector<Mesh> mesh_list;
        Engine& the_engine;

        IDManager ID_manager;

        Messenger<MeshManager&> on_reallocate;

        // Inherited via ITypeManager
        virtual bool UpdateWhilePaused() const override;

    };//End of MeshManager class

    std::pair<std::string_view, int> MeshDeserialize(Engine& engine, const rapidjson::Value& val);

} // End Of Dapper Namespace