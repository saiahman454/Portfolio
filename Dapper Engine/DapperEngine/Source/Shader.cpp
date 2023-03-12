//-----------------------------------------------------------------------------
//
// File Name:	Shader.cpp
// Author(s):	Adam Tackett
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//-----------------------------------------------------------------------------

#include "Shader.h"

//Dapper Namespace!
namespace Dapper 
{
	//Default Constructor
	Shader::Shader(int the_ID) : ID(the_ID)
	{
		shaderID = 0;
		uniformModel = 0;
		uniformProjection = 0;
		uniformView = 0;
	}

	//Deconstructor
	Shader::~Shader()
	{
		ClearShader();
	}

	Shader::Shader(Shader&& shader) noexcept :
		shaderID(shader.shaderID),
		ID(shader.ID),
		file_location(std::move(shader.file_location))
	{
		shader.shaderID = 0;
		shader.ID = 0;
	}

	//Create shader from string
	void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
	{
		CompileShader(vertexCode, fragmentCode);
	}

	//Create shader from file
	void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
	{
		std::string vertexString = ReadFile(vertexLocation);
		std::string fragmentString = ReadFile(fragmentLocation);
		const char* vertexCode = vertexString.c_str();
		const char* fragmentCode = fragmentString.c_str();

		CompileShader(vertexCode, fragmentCode);
	}

	//Read the file and return string
	std::string Shader::ReadFile(const char* fileLocation)
	{
		std::string content;
		std::ifstream fileStream(fileLocation, std::ios::in);

		if (!fileStream.is_open()) 
		{
			std::cout << "Failed to read " << fileLocation << "!File doesn't exist.\n";
			return "";
		}

		std::string line = "";
		while (!fileStream.eof())
		{
			std::getline(fileStream, line);
			content.append(line + "\n");
		}

		fileStream.close();
		return content;
	}

	//Compile the shader
	void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
	{
		shaderID = glCreateProgram();

		if (!shaderID)
		{
			std::cout << "Error creating shader program!\n";
			return;
		}

		AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
		AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

		GLint result = 0;
		GLchar eLog[1024] = { 0 };

		glLinkProgram(shaderID);
		glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
			std::cout << "Error linking program:" << eLog << "\n";
			return;
		}

		glValidateProgram(shaderID);
		glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
			std::cout << "Error linking program:" << eLog << "\n";
			return;
		}

		uniformProjection = glGetUniformLocation(shaderID, "projection");
		uniformModel = glGetUniformLocation(shaderID, "model");
		uniformView = glGetUniformLocation(shaderID, "view");
	}

	void Shader::ClearShader()
	{
		if (shaderID != 0)
		{
			glDeleteProgram(shaderID);
			shaderID = 0;
		}

		uniformModel = 0;
		uniformProjection = 0;
	}

	void Shader::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
		//auto object = jsonVal.GetObject();
		//use jsonVal instead of object or it searches for "Shader" and crashes
		deserial;
		file_location = jsonVal.GetString();
	}

	void Shader::SetVector2f(std::string variableName, glm::vec2 input)
	{
		glUniform2f(glGetUniformLocation(shaderID, variableName.c_str()), 
										 input.x, input.y);
	}

	void Shader::SetVector3f(std::string variableName, glm::vec3 input)
	{
		glUniform3f(glGetUniformLocation(shaderID, variableName.c_str()),
										 input.x, input.y, input.z);
	}

	void Shader::SetVector4f(std::string variableName, glm::vec4 input)
	{
		//v allows passing in array's
		glUniform4fv(glGetUniformLocation(shaderID, variableName.c_str()), 
										  1, &input.x);
	}

	void Shader::SetMatrix4f(std::string variableName, glm::mat4 input)
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderID, variableName.c_str()),
												1, GL_FALSE, &input[0][0]);
	}

	//Shader onto graphics card
	void Shader::AddShader(GLuint glProgram, const char* shaderCode,
						   GLenum shaderType)
	{
		GLuint theShader = glCreateShader(shaderType);

		const GLchar* theCode[1];
		theCode[0] = shaderCode;

		GLint codeLength[1];
		codeLength[0] = (GLint)strlen(shaderCode);

		glShaderSource(theShader, 1, theCode, codeLength);
		glCompileShader(theShader);

		GLint result = 0;
		GLchar eLog[1024] = { 0 };

		glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
			//printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
			std::cout << "Error compiling the" << shaderType << "shader:" << eLog << std::endl;
			return;
		}

		glAttachShader(glProgram, theShader);
	}

}//End of Dapper Namespace