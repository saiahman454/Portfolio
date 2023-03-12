//------------------------------------------------------------------------------
//
// File Name:	CameraAnchorManager.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "CameraAnchorManager.h"
#include "ComponentRegistry.h"
#include "Utility.h"
#include "Transform.h"
#include "Camera.h"
#include <cmath>

// Dapper Namespace!
namespace Dapper
{
    CameraAnchorManager::CameraAnchorManager(Engine& engine) :
        the_engine(engine)
    {
    }

    CameraAnchorManager::~CameraAnchorManager()
    {
    }

    void CameraAnchorManager::Initialize()
    {
        //register the deserialize function
        ComponentRegistry::FunctionBuffer CameraAnchorFunctions
        { CameraAnchorCreate, CameraAnchorDestroy, CameraAnchorGet };

        dynamic_cast<Deserializer*>(the_engine.Get("Deserializer"))->
            RegisterFunction("CameraAnchor", CameraAnchorDeserialize);

        dynamic_cast<ComponentRegistry*>
            (the_engine.Get("ComponentRegistry"))
            ->Register("CameraAnchor", CameraAnchorFunctions);
    }

    void CameraAnchorManager::Update(float dt)
    {
        for (CameraAnchor& theAnchor : anchor_list)
        {
            if (theAnchor.IsFollowing())
            {
                Transform* theTransform = GetComponentFromParent<Transform>
                    (the_engine, theAnchor.GetParentID(), "Transform");
                //lerp it eventually
                Camera* theCamera = GetSystemFromEngine<Camera*>
                    (the_engine, "Camera");
                auto trans = theTransform->GetTranslation();
                auto campos = theCamera->getCameraPosition();

                if (isLerping)
                {
                    float xlerp = std::lerp(campos.x, trans.x, 4.0f * dt);
                    float ylerp = std::lerp(campos.y, trans.y, 4.0f * dt);
                    // If the camera is too far away, snap to player
                    if (abs(xlerp - trans.x) > 300 || abs(ylerp - trans.y) > 300)
                    {
                        theCamera->setCameraPosition(trans);
                    }
                    else
                    {
                        theCamera->setCameraPosition({ xlerp,ylerp,0.1 });
                    }
                    previousLerp = theCamera->getCameraPosition();
                }
                // Only lerp the camera if the game is lerping
                else
                {
                    theCamera->setCameraPosition(previousLerp);
                }
            }
        }
    }

    void CameraAnchorManager::Render()
    {
    }

    int CameraAnchorManager::GetNew()
    {
        // Add the new anchor
        CameraAnchor newAnchor;
        int obj_ID = idManager.Create(anchor_list, newAnchor);
        anchor_list[idManager[obj_ID]].SetID(obj_ID);
        //anchor_list[idManager[obj_ID]].Init();
        return obj_ID;
    }

    std::pair<CameraAnchor*, int> CameraAnchorManager::HasCameraAnchor(std::string camera_name)
    {
        int count = 0;
        for (CameraAnchor& the_cameraAnchor : anchor_list)
        {
            if (the_cameraAnchor.GetName() == camera_name)
            {
                return { &the_cameraAnchor, count };
            }
            count++;
        }

        return { nullptr, -1 };
    }

    void CameraAnchorManager::Destroy(int ID)
    {
        idManager.Destroy(anchor_list, ID);
    }

    void CameraAnchorManager::SetIsLerping(bool _isLerping)
    {
        isLerping = _isLerping;
    }

    int CameraAnchorManager::GetIsLerping()
    {
        return isLerping;
    }

    bool CameraAnchorManager::UpdateWhilePaused() const
    {
        return false;
    }

    std::pair<std::string_view, int> CameraAnchorDeserialize(Engine& engine, const rapidjson::Value& val)
    {
        CameraAnchorManager* manager = dynamic_cast<CameraAnchorManager*>
            (engine.Get("CameraAnchorManager"));
        Deserializer* deserial = dynamic_cast<Deserializer*>
            (engine.Get("Deserializer"));
        int ID = manager->GetNew();
        CameraAnchor& toReturn = (*manager)[ID];
        toReturn.Read(*deserial, val);
        return { toReturn.GetName(), ID };
    }

    CameraAnchor* CameraAnchorCreate(Engine& engine)
    {
        CameraAnchorManager& the_man = *dynamic_cast<CameraAnchorManager*>
            (engine.Get("CameraAnchorManager"));
        int index = the_man.GetNew();
        return &the_man[index];
    }

    void CameraAnchorDestroy(int ID, Engine& engine)
    {
        CameraAnchorManager& the_man = *dynamic_cast<CameraAnchorManager*>
            (engine.Get("CameraAnchorManager"));
        the_man.Destroy(ID);
    }

    CameraAnchor* CameraAnchorGet(int ID, Engine& engine)
    {
        CameraAnchorManager& the_man = *dynamic_cast<CameraAnchorManager*>
            (engine.Get("CameraAnchorManager"));
        return &the_man[ID];
    }

} // End Of Dapper Namespace