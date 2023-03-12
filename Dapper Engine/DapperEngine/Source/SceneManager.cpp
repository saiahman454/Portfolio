//------------------------------------------------------------------------------
//
// File Name:	SceneManager.cpp
// Author(s):	Adam Tackett
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "SceneManager.h"
#include "Deserializer.h"
#include "Utility.h"
#include "Input.h"
#include "Sprite.h"
#include "Window.h"
#include "Ambience.h"
#include "BGMController.h"
#include "CameraAnchorManager.h"

// Dapper Namespace!
namespace Dapper {
    SceneManager::SceneManager(Engine& my_engine) :
        my_engine(my_engine)
    {
    }

    SceneManager::~SceneManager()
    {
    }

    const std::string& SceneManager::Name() const
    {
        return name;
    }

    void SceneManager::Initialize()
    {
    }

    void SceneManager::Update(float dt)
    {
        // If fading bool, fade the screen
        if (isFadingOut)
        {
            assert(isFadingIn == false);
            currentFadeTransparency += dt;

            Sprite& fade_sprite = *GetComponentFromParent<Sprite>(my_engine, fadeObjectID, "Sprite");
            fade_sprite.SetTint(glm::vec4(0.0, 0.0, 0.0, currentFadeTransparency / timeSpentFading)); // Scale timer to transparency

            if (currentFadeTransparency >= timeSpentFading)
            {
                // Start at the top, since another object at transparency 1 will be created
                currentFadeTransparency = timeSpentFading;

                // Unload old scene and delete fade object
                UnloadScene(GetSceneID(currentScene));

                //int howMany = theFileToUnload.size();
                //for (int i = 0; i < howMany; i++)
                //{
                //    if (ID_Manager.Has(theFileToUnload[i]))
                //    {
                //        theScenes[ID_Manager[theFileToUnload[i]]].UnLoad();
                //        ID_Manager.Destroy(theScenes, theFileToUnload[i]);
                //        //remove the ID after it is unloaded
                //        on_change.SendMessage();
                //    }
                //}
                //theFileToUnload.clear();

                GameObjectManager& object_manager = *(GetSystemFromEngine<GameObjectManager*>(my_engine, "GameObjectManager"));
                object_manager.DeleteGameObject(fadeObjectID);

                fadeObjectID = object_manager.MakePrefabWithName("SceneFade");


                // Load the new scene
                LoadScene(sceneToLoad);

                // Load Fade object from prefab with 0 transparency
                assert(fadeObjectID != -1);

                // Set tint to 1.0, set flag to fade in
                Sprite& fade_sprite = *GetComponentFromParent<Sprite>(my_engine, fadeObjectID, "Sprite");
                fade_sprite.SetTint(glm::vec4(0.0, 0.0, 0.0, 1.0));
                isFadingIn = true;
                isFadingOut = false;
            }
        }
        if (isFadingIn)
        {
            assert(isFadingOut == false);
            assert(fadeObjectID != -1);

            currentFadeTransparency -= dt;

            GameObjectManager& object_manager = *(GetSystemFromEngine<GameObjectManager*>(my_engine, "GameObjectManager"));
            fadeObjectID = object_manager.FindObjectWithTag("SceneFade");

            Sprite& fade_sprite = *GetComponentFromParent<Sprite>(my_engine, fadeObjectID, "Sprite");
            fade_sprite.SetTint(glm::vec4(0.0, 0.0, 0.0, currentFadeTransparency * 1.4)); // Scale timer to transparency

            if (currentFadeTransparency <= 0.0f)
            {
                // Push scene states
                if (push_or_pop == ConditionCheck::pop)
                    PopSceneStates();

                object_manager.DeleteGameObject(fadeObjectID);
                fadeObjectID = -1;
                isFadingIn = false;
                isFadingOut = false;
            }
        }

        //UnLoad First
        int howMany = theFileToUnload.size();
        for (int i = 0; i < howMany; i++)
        {
            if (ID_Manager.Has(theFileToUnload[i]))
            {
                theScenes[ID_Manager[theFileToUnload[i]]].UnLoad();
                ID_Manager.Destroy(theScenes, theFileToUnload[i]);
                //remove the ID after it is unloaded
                on_change.SendMessage();
            }
        }
        theFileToUnload.clear();

        howMany = theFileNames.size();
        for (int i = 0; i < howMany; i++)
        {
            std::string name(theFileNames[i]);
            std::string theName = "Data/Scenes/" + name + ".json";
            Scene boi(my_engine);

            ID_Manager.Create(theScenes, std::move(boi));

            theScenes.back().Read(theName);
            theScenes.back().SetName(name);
            on_change.SendMessage();
        }
        //all names loaded so clear it
        theFileNames.clear();
        if (my_engine.IsGamePaused())
        {
            Window& winder = *GetSystemFromEngine<Window*>(my_engine, "Window");
            glfwSetInputMode(winder.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            if (theScenes.size() == 1)
            {
                if (IsSceneLoaded("Level1") || IsSceneLoaded("Level2") || IsSceneLoaded("Level3"))
                {
                    Window& winder = *GetSystemFromEngine<Window*>(my_engine, "Window");
                    glfwSetInputMode(winder.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

                    CameraAnchorManager& cameraanch_manager = *GetSystemFromEngine<CameraAnchorManager*>(my_engine, "CameraAnchorManager");
                    cameraanch_manager.SetIsLerping(true);
                }
                else
                {
                    Window& winder = *GetSystemFromEngine<Window*>(my_engine, "Window");
                    glfwSetInputMode(winder.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                }
            }
            else
            {
                Window& winder = *GetSystemFromEngine<Window*>(my_engine, "Window");
                glfwSetInputMode(winder.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

                CameraAnchorManager& cameraanch_manager = *GetSystemFromEngine<CameraAnchorManager*>(my_engine, "CameraAnchorManager");
                cameraanch_manager.SetIsLerping(false);
            }
        }

    }

    void SceneManager::Render()
    {
    }
    bool SceneManager::UpdateWhilePaused() const
    {
        return true;
    }

    int SceneManager::LoadScene(std::string_view toLoad)
    {
        auto foundIt = std::find(theFileNames.begin(), theFileNames.end(), toLoad);

        auto foundScene = std::find_if(theScenes.begin(), theScenes.end(),
            [toLoad](const Scene& scene) {
                if (scene.GetName() == toLoad)
                    return true;
                else
                    return false;
            }
        );
        if (foundScene == theScenes.end() && foundIt == theFileNames.end())
        {
            theFileNames.emplace_back(toLoad);
        }

        // This should return something, I added 0 to get rid of build errors
        return 0;
    }

    // True = push       False = pop
    int SceneManager::FadeToScene(std::string_view currentscene,
        std::string_view toLoad, ConditionCheck pushOrPop)
    {
        // Set fading bool
        isFadingOut = true;
        push_or_pop = pushOrPop;
        // push scene states to stack
        if (pushOrPop == ConditionCheck::push)
        {
            PushSceneStates();
        }
        // pause all scenes
        PauseAllScenes();
        // Pause the current scene
        //GetScene(currentscene).setPaused(true);
        currentScene = currentscene;
        sceneToLoad = toLoad;

        // Ambience Change
        Ambience& ambience = *(GetSystemFromEngine<Ambience*>(my_engine, "Ambience"));
        ambience.AmbienceSetParameter(toLoad);

        BGMController& bgmcontroller = *(GetSystemFromEngine<BGMController*>(my_engine, "BGMController"));
        bgmcontroller.BGMSetParameter(toLoad);

        // Load Fade object from prefab with 0 transparency
        GameObjectManager& object_manager = *(GetSystemFromEngine<GameObjectManager*>(my_engine, "GameObjectManager"));
        fadeObjectID = object_manager.MakePrefabWithName("SceneFade");
        assert(fadeObjectID != -1);

        // This should return something, I added 0 to get rid of build errors
        return 0;
    }

    void SceneManager::UnloadScene(std::string_view toUnload)
    {
        int howMany = theScenes.size();
        for (int i = 0; i < howMany; ++i)
        {
            if (theScenes[i].GetName() == toUnload)
            {
                theFileToUnload.push_back(ID_Manager.IndexToId(i));
                return;
            }
        }
    }

    void SceneManager::UnloadScene(int toUnload)
    {
        theFileToUnload.push_back(toUnload);
        return;
    }

    Scene& SceneManager::GetScene(int theId)
    {
        int theIndex = ID_Manager[theId];
        return theScenes[theIndex];
    }

    Scene& SceneManager::GetScene(std::string_view theName)
    {
        for (int i = 0; i < theScenes.size(); ++i)
        {
            if (theName == theScenes[i].GetName())
            {
                return theScenes[i];
            }
        }
        //Couldnt find it
        assert(false, "Scene doesnt exist");
    }

    int SceneManager::GetSceneID(std::string_view theName)
    {
        for (int i = 0; i < theScenes.size(); ++i)
        {
            if (theName == theScenes[i].GetName())
            {
                return ID_Manager.IndexToId(i);
            }
        }
        //Couldnt find it
        return -1;
    }

    int SceneManager::GetNew(const std::string& name)
    {
        Scene boi(my_engine);
        int id = ID_Manager.Create(theScenes, std::move(boi));
        theScenes[ID_Manager[id]].SetName(name);
        on_change.SendMessage();
        return id;
    }

    void SceneManager::PauseAllScenes()
    {
        for (int i = 0; i < theScenes.size(); ++i)
        {
            theScenes[i].setPaused(true);
        }
    }

    void SceneManager::UnPauseAllScenes()
    {
        for (int i = 0; i < theScenes.size(); ++i)
        {
            theScenes[i].setPaused(false);
        }
    }

    void SceneManager::UnloadAllScenes()
    {
        int howMany = theScenes.size();
        for (int i = 0; i < howMany; ++i)
        {
            theFileToUnload.push_back(ID_Manager.IndexToId(i));
        }
        on_change.SendMessage();
    }

    void SceneManager::ForceLoadScene(std::string_view toLoad)
    {
        theFileNames.emplace_back(toLoad);

        int howMany = theFileNames.size();
        for (int i = 0; i < howMany; i++)
        {
            std::string name(theFileNames[i]);
            std::string theName = "Data/Scenes/" + name + ".json";
            Scene boi(my_engine);

            ID_Manager.Create(theScenes, std::move(boi));

            theScenes.back().Read(theName);
            theScenes.back().SetName(name);
        }
        on_change.SendMessage();
        //all names loaded so clear it
        theFileNames.clear();
    }

    void SceneManager::RegisterOnChange(std::function<void(void)> func)
    {
        on_change.RegisterMessage(func);
    }

    unsigned SceneManager::Size()
    {
        return theScenes.size();
    }

    Scene& SceneManager::GetSceneAtIndex(int index)
    {
        return theScenes[index];
    }

    int SceneManager::MakeNewScene()
    {
        Scene blank(my_engine);
        theScenes.push_back(blank);
        return theScenes.size() - 1;
    }

    int SceneManager::SceneInCharge(int gameObjectId)
    {
        int howBig = theScenes.size();
        for (int i = 0; i < howBig; i++)
        {
            //is object in scene
            if (theScenes[i].InChargeOfObject(gameObjectId))
            {
                return ID_Manager.IndexToId(i);
            }
        }

        return -1;
    }

    void SceneManager::setEnableSceneAll(bool condition)
    {
        for (int i = 0; i < theScenes.size(); ++i)
        {
            theScenes[i].setEnableScene(condition);
        }
    }

    void SceneManager::setEnableScene(std::string sceneToEnable, bool condition)
    {
        // Find a scene, and then enable/disable it
        for (int i = 0; i < theScenes.size(); ++i)
        {
            if (sceneToEnable == theScenes[i].GetName())
            {
                theScenes[i].setEnableScene(condition);
            }
        }
    }

    bool SceneManager::IsSceneLoaded(std::string name)
    {
        for (int i = 0; i < theScenes.size(); ++i)
        {
            if (name == theScenes[i].GetName())
            {
                return true;
            }
        }
        return false;
    }

    void SceneManager::PushSceneStates()
    {
        // Create a vector of all the current scene states, then add it to the sceneStates stack
        std::vector<sceneState> current_scene_states;
        for (int i = 0; i < theScenes.size(); ++i)
        {
            sceneState state(ID_Manager.IndexToId(i), theScenes[i].IsPaused(), theScenes[i].IsEnabled());
            current_scene_states.push_back(state);
        }
        sceneStates.push(current_scene_states);
    }

    void SceneManager::PopSceneStates()
    {
        // Apply the states from the top of the stack
        if (sceneStates.empty()) return;
        const std::vector<sceneState>& current_scene_states = sceneStates.top();
        for (int i = 0; i < current_scene_states.size(); ++i)
        {
            if (ID_Manager.Has(current_scene_states[i].sceneID))
            {
                int scene_index = ID_Manager[current_scene_states[i].sceneID];
                theScenes[scene_index].setPaused(current_scene_states[i].isPaused);
                theScenes[scene_index].setEnableScene(current_scene_states[i].isEnabled);
            }
        }

        sceneStates.pop();
    }

    void SceneManager::LoadSceneFromFilename(const std::string& toLoad)
    {

    }
} // End Of Dapper Namespace