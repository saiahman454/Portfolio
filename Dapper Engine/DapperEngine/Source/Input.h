//-----------------------------------------------------------------------------
//
// File Name:	Input.h
// Author(s):	Adam Tackett Isaiah Dickison
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------

#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include "ISystem.h"
#include "Engine.h"
#include "Messenger.h"
#include "glm/vec2.hpp"

//Dapper Namespace!
namespace Dapper 
{
	//states of keys
	enum class keyboard_key 
	{
		KEY_UP = 0,
		KEY_DOWN,
		KEY_TRIGGERED,
		KEY_RELEASE
	};
	//states of mouse buttons
	enum class mouse_button
	{
		MOUSE_UP = 0,
		MOUSE_DOWN,
		MOUSE_TRIGGERED,
		MOUSE_RELEASE

	};
	class Input : public ISystem
	{
	public:
		Input(Engine& engine1);
		~Input()override;

		inline const std::string& Name()const override { return name; }

		void Initialize()override;

		void Update(float dt) override;

		void Render() override;
		inline const keyboard_key& getKey(int glfw_key)const { return all_keys[glfw_key]; }
		inline const mouse_button& getMouse(int glfw_key)const { return all_mouse[glfw_key]; }
		float getXChange();
		float getYChange();

		inline float getXPos() { return xPos; }
		inline float getYPos() { return yPos; }

		inline glm::vec2 getScrollOffset() const { return scroll_offset; }
		inline glm::vec2 getScrollChange() const { return scroll_change; }
		inline void SetScollChange(glm::vec2 condition) { scroll_change = condition; };
		
		void getMovement(bool* keys, GLfloat deltaTime);

		static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
		static void HandleMouseButtons(GLFWwindow* window, int button, int action, int mode);
		static void handleMouse(GLFWwindow* window, double xPos, double yPos);
		static void handleMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
		static void handleIcon(GLFWwindow* window, int iconified);
		//static void 

		inline Engine& GetEngine() { return engine; }
		inline const glm::vec2& GetMousePos()const{ return mousepos; }

		glm::vec2 GetMousePosinWorldCoords();

		std::vector<keyboard_key>& GetAllKeys();
		std::vector<mouse_button>& GetAllMouseButtons();

		void RegisterMouseTrigger(std::function<void(int)> func);
		void RegisterMouseRelease(std::function<void(int)> func);

		inline const Messenger<int>& GetMouseTriggerMessage()const { return onMouseTrigger; }
		inline const Messenger<int>& GetMouseReleaseMessage()const { return onMouseRelease; }

		void setPause(bool paused);

	private:
		const std::string name = "Input";
		Engine& engine;

		float lastX;
		float lastY;
		float xChange;
		float yChange;
		bool mouseFirstMoved;

		glm::vec2 scroll_offset;
		glm::vec2 scroll_change;

		GLfloat moveSpeed;
		float xPos;
		float yPos;
		glm::vec2 mousepos;
		std::vector<keyboard_key> all_keys;
		std::vector<mouse_button> all_mouse;


		Messenger<int> onMouseTrigger;
		Messenger<int> onMouseRelease;

		bool pause_toggle = true;
		// Inherited via ISystem
		inline virtual bool UpdateWhilePaused()const override { return true; };
	};

}//End of Dapper Namespace
