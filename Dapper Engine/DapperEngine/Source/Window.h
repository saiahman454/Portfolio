//-----------------------------------------------------------------------------
//
// File Name:	Window.h
// Author(s):	Adam Tackett, (Code review by Isaiah Dickison)
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------

#pragma once

#include "stdio.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ISystem.h"
#include "Engine.h"
#include "Messenger.h"
#include "glm/vec2.hpp"

//Dapper Namespace!
namespace Dapper {

	class Window:public ISystem
	{
	public:
		Window(Engine& engine1);
		Window(int windowWidth, int windowHeight, Engine& engine1);
		~Window()override;

		inline const std::string& Name()const override { return name; }

		void Initialize()override;
		void Update(float dt)override;
		void Render()override;

		inline int getBufferWidth() { return bufferWidth; }
		inline int getBufferHeight() { return bufferHeight; }
		bool getShouldClose();
		void swapBuffers();
		inline GLFWwindow* getWindow() { return mainWindow; };
		void RegisterOnResize(std::function<void(int, int)> func);

		inline Engine& GetEngine() { return engine; }

		glm::vec2 GetWindowSize()const;

		bool IsFullScreen();
		void SetFullScreen(bool condition);
	private:
		const std::string name = "Window";

		GLFWwindow* mainWindow;
		int width, height;
		int bufferWidth, bufferHeight;
		bool shouldClose;
		Engine& engine;
		Messenger<int, int> on_resize;

		GLFWmonitor* monitor;
		
		glm::vec<2,int> held_pos = { 0,0 };
		glm::vec<2, int> held_size = { 0,0 };
		bool fullscreen = false;

		void createCallbacks();
		static void closeCallback(GLFWwindow* theWindow);

		// Inherited via ISystem
		virtual bool UpdateWhilePaused()const override;

		static void OnWindowResize(GLFWwindow* window, int width, int height);
	};

}//End of Dapper Namespace