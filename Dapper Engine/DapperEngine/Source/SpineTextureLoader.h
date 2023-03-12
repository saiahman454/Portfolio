//------------------------------------------------------------------------------
//
// File Name:	SpineTextureLoader.h
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include <spine/spine.h>
#include "Texture.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  // Custom Texture Loader for Spine to use
  class TextureLoader : public spine::TextureLoader
  {
  public:
    // Constructor and Destructor
    TextureLoader();
    virtual ~TextureLoader() override;
  
    // Load a texture
    void load(spine::AtlasPage& page, const spine::String& path) override;
  
    // Unload a texture
    void unload(void* texture) override;
  
  };

} // End Of Dapper Namespace