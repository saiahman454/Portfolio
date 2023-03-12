//-----------------------------------------------------------------------------
//
// File Name:	Camera.h
// Author(s):	Adam Tackett
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------

#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GLFW\glfw3.h>

#include "ISystem.h"
#include "Engine.h"
#include "Input.h"
#include <numeric>
//Dapper Namespace!
namespace Dapper {

	class Camera:public ISystem
	{
	public:
		Camera(Engine& engine1);
		Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, 
			   GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed, 
			   Engine& engine1);
		~Camera();

		const std::string& Name()const override;
		void Initialize()override;
		void Update(float dt)override;
		void Render()override;

		inline std::string GetName() { return name; }

		void keyControl(const std::vector<keyboard_key>& keys, GLfloat deltaTime1);
		void mouseControl(GLfloat xChange, GLfloat yChange);
		glm::mat4 calculateViewMatrix();
		inline glm::vec3 getCameraPosition() { return position; }
		inline glm::vec3 getCameraMovement() { return position - lastPosition; }
		inline glm::vec3 getCameraDirection() { return glm::normalize(front); }
		glm::mat4 calculateDeviceMatrix();
		void setCameraPosition(glm::vec3 input);
		inline void setScale(float scale_) { scale = scale_; }

		inline const glm::vec2& GetCameraSize()const { return cameraSize * scale; }
		void CalcBounds();
	private:
		const std::string name = "Camera";

		glm::vec3 position;
		glm::vec3 lastPosition;
		glm::vec3 front;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 worldUp;
		GLfloat yaw;	//left and right x axis
		GLfloat pitch;	//up and down y axis
		GLfloat moveSpeed;
		GLfloat turnSpeed;
		glm::vec2 cameraSize;
		float scale;
		
		Engine& engine;

		glm::vec2 X_range;
		glm::vec2 Y_range;


		void update();

		void CheckBounds();
		// Inherited via ISystem
		virtual bool UpdateWhilePaused() const override;
	};

}//End of Dapper Namespace
