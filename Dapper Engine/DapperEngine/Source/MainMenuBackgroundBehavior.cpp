//------------------------------------------------------------------------------
//
// File Name:	MainMenuBackgroundBehavior.cpp
// Author(s):	Natalie Bouley
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "MainMenuBackgroundBehavior.h"
#include "AnimationSpine.h"
#include "SpineData.h"
#include "Utility.h"

// Dapper Namespace!
namespace Dapper
{
    MainMenuBackgroundBehavior::MainMenuBackgroundBehavior(int ID, Engine* theEngine) :
        engine(theEngine),
        paused(false),
        my_ID(ID),
        parent_ID(-1)
    {
    }

    MainMenuBackgroundBehavior::~MainMenuBackgroundBehavior()
    {
    }

    void MainMenuBackgroundBehavior::Init()
    {
        AnimationSpine& animation = *GetComponentFromParent<AnimationSpine>(*engine, parent_ID, "AnimationSpine");

        if (&animation)
        {
            spine::TrackEntry* current_animation = animation.GetInstance()->GetAnimationState()->getCurrent(0);
            animation.GetInstance()->GetAnimationState()->setAnimation(0, "MainMenu", true);

            //SpineData* instance = animation.GetData();
            //spine::SkeletonData* skeletondata = instance->GetSkeletonData();
            //spine::BoneData root = skeletondata->findBone("root");
            //root.scaleX = 2;
            //root.scaleY = 2;
        }
    }

    void MainMenuBackgroundBehavior::Update(float dt)
    {
    }

    void MainMenuBackgroundBehavior::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
    {
        deserial;
    }

    void MainMenuBackgroundBehavior::Write(IJsonWriter& writer) const
    {
        writer.WriteKey("MainMenuBackgroundBehavior");
        writer.StartObject();
        writer.EndObject();
    }

    void MainMenuBackgroundBehavior::ImGuiDisplay(float scale)
    {
    }
} // End Of Dapper Namespace