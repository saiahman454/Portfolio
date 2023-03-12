//------------------------------------------------------------------------------
//
// File Name:	Texture.cpp
// Author(s):	Adam Tackett	
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Texture.h"
#include "CommonValues.h"
#include <iostream>

// Dapper Namespace!
namespace Dapper 
{
	Texture::Texture()
	{
	}
	Texture::Texture(int the_ID) : ID(the_ID)
	{
		textureID = -1;
		width = 0;
		height = 0;
		bitDepth = 0;
		fileLocation = ""; //NULL works aswell
	}

	Texture::Texture(const char* fileLoc)
	{
		textureID = -1;
		width = 0;
		height = 0;
		bitDepth = 0;
		fileLocation = fileLoc;
	}

	Texture::Texture(Texture&& texture) noexcept : 
		textureID(texture.textureID),
		width(texture.width), 
		height(texture.height),
		bitDepth(texture.bitDepth), 
		fileLocation(std::move(texture.fileLocation)),
		ID(texture.ID)
	{
		texture.textureID = -1;
	}

	Texture::~Texture()
	{
		ClearTexture();
	}

	bool Texture::LoadTexture()
	{
		unsigned char* texData = stbi_load(fileLocation.c_str(), 
										   &width, &height,
										   &bitDepth, STBI_default);
		if (!texData)
		{
			std::cout << "Failed to find" << fileLocation << "\n";
			return false;
		}

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		//&&&Adam Trying to get Alpha working
		//glEnable(GL_DEPTH_TEST);
		//glEnablei(GL_BLEND, 1);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		// This should make it so that you don't need to have a separate function for alpha.
		// As it just finds out the amount of color channels from the file on load.
		GLint internalFormat;
		GLint format;

		switch(bitDepth)
		{
		case 3:
			internalFormat = GL_RGB;
			format = GL_RGB;
			break;
		case 4:
			internalFormat = GL_RGBA;
			format = GL_RGBA;
			break;
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // can use linear or nearest(more pixalated)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);//&&&Adam Check
		stbi_image_free(texData);

		return true;
	}

	// Used in sprite draw
	bool Texture::LoadTextureA()
	{
		unsigned char* texData = stbi_load(fileLocation.c_str(), 
										   &width, &height, 
										   &bitDepth, STBI_rgb_alpha);
		if (!texData)
		{
			std::cout << "Failed to find" << fileLocation << "\n";
			return false;
		}

		// Set settings for drawing the texture
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // can use linear or nearest(more pixalated)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData); //RGBA ~ RGB
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(texData);

		return true;
	}

	void Texture::UseTexture()
	{
		glActiveTexture(GL_TEXTURE0); //texture unit
		glBindTexture(GL_TEXTURE_2D, textureID); //Bind tex to 0
	}

	void Texture::ClearTexture()
	{
		glDeleteTextures(1, &textureID);
		textureID = 0;
		width = 0;
		height = 0;
		bitDepth = 0;
		fileLocation = "";
	}

	void Texture::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
		fileLocation =	jsonVal.GetString();
		deserial;
	}

} // End Of Dapper Namespace