//------------------------------------------------------------------------------
//
// File Name:	JsonWriter.h
// Author(s):	
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
#include "IJsonWriter.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/ostreamwrapper.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class JsonWriter :public IJsonWriter
  {
  public:
    JsonWriter();
    ~JsonWriter();


    void WriteToFile(const char* filepath);
    void StartObject();
    void EndObject();
    void StartArray();
    void EndArray();
    void WriteString(const char* key, const char* string);
    void WriteFloat(const char* key, float number);
    void WriteInt(const char* key, int number);
    void WriteVec2(const char* key, float x, float y);
    void WriteVec3(const char* key, float x, float y, float z);
    void WriteVec4(const char* key, float x, float y, float z, float w);
    void WriteKey(const char* key);
    void WriteBool(const char* key, bool value);
    void WriteJustString(const char* string);
  private:
    void AddNewLine();
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer;
    unsigned tabs = 0;


    //rapidjson::Writer writer(buffer);

  };

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


