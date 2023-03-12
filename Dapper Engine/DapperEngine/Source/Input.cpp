//-----------------------------------------------------------------------------
//
// File Name:	Input.cpp
// Author(s):	Adam Tackett
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------

#include "Input.h"
#include "Window.h"
#include "Utility.h"
#include "Time.h"
#include <iostream>
#include "Audio.h"
#include <assert.h>
#include "Camera.h"
#include "SceneManager.h"
#include "BGMController.h"
#include "Transform.h"
#include "CameraAnchorManager.h"

//Dapper Namespace!
namespace Dapper
{
    /*--------------------------------------
    Description: writes object to a file
    Params:
    engine - parent engine
    ---------------------------------------*/
    Input::Input(Engine& engine1) :engine(engine1)
    {
        moveSpeed = 5.0f;
        xPos = 0.0f;
        yPos = 0.0f;

        xChange = 0.0f;
        yChange = 0.0f;

        lastX = 0.0f;
        lastY = 0.0f;
        mouseFirstMoved = true;


        all_keys.resize(GLFW_KEY_LAST + 1);
        all_mouse.resize(GLFW_MOUSE_BUTTON_LAST + 1);
    }
    /*--------------------------------------
    Description: basic destructor
    ---------------------------------------*/
    Input::~Input()
    {
    }

    /*--------------------------------------
    Description: Initialize function
    ---------------------------------------*/
    void Input::Initialize()
    {
    }

    /*--------------------------------------
    Description: checks for key and mouse presses
    while updating the state machine.
    dt - delta time
    ---------------------------------------*/
    void Input::Update(float dt)
    {
        dt;
        for (int i = 0; i < GLFW_KEY_LAST + 1; ++i)
        {
            if (all_keys[i] == keyboard_key::KEY_TRIGGERED)
            {
                all_keys[i] = keyboard_key::KEY_DOWN;
            }
            else if (all_keys[i] == keyboard_key::KEY_RELEASE)
            {
                all_keys[i] = keyboard_key::KEY_UP;
            }
        }
        for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST + 1; ++i)
        {
            if (all_mouse[i] == mouse_button::MOUSE_TRIGGERED)
            {
                all_mouse[i] = mouse_button::MOUSE_DOWN;
            }
            else if (all_mouse[i] == mouse_button::MOUSE_RELEASE)
            {
                all_mouse[i] = mouse_button::MOUSE_UP;
            }
        }

