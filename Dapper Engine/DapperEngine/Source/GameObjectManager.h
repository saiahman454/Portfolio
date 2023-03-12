//------------------------------------------------------------------------------
//
// File Name:	GameObjectManager.h
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
#include "vector"
#include "GameObject.h"
#include "Engine.h"
#include "rapidjson/document.h"
#include "IDManager.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
    class GameObjectManager : public ITypeManager<GameObject>
    {
    public:
        GameObjectManager(Engine& engine);
        ~GameObjectManager()override;

        inline const std::string & Name()const override{ return name; };


        void Initialize()override;

        void Update(float dt) override;

        void Render() override;


        inline GameObject& operator[] (int ID)override{ return game_objects[ID_manager[ID]]; };

        GameObject& operator[] (const std::string& name);
        GameObject* GetObjectAtIndex(int index);

        int GetNew();

        inline int Size(){ return (int)game_objects.size(); };

        int MakeNewWithName(std::string obj_name);

        void DeleteGameObject(int index);

        void DeleteAllGameObjects();

        void WriteAllGameObjects(IJsonWriter& writer);

        const std::vector<GameObject>& GetAllGameObjects();

        int MakePrefabWithName(std::string_view prefab_name);

        int FindIndex(const std::string& name);

        int FindObjectWithTag(std::string_view tag) const;

        void SortGameObjects();
        void OnObjectRename();
        void SetGameObjectPauseState(bool Condition, int iD);

        bool HasObject(int gameObject)const;

    private:
        const std::string name = "GameObjectManager";

        std::vector<int> toDestroyIds;

        std::vector<GameObject> game_objects;
        std::vector<const char *> game_object_names;

        bool sort_objects;

        Engine& my_engine;

        IDManager ID_manager;

        // Inherited via ITypeManager
        inline virtual bool UpdateWhilePaused() const override{ return true; };

        std::string NameObjectFromPrefab(std::string_view prefab_name);

    };//End of PhysicsManager class

    std::pair<std::string_view, int> GameObjectDeserialize(Engine& engine, const rapidjson::Value& val);
    std::pair<std::string_view, int> GameObjectArrayDeserialize(Engine& engine, const rapidjson::Value& val);

} // End Of Dapper Namespace