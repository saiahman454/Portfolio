//-----------------------------------------------------------------------------
//
// File Name:	Camera.cpp
// Author(s):	Adam Tackett
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------

#include "Camera.h"
#include "Utility.h"
#include "Input.h"
#include "SceneManager.h"
#include "Transform.h"

//Dapper Namespace!
namespace Dapper {

  //Default Constructor
  Camera::Camera(Engine& engine1) :engine(engine1)
  {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);;
    right = glm::normalize(glm::cross(front, worldUp));
    yaw = -90.0f;	//left and right x axis
    pitch = 0.0f;	//up and down y axis
    moveSpeed = 150.0f;
    turnSpeed = 0.5f;
    cameraSize.x = 1600;
    cameraSize.y = 900;
    scale = 1.0f;
  }

  //Param Constructor
  Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp,
    GLfloat startYaw, GLfloat startPitch,
    GLfloat startMoveSpeed, GLfloat startTurnSpeed,
    Engine& engine1) :engine(engine1)
  {
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;
    cameraSize.x = 1600;
    cameraSize.y = 900;
    scale = 1.0f;

    update();
  }

  //Deconstructor
  Camera::~Camera()
  {
  }

  const std::string& Camera::Name() const
  {
    return name;
  }

  void Camera::Initialize()
  {
    position = glm::vec3(0.0f, 0.0f, -100.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    worldUp = glm::vec3(0.0f, 1.0f, 0.0f);;
    right = glm::normalize(glm::cross(front, worldUp));
    yaw = -90.0f;	//left and right x axis
    pitch = 0.0f;	//up and down y axis
    moveSpeed = 1000.0f;
    turnSpeed = 0.5f;

    cameraSize.x = 1600;
    cameraSize.y = 900;
    SceneManager& scene_man = *GetSystemFromEngine<SceneManager*>(engine, "SceneManager");
    std::function<void()> func = std::bind(&Camera::CalcBounds, this);
    scene_man.RegisterOnChange(func);

  }

  void Camera::Update(float dt)
  {
    dt;
    Input& input = *GetSystemFromEngine<Input*>(engine, "Input");
    keyControl(input.GetAllKeys(), dt);
    lastPosition = position;
    if (!engine.IsGamePaused())
    {
      CheckBounds();
    }

    //&&&Adam Enable for 3D
    //mouseControl(input->getXChange(), input->getYChange());
  }

  void Camera::Render()
  {
  }

  void Camera::keyControl(const std::vector<keyboard_key>& keys, GLfloat deltaTime1)
  {
    GLfloat velocity = moveSpeed * deltaTime1;
    if (engine.IsGamePaused())
    {
      if (keys[GLFW_KEY_SPACE] == keyboard_key::KEY_DOWN)
      {
        moveSpeed = 3000.0f;
      }
      else
      {
        moveSpeed = 1000.0f;
      }

      if (keys[GLFW_KEY_UP] == keyboard_key::KEY_DOWN || keys[GLFW_KEY_W] == keyboard_key::KEY_DOWN)
      {
        position += up * velocity; //front or up
      }

      if (keys[GLFW_KEY_DOWN] == keyboard_key::KEY_DOWN || keys[GLFW_KEY_S] == keyboard_key::KEY_DOWN)
      {
        position -= up * velocity; //front .. up
      }

      if (keys[GLFW_KEY_LEFT] == keyboard_key::KEY_DOWN || keys[GLFW_KEY_A] == keyboard_key::KEY_DOWN)
      {
        position -= right * velocity;
      }

      if (keys[GLFW_KEY_RIGHT] == keyboard_key::KEY_DOWN || keys[GLFW_KEY_D] == keyboard_key::KEY_DOWN)
      {
        position += right * velocity;
      }
    }
  }

  //Mouse Control for looking around 3D
  void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
  {
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    if (pitch > 89.0f)
    {
      pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
      pitch = -89.0f;
    }

    update();
  }

  //Used for Rendering function to get camera perspective
  glm::mat4 Camera::calculateViewMatrix()
  {
    //lookAT not working
    //return glm::lookAt(position, position + front, up);
    return glm::translate(glm::mat4(1), -position);
  }

  glm::mat4 Camera::calculateDeviceMatrix()
  {
    return glm::scale(glm::mat4(1), glm::vec3(2 / (scale * cameraSize.x), 2 / (scale * cameraSize.y), 1));
  }

  void Camera::setCameraPosition(glm::vec3 input)
  {
    position = input;
  }

  //Update Camera for looking around in 3D
  void Camera::update()
  {
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
  }

  void Camera::CalcBounds()
  {
    position.x = 0;
    position.y = 0;
    X_range = { std::numeric_limits<float>::max(), std::numeric_limits<float>::min() };
    Y_range = { std::numeric_limits<float>::max(), std::numeric_limits<float>::min() };
    GameObjectManager& obj_man = *GetSystemFromEngine<GameObjectManager*>(engine, "GameObjectManager");
    const std::vector<GameObject>& all_obj = obj_man.GetAllGameObjects();
    for (GameObject current : all_obj)
    {
      if (current.GetFlag() == "Tile")
      {

        Transform& trans = *GetComponentFromParent<Transform>(engine, current.GetID(), "Transform");
        glm::vec3 pos = trans.GetTranslation();
        glm::vec2 size = trans.GetScale();
        if (pos.x > X_range.y)
        {
          X_range.y = pos.x + (size.x / 2);
        }
        if (pos.x < X_range.x)
        {
          X_range.x = pos.x - (size.x / 2);
        }
        if (pos.y > Y_range.y)
        {
          Y_range.y = pos.y + (size.y / 2);
        }
        if (pos.y < Y_range.x)
        {
          Y_range.x = pos.y - (size.y / 2);
        }
      }
    }

  }

  void Camera::CheckBounds()
  {
    if (position.x - (cameraSize.x / 2) < X_range.x)
    {
      position.x = X_range.x + (cameraSize.x / 2);
    }
    if (position.x + (cameraSize.x / 2) > X_range.y)
    {
      position.x = X_range.y - (cameraSize.x / 2);
    }
    if (position.y - (cameraSize.y / 2) < Y_range.x)
    {
      position.y = Y_range.x + (cameraSize.y / 2);
    }
    if (position.y + (cameraSize.y / 2) > Y_range.y)
    {
      position.y = Y_range.y - (cameraSize.y / 2);
    }

  }

  bool Camera::UpdateWhilePaused() const
  {
    return true;
  }

}//End of Dapper Namespace