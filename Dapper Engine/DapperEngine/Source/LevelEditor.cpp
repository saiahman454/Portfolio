//------------------------------------------------------------------------------
//
// File Name:	LevelEditor.cpp
// Author(s):	Isaiah Dickison
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "LevelEditor.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Window.h"
#include "Time.h"
#include "Utility.h"
#include "Camera.h"
#include "GameObjectListBox.h"
#include "ComponentWindow.h"
#include "SaveMenu.h"
#include "FpsWindow.h"
#include "PrefabsWindow.h"
#include "MassChangeWindow.h"
// Dapper Namespace!
namespace Dapper
{
  /*--------------------------------------
  Description: Constuctor
  Params:
  engine - the parent engine
  ---------------------------------------*/
  LevelEditor::LevelEditor(Engine& engine) : my_engine(engine)
  {
    gui_mouse.resize(GLFW_MOUSE_BUTTON_LAST + 1);
  }
  /*--------------------------------------
  Description: Destuctor
  ---------------------------------------*/
  LevelEditor::~LevelEditor()
  {
    for (ImGuiWindow* current : windows)
    {
      delete current;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  /*--------------------------------------
  Description: initializing the level editor 
  and seting up imgui
  ---------------------------------------*/
  void LevelEditor::Initialize()
  {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    Window& my_window = *dynamic_cast<Dapper::Window*>(my_engine.Get("Window"));
    ImGui_ImplGlfw_InitForOpenGL(my_window.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    //add all windows
    AddWindow(new GameObjectListBox(*this));
    AddWindow(new FpsWindow(*this));
    AddWindow(new ComponentWindow(*this));
    AddWindow(new SaveMenu(*this));
    AddWindow(new PrefabsWindow(*this));
    AddWindow(new MassChangeWindow(*this));

    for (ImGuiWindow* current : windows)
    {
      current->Init();
    }

    Window* win = GetSystemFromEngine<Window*>(my_engine, "Window");
    std::function<void(int, int)> var = std::bind(&LevelEditor::OnResizeCalcScale, this, std::placeholders::_1, std::placeholders::_2);
    win->RegisterOnResize(var);
    Input* input = GetSystemFromEngine<Input*>(my_engine, "Input");
    std::function<void(int)> func_trig = std::bind(&LevelEditor::OnTrigger, this, std::placeholders::_1);
    input->RegisterMouseTrigger(func_trig);
    std::function<void(int)> func_rel = std::bind(&LevelEditor::OnRelease, this, std::placeholders::_1);
    input->RegisterMouseRelease(func_rel);
  }
  /*--------------------------------------
  Description: Update function
  Params:
  dt - Delta time
  ---------------------------------------*/
  void LevelEditor::Update(float dt)
  {
    dt;
  }

  /*--------------------------------------
  Description: Render function that displays everything
  ---------------------------------------*/
  void LevelEditor::Render()
  {
    float scale = GetScaleFactor();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    Camera* camera_system = GetSystemFromEngine<Camera*>(my_engine, "Camera");

    if (my_engine.IsGamePaused())
    {
      for (ImGuiWindow* current : windows)
      {
        current->Render();
      }

      // Scroll to change the scale of the camera
      Input* input_system = GetSystemFromEngine<Input*>(my_engine, "Input");
      const std::vector<keyboard_key>& keys = input_system->GetAllKeys();

      if (keys[GLFW_KEY_LEFT_CONTROL] == keyboard_key::KEY_DOWN)
      {
          glm::vec2 scroll_change = input_system->getScrollChange();
          scroll_zoom -= scroll_change;
          camera_system->setScale((float)pow(2.0, scroll_zoom.y / 10));
          input_system->SetScollChange({ 0,0 });

      }
    }
    else
    {
        camera_system->setScale(1.0f);
    }
    ImVec2 size(115, 45);

     for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST + 1; ++i)
     {
        if (gui_mouse[i] == mouse_button::MOUSE_TRIGGERED)
        {
          gui_mouse[i] = mouse_button::MOUSE_DOWN;
        }
        else if (gui_mouse[i] == mouse_button::MOUSE_RELEASE)
        {
          gui_mouse[i] = mouse_button::MOUSE_UP;
        }
     }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    Window* window = GetSystemFromEngine<Window*>(my_engine, "Window");
    window->swapBuffers();
    (glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    (glClear(GL_COLOR_BUFFER_BIT));

  }

  /*--------------------------------------
  Description: Adds a window to the level editor
  Params:
  new_window - the new imgui window
  ---------------------------------------*/
  void LevelEditor::AddWindow(ImGuiWindow* new_window)
  {
    windows.push_back(new_window);
  }

  /*--------------------------------------
  Description: Gets the level editor window by name
  Params:
  window_name - the name of the window
  ---------------------------------------*/
  ImGuiWindow* LevelEditor::GetWindow(std::string window_name)
  {
    for (int i = 0; i < (int)windows.size(); i++)
    {
      if (windows[i]->GetWindowName() == window_name)
      {
        return windows[i];
      }
    }
    return nullptr;
  }

  /*--------------------------------------
  Description: calculates the window scale
  Params:
  width - width
  height - height
  ---------------------------------------*/
  void LevelEditor::OnResizeCalcScale(int width, int height)
  {
    height;
    Camera& camera = *GetSystemFromEngine<Camera*>(my_engine, "Camera");
    window_scale = width / camera.GetCameraSize().x;
  }

  /*--------------------------------------
  Description: mouse button release for level editor 
  Params: button - the glfw button
  ---------------------------------------*/
  void LevelEditor::OnRelease(int button)
  {
    gui_mouse[button] = mouse_button::MOUSE_RELEASE;
  }
  /*--------------------------------------
  Description: mouse button trigger for level editor
  Params: button - the glfw button
  ---------------------------------------*/
  void LevelEditor::OnTrigger(int button)
  {
    gui_mouse[button] = mouse_button::MOUSE_TRIGGERED;
  }

} // End Of Dapper Namespace