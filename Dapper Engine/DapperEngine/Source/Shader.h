//-----------------------------------------------------------------------------
//
// File Name:	Shader.h
// Author(s):	Adam Tackett
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------

#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include "Deserializer.h"

//Dapper Namespace!
namespace Dapper {

	class Shader
	{
	public:
		Shader(int the_ID);
		~Shader();
		Shader(Shader&& shader) noexcept;
		Shader(const Shader& shader) = default;

		void CreateFromString(const char* vertexCode, const char* fragmentCode);
		void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
		std::string ReadFile(const char* fileLocation);
		inline GLuint GetProjectionLocation() { return uniformProjection; }
		inline GLuint GetModelLocation() { return uniformModel; }
		inline GLuint GetViewLocation() { return uniformView; }
		inline void UseShader() { glUseProgram(shaderID); }
		void ClearShader();
		inline std::string GetFileName() { return file_location; };

		inline std::string_view GetName() { return the_name; }

		void Read(Deserializer& deserial, const rapidjson::Value& jsonVal);
		void SetVector2f(std::string variableName, glm::vec2 input);
		void SetVector3f(std::string variableName, glm::vec3 input);
		void SetVector4f(std::string variableName, glm::vec4 input);

		void SetMatrix4f(std::string variableName, glm::mat4 input);

	private:
		const inline static std::string the_name = "Shader";

		GLuint shaderID, uniformProjection, uniformModel, uniformView;
		void CompileShader(const char* vertexCode, const char* fragmentCode);
		void AddShader(GLuint glProgram, const char* shaderCode, GLenum shaderType);

		// The name of the Shader
		std::string file_location;

		int ID;
	};

}//End of Dapper Namespace
