//------------------------------------------------------------------------------
//
// File Name:	IJsonWriter.h
// Author(s):	Isaiah
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

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class IJsonWriter
  {
  public:
    virtual void WriteToFile(const char* filepath) = 0;
    virtual void StartObject() = 0;
    virtual void EndObject() = 0;
    virtual void StartArray() = 0;
    virtual void EndArray() = 0;
    virtual void WriteString(const char* key, const char* string) = 0;
    virtual void WriteFloat(const char* key, float number) = 0;
    virtual void WriteInt(const char* key, int number) = 0;
    virtual void WriteVec2(const char* key, float x, float y) = 0;
    virtual void WriteVec3(const char* key, float x, float y, float z) = 0;
    virtual void WriteVec4(const char* key, float x, float y, float z, float w) = 0;
    virtual void WriteKey(const char* key) = 0;
    virtual void WriteBool(const char* key, bool value) = 0;
    virtual void WriteJustString(const char* string) = 0;
  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


