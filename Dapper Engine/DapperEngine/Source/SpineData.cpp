//------------------------------------------------------------------------------
//
// File Name:	SpineData.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "SpineData.h"
#include "SpineTextureLoader.h"
#include "Trace.h"
#include "AnimationSpine.h"
#include <iostream>
// Dapper Namespace!
namespace Dapper {


/*********************************** Texture Loader ************************************/
  // Declare a single instance of the texture loader
    static TextureLoader* texture_loader_ = NULL;
  
  /*--------------------------------------
  Description: Default Constructor
  Params:
  Return: Void
  ---------------------------------------*/
  TextureLoader::TextureLoader()
  {
  }

  /*--------------------------------------
  Description: Destructor
  Params:
  Return: Void
  ---------------------------------------*/
  TextureLoader::~TextureLoader() 
  {
  }

  /*--------------------------------------
  Description: Loads in a texture for the spine animation
  Params: page_- the AtlasPage whose renderer object will be set to the texture
          path- the string whose name is the name of the texture 
  Return: Void
  ---------------------------------------*/
  void TextureLoader::load(spine::AtlasPage& page_, const spine::String& path)
  {
    void* texture = new Texture(path.buffer());
    page_.setRendererObject(texture);
  }

  /*--------------------------------------
  Description: Unloads a given texture
  Params: texture- the texture to be unloaded
  Return: Void
  ---------------------------------------*/
  void TextureLoader::unload(void* texture)
  {
    delete static_cast<Dapper::Texture*>(texture);
  }

  /*--------------------------------------
  Description: Gets the instance of the texture loader
  Params: 
  Return: A pointer to the texture loader
  ---------------------------------------*/
  TextureLoader* GetTextureLoader()
  {
    if (texture_loader_)
    {
      return texture_loader_;
    }
    texture_loader_ = new TextureLoader();
    return texture_loader_;
  }
  /****************************** End of Texture Loader *******************************/

  /*--------------------------------------
  Description: Default Constructor
  Params: filename- The name of the file to be read from
          _parent- The parent AnimationSpine component
  Return: Void
  ---------------------------------------*/
  SpineData::SpineData(std::string& filename, AnimationSpine* _parent)
    : parent(_parent)
  {
    ReadData(filename);
    animations = skeleton_data->getAnimations();
  }

  /*--------------------------------------
  Description: Destructor
  Params: 
  Return: Void
  ---------------------------------------*/
  SpineData::~SpineData()
  {
    delete atlas;
    delete skeleton_data;
    delete animation_state_data;
  }


  /*--------------------------------------
  Description: Read all of the data from a given file for a Spine animation
  Params: filename- The name of the file to be read from
  Return: Void
  ---------------------------------------*/
  void SpineData::ReadData(std::string& filename)
  {
    // Load the custom atlas
    atlas = new spine::Atlas(("./Assets/Spine/" + filename + ".atlas").c_str(), GetTextureLoader());

    // Read the skeleton data and animation state data from the file.
    spine::SkeletonJson atlas_json(atlas);
    skeleton_data = atlas_json.readSkeletonDataFile(("./Assets/Spine/" + filename + ".json").c_str());
    // Uncomment these two lines to read from binary instead of from a json file
    //spine::SkeletonBinary atlas_binary(atlas);
    //skeleton_data = atlas_binary.readSkeletonDataFile(("./Assets/Spine/" + filename + ".skel").c_str());
    animation_state_data = new spine::AnimationStateData(skeleton_data);

    // Error check
    if (!skeleton_data || !animation_state_data)
    {
      std::ofstream& trace = (*dynamic_cast<Trace*>(parent->GetEngine()->Get("Trace"))).GetStream();
      trace << atlas_json.getError().buffer() << std::endl;
    }
  }

} // End Of Dapper Namespace