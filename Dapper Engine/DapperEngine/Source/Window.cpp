//-----------------------------------------------------------------------------
//
// File Name:	Window.cpp
// Author(s):	Adam Tackett, (Code review by Isaiah Dickison)
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------

#include <iostream>
#include "Window.h"
#include "Input.h"
#include "Utility.h"

//Dapper Namespace!
namespace Dapper {

  //Default Constructor
  Window::Window(Engine& engine1) :engine(engine1)
  {
    width = 800;
    height = 600;

    mainWindow = NULL;
    bufferWidth = 0;
    bufferHeight = 0;
    shouldClose = false;
  }

  //Param Constructor
  Window::Window(int windowWidth, int windowHeight, Engine& engine1) :engine(engine1)
  {
    width = windowWidth;
    height = windowHeight;

    mainWindow = NULL;
    bufferWidth = 0;
    bufferHeight = 0;
    shouldClose = false;
  }

  //Deconstructor
  Window::~Window()
  {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
  }

  //Initalize the window
  void Window::Initialize()
  {
    //Initalise GLFW
    if (!glfwInit())
    {
      printf("Error Initializing GLFW");
      glfwTerminate();
      shouldClose = true;
      return;
    }

    

    //Setup GLFW Windows Properties
    //OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Core Profile = no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Allow forward compatiblity
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    //Create the window
    mainWindow = glfwCreateWindow(width, height, "Rushing Tide", NULL, NULL);
    glfwSetWindowUserPointer(mainWindow, this);

    if (!mainWindow)
    {
      printf("Error creating GLFW window!");
      //glfwTerminate();
      shouldClose = true;
      return;
    }

    monitor = glfwGetPrimaryMonitor();

    //allow corner X to close window
    glfwSetWindowCloseCallback(mainWindow, closeCallback);

    glfwSetWindowSizeCallback(mainWindow, OnWindowResize);



    //Get buffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    //Set the current context
    glfwMakeContextCurrent(mainWindow);
    //glfwSetWindowAspectRatio(mainWindow, 16, 9);
    //Handle Key + Mouse Input
    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //GLFW_CURSOR_NORMAL //Turns on cursor

    //Allow modern extension access
    glewExperimental = GL_TRUE;

    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
      std::cout << "Error:" << glewGetErrorString(error);
      //printf("Error: %s", glewGetErrorString(error));
      // might need line 111 if 110 doesnt work
      glfwDestroyWindow(mainWindow);
      glfwTerminate();
      shouldClose = true;
      return;
    }

    //glEnable(GL_DEPTH_TEST);

    //Create Viewport
    glViewport(0, 0, bufferWidth, bufferHeight);

    //glfwSetWindowUserPointer(mainWindow, this);

    shouldClose = false;

    SetFullScreen(true);

    #ifdef _DEBUG
        SetFullScreen(false);
    #endif

    return;
  }

  void Window::Update(float dt)
  {
    dt;

    glfwPollEvents();

    if (shouldClose || glfwWindowShouldClose(mainWindow))
    {
      engine.Stop();
      return;
    }

    


    glUseProgram(0);
  }

  void Window::Render()
  {
    //swapBuffers();
    //(glClearColor(0.0f, 0.0f, 1.0f, 1.0f));
    //(glClear(GL_COLOR_BUFFER_BIT));
  }

  //Close the window if conditions met
  bool Window::getShouldClose()
  {
    return glfwWindowShouldClose(mainWindow);
  }

  //Setup for key Input
  void Window::createCallbacks()
  {
    glfwSetKeyCallback(mainWindow, Input::handleKeys);
    glfwSetCursorPosCallback(mainWindow, Input::handleMouse);
    glfwSetMouseButtonCallback(mainWindow, Input::HandleMouseButtons);
    glfwSetScrollCallback(mainWindow, Input::handleMouseScroll);
    glfwSetWindowIconifyCallback(mainWindow, Input::handleIcon);
  }

  void Window::closeCallback(GLFWwindow* theWindow)
  {
    glfwSetWindowShouldClose(theWindow, true);
  }

  bool Window::UpdateWhilePaused()const
  {
    return true;
  }

  void Window::OnWindowResize(GLFWwindow* window, int width, int height)
  {
    window;
    glViewport(0, 0, width, height);
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    win->on_resize.SendMessage(width, height);
  }

  void Window::swapBuffers()
  {
    glfwSwapBuffers(mainWindow);
  }

  void Window::RegisterOnResize(std::function<void(int, int)> func)
  {
    on_resize.RegisterMessage(func);
  }

  glm::vec2 Window::GetWindowSize() const
  {
    int wid, heig;
    glfwGetWindowSize(mainWindow, &wid, &heig);
    return { wid,heig };
  }

  bool Window::IsFullScreen()
  {
    return fullscreen;
  }

  void Window::SetFullScreen(bool condition)
  {
    //if (IsFullScreen() == condition)
    //{
    //  return;
    //}

    if (condition)
    {
      // backup window position and window size
      glfwGetWindowPos(mainWindow, &held_pos.x, &held_pos.y);
      glfwGetWindowSize(mainWindow, &held_size.x, &held_size.y);

      // get resolution of monitor
      const GLFWvidmode* mode = glfwGetVideoMode(monitor);

      // switch to full screen
      glfwSetWindowMonitor(mainWindow, monitor, 0, 0, mode->width, mode->height, 0);
      fullscreen = true;
    }
    else
    {
      // restore last window size and position
      glfwSetWindowMonitor(mainWindow, nullptr, held_pos.x, held_pos.y,held_size.x, held_size.y, 0);
      fullscreen = false;
    }
  }





}//End of Dapper Namespace