        //scroll_change = { 0, 0 };
    }

    /*--------------------------------------
    Description: Render function
    ---------------------------------------*/
    void Input::Render()
    {
    }

    /*--------------------------------------
    Description: get function for x
    Return - the change in x
    ---------------------------------------*/
    float Input::getXChange()
    {
        GLfloat theChange = xChange;
        xChange = 0.0f;
        return theChange;
    }

    /*--------------------------------------
    Description: get function for y
    Return - the change in Y
    ---------------------------------------*/
    float Input::getYChange()
    {
        GLfloat theChange = yChange;
        yChange = 0.0f;
        return theChange;
    }



    /*--------------------------------------
    Description: checks for key presses
    ---------------------------------------*/
    //Input from keys to move player
    void Input::getMovement(bool* keys1, GLfloat deltaTime)
    {
        GLfloat velocity = moveSpeed * deltaTime;

        if (keys1[GLFW_KEY_W])
        {
            yPos += velocity; //up
        }

        if (keys1[GLFW_KEY_S])
        {
            yPos -= velocity; //down
        }

        if (keys1[GLFW_KEY_A])
        {
            xPos -= velocity; //left
        }

        if (keys1[GLFW_KEY_D])
        {
            xPos += velocity; //right
        }
    }
    /*--------------------------------------
    Description: the glfw call back for key presses
    ---------------------------------------*/
    //Handles all key inputs
    void Input::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
    {
        code;
        mode;
        Window* winder = static_cast<Window*>(glfwGetWindowUserPointer(window));
        Input* theInput = GetSystemFromEngine<Input*>(winder->GetEngine(), "Input");

        std::vector<keyboard_key>& everykey = theInput->all_keys;

        if (action == GLFW_PRESS)
        {
            assert(everykey[key] != keyboard_key::KEY_DOWN);
            assert(everykey[key] != keyboard_key::KEY_TRIGGERED);
            everykey[key] = keyboard_key::KEY_TRIGGERED;

            if (key == GLFW_KEY_ESCAPE)
            {
                //glfwSetWindowShouldClose(window, GL_TRUE);
                SceneManager* theMan = GetSystemFromEngine<SceneManager*>
                    (winder->GetEngine(), "SceneManager");

                BGMController& bgm = *GetSystemFromEngine<BGMController*>(winder->GetEngine(), "BGMController");

                if (theMan->IsSceneLoaded("Level1") ||
                    theMan->IsSceneLoaded("Level2") ||
                    theMan->IsSceneLoaded("Level3"))
                {
                    //Prevent escape spam causing crash
                    if (theMan->IsSceneLoaded("CreditsPause") ||
                        theMan->IsSceneLoaded("ControlsPause") ||
                        theMan->IsSceneLoaded("PauseMenuConfirmQuit") ||
                        theMan->IsSceneLoaded("OptionsPause")
                        )
                    {
                        return;
                    }

                    // Loading the pause screen
                    if (theInput->pause_toggle)
                    {
                        theMan->PushSceneStates();
                        theMan->PauseAllScenes();
                        //theMan->setEnableSceneAll(false);
                        theMan->LoadScene("Pause");

                        theInput->pause_toggle = false;
                        bgm.SetPaused(1);

                    }
                    else
                    {
                        theMan->PopSceneStates();
                        //theMan->UnPauseAllScenes();
                        theMan->UnloadScene("Pause");
                        theInput->pause_toggle = true;
                        bgm.SetPaused(0);

                    }

                }
            }
            if (key == GLFW_KEY_F11)
            {
                if (winder->IsFullScreen())
                {
                    winder->SetFullScreen(false);
                }
                else
                {
                    winder->SetFullScreen(true);
                }

            }
            if (key == GLFW_KEY_SEMICOLON)
            {
                winder->GetEngine().ToggleGamePaused();
            }
        }
        else if (action == GLFW_RELEASE)
        {
            assert(everykey[key] != keyboard_key::KEY_UP);
            assert(everykey[key] != keyboard_key::KEY_RELEASE);

            everykey[key] = keyboard_key::KEY_RELEASE;
        }


    }

    /*--------------------------------------
    Description: glfw call backs for mouse buttons
    ---------------------------------------*/
    void Input::HandleMouseButtons(GLFWwindow* window, int button, int action, int mode)
    {
        mode;
        Window* winder = static_cast<Window*>(glfwGetWindowUserPointer(window));
        Input* theInput = GetSystemFromEngine<Input*>(winder->GetEngine(), "Input");
        std::vector<mouse_button>& everybutton = theInput->all_mouse;
        if (action == GLFW_PRESS)
        {
            assert(everybutton[button] != mouse_button::MOUSE_DOWN);
            assert(everybutton[button] != mouse_button::MOUSE_TRIGGERED);
            everybutton[button] = mouse_button::MOUSE_TRIGGERED;
            auto messager = theInput->GetMouseTriggerMessage();
            messager.SendMessage(button);
        }
        if (action == GLFW_RELEASE)
        {
            assert(everybutton[button] != mouse_button::MOUSE_UP);
            assert(everybutton[button] != mouse_button::MOUSE_RELEASE);
            everybutton[button] = mouse_button::MOUSE_RELEASE;
            auto messager = theInput->GetMouseReleaseMessage();
            messager.SendMessage(button);
        }

    }

    /*--------------------------------------
    Description: glfw call caks for mouse movement
    ---------------------------------------*/
    //Handles all mouse inputs
    void Input::handleMouse(GLFWwindow* window, double xPos, double yPos)
    {
        Window* winder = static_cast<Window*>(glfwGetWindowUserPointer(window));
        Input* theInput = GetSystemFromEngine<Input*>(winder->GetEngine(), "Input");

        if (theInput->mouseFirstMoved)
        {
            theInput->lastX = (float)xPos;
            theInput->lastY = (float)yPos;
            theInput->mouseFirstMoved = false;
        }

        theInput->xChange = (float)xPos - theInput->lastX;
        theInput->yChange = theInput->lastY - (float)yPos;

        theInput->mousepos.x = (float)xPos;
        theInput->mousepos.y = (float)yPos;
        //Debug check mouse movement
        //printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange); 
        //std::cout << "X:" << theWindow->lastX << " Y:" << theWindow->lastY << "\n";
    }

    // Handles when mouse is scrolled
    void Input::handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
    {
        Window* winder = static_cast<Window*>(glfwGetWindowUserPointer(window));
        Input* theInput = GetSystemFromEngine<Input*>(winder->GetEngine(), "Input");

        theInput->scroll_change = { xoffset, yoffset };

        theInput->scroll_offset.x -= xoffset;
        theInput->scroll_offset.y -= yoffset;
    }

    void Input::handleIcon(GLFWwindow* window, int iconified)
    {
        Window* winder = static_cast<Window*>(glfwGetWindowUserPointer(window));
        Input* theInput = GetSystemFromEngine<Input*>(winder->GetEngine(), "Input");
        SceneManager* theMan = GetSystemFromEngine<SceneManager*>(winder->GetEngine(), "SceneManager");
        BGMController& bgm = *GetSystemFromEngine<BGMController*>(winder->GetEngine(), "BGMController");
        Audio& audio = *GetSystemFromEngine<Audio*>(winder->GetEngine(), "Audio");
        Dapper::Bus* bus = audio.GetBus("/");
        if (iconified)
        {
            theMan->PushSceneStates();
            theMan->PauseAllScenes();
            //theMan->setEnableSceneAll(false);
            theMan->LoadScene("Pause");
            theInput->pause_toggle = false;
            bgm.SetPaused(1);
            bus->SetVolume(0);
        }
        else if (!iconified)
        {

            bus->SetVolume(1);

        }
    }

    /*--------------------------------------
    Description: calculates mouse click in world coords
    Return - the corrdinates
    ---------------------------------------*/
    glm::vec2 Input::GetMousePosinWorldCoords()
    {
        Window& winder = *GetSystemFromEngine<Window*>(engine, "Window");
        Camera& camera = *GetSystemFromEngine<Camera*>(engine, "Camera");
        glm::vec2 window_size = winder.GetWindowSize();

        glm::vec2 camera_size = camera.GetCameraSize();
        glm::vec3 camerapos = camera.getCameraPosition();
        glm::vec2 two_D_campos(camerapos.x, camerapos.y);
        glm::vec2 half(.5, -.5);

        double testx, testy;
        glfwGetCursorPos(winder.getWindow(), &testx, &testy);

        glm::vec2 percent{ ((float)testx / window_size.x,(float)testy / window_size.y) };
        float perX = (float)testx / window_size.x;
        float perY = (float)testy / window_size.y;

        float worldX = camera_size.x * (perX - 0.5) + camerapos.x;
        float worldY = -1 * (camera_size.y * (perY - 0.5) - camerapos.y);



        return { worldX, worldY };

    }

    /*--------------------------------------
    Description: returns the vector with all the keys
    ---------------------------------------*/
    std::vector<keyboard_key>& Input::GetAllKeys()
    {
        return all_keys;
    }
    /*--------------------------------------
    Description: returns the vector with all the mouse buttons
    ---------------------------------------*/
    std::vector<mouse_button>& Input::GetAllMouseButtons()
    {
        return all_mouse;
    }
    /*--------------------------------------
    Description: register function for mouse trigger
    ---------------------------------------*/
    void Input::RegisterMouseTrigger(std::function<void(int)> func)
    {
        onMouseTrigger.RegisterMessage(func);
    }
    /*--------------------------------------
    Description: register function for mouse relese
    ---------------------------------------*/
    void Input::RegisterMouseRelease(std::function<void(int)> func)
    {
        onMouseRelease.RegisterMessage(func);
    }
    void Input::setPause(bool paused)
    {

        pause_toggle = paused;

    }
}