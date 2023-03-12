//------------------------------------------------------------------------------
//
// File Name:	Texture.h
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include <GL/glew.h>
#include <string>
#include "Deserializer.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
	class Texture
	{
	public:
		Texture();
		Texture(int the_ID);
		Texture(const char* fileLoc);
		Texture(Texture&& texture) noexcept;
		Texture(const Texture& texture) = default;
		~Texture();

		bool LoadTexture();
		bool LoadTextureA();

		void UseTexture();
		void ClearTexture();

		inline std::string_view GetName() { return the_name; }
		inline std::string GetFileLocation() { return fileLocation; }
		inline void SetFileLocation(std::string filename) { fileLocation = filename; }
		inline void SetID(int id) { ID = id; }
		inline int GetID() { return ID; };

		inline GLuint GetOpenGLID()const { return textureID; }
		void Read(Deserializer& deserial, const rapidjson::Value& jsonVal);


	private:
		const static inline std::string the_name = "Texture";

		GLuint textureID{ 0 };
		int width{ 0 };
		int height{ 0 };
		int bitDepth{ 4 };
		std::string fileLocation{};

		int ID{ -1 };
	};//End of Texture Class

} // End Of Dapper Namespace
//------------------------------------------------------------------------------


