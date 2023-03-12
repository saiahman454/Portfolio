//------------------------------------------------------------------------------
//
// File Name:	SceneManager.h
// Author(s):	Adam Tackett
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
#include "Engine.h"
#include "Scene.h"
#include "IDManager.h"
#include "Messenger.h"
#include "GameObject.h"
#include <stack>
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

    enum class ConditionCheck
    {
        push,
        pop,
        none
    };

  class SceneManager : public ISystem{
  public:

    SceneManager(Engine& my_engine);

    ~SceneManager() override;

    // Inherited via ISystem
    virtual const std::string& Name() const override;
    virtual void Initialize() override;
    virtual void Update(float dt) override;
    virtual void Render() override;
    virtual bool UpdateWhilePaused() const override;

    int FadeToScene(std::string_view currentScene, std::string_view toLoad, ConditionCheck pushOrPop);
    int LoadScene(std::string_view toLoad);
    void UnloadScene(std::string_view toUnload);
    void UnloadScene(int toUnload);

    Scene& GetScene(int theId);
    Scene& GetScene(std::string_view theName);
    int GetSceneID(std::string_view theName);
    int GetNew(const std::string& name);

    void PauseAllScenes(); //Go through each object and set there pauses Icomponent and GameObject
    void UnPauseAllScenes();
    void UnloadAllScenes();

    void ForceLoadScene(std::string_view toLoad);
    
    void RegisterOnChange(std::function<void(void)> func);
    unsigned Size();
    Scene& GetSceneAtIndex(int index);
    int MakeNewScene();
    int SceneInCharge(int gameObjectId);

    void setEnableSceneAll(bool condition);
    void setEnableScene(std::string sceneToEnable, bool condition);

    bool IsSceneLoaded(std::string name);

    void PushSceneStates();
    void PopSceneStates();


  private:
    
    void LoadSceneFromFilename(const std::string& toLoad);

    const std::string name = "SceneManager";
    Engine& my_engine;

    // For fading out a scene
    bool isFadingOut = false;
    bool isFadingIn = false;
    bool switchScenes = false;
    ConditionCheck push_or_pop = ConditionCheck::none;
    float currentFadeTransparency = 0.0f;
    float timeSpentFading = 0.5f;
    std::string currentScene;
    std::string_view sceneToLoad;
    int fadeObjectID = -1;

    struct sceneState {
        int sceneID;
        bool isPaused;
        bool isEnabled;
    };

    IDManager ID_Manager;
    std::stack<std::vector<sceneState>> sceneStates;
    std::vector<Scene> theScenes;
    std::vector<std::string> theFileNames;
    std::vector<int> theFileToUnload;
    Messenger<> on_change;

  };

} // End Of Dapper Namespace

//------------------------------------------------------------------------------


