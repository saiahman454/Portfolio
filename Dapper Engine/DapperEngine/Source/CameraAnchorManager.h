//------------------------------------------------------------------------------
//
// File Name:	CameraAnchorManager.h
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
#include "CameraAnchor.h"
#include "IDManager.h"
#include "rapidjson/document.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
    class CameraAnchorManager : public ITypeManager<CameraAnchor>
    {
    public:
        CameraAnchorManager(Engine& engine);
        ~CameraAnchorManager()override;

        inline const std::string& Name()const override { return name; }
        void Initialize()override;
        void Update(float dt) override;
        void Render() override;
        inline CameraAnchor& operator[](int index) { return anchor_list[idManager[index]]; }
        int GetNew();
        std::pair<CameraAnchor*, int> HasCameraAnchor(std::string camera_name);
        void Destroy(int ID);
        void SetIsLerping(bool _isLerping);
        int GetIsLerping();

    private:
        const static inline std::string name = "CameraAnchorManager";
        std::vector<CameraAnchor> anchor_list;
        Engine& the_engine;
        IDManager idManager;
        bool isLerping = true;
        glm::vec3 previousLerp = { 0.0,0.0,0.1 };

        // Inherited via ITypeManager
        virtual bool UpdateWhilePaused() const override;

    };//End of CameraAnchorManager class

    std::pair<std::string_view, int> CameraAnchorDeserialize(Engine& engine, const rapidjson::Value& val);

    CameraAnchor* CameraAnchorCreate(Engine& engine);
    void CameraAnchorDestroy(int ID, Engine& engine);
    CameraAnchor* CameraAnchorGet(int ID, Engine& engine);
} // End Of Dapper Namespace