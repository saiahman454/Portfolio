//------------------------------------------------------------------------------
//
// File Name:	JsonWriter.cpp
// Author(s):	Isaiah Dickison
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "JsonWriter.h"
#include <string>


#include <filesystem>
#include <fstream>
#include <iostream>
// Dapper Namespace!
namespace Dapper {

  /*--------------------------------------
  Description: Writer constuctor
  ---------------------------------------*/
  JsonWriter::JsonWriter() :
    writer(buffer)
  {
  }
  /*--------------------------------------
  Description: Writer destuctor
  ---------------------------------------*/
  JsonWriter::~JsonWriter()
  {
  }

  /*--------------------------------------
  Description: writes all gameobjects to a file
  Params: filepath - the file path
  ---------------------------------------*/
  void JsonWriter::WriteToFile(const char* filepath)
  {
    std::string filename = filepath;
    std::ofstream intofile(filename, std::ios::out | std::ios::trunc);
    if (intofile.is_open())
    {
      intofile << buffer.GetString() << std::endl;
      intofile.close();
      std::cout << "Successfully saved" << filepath << std::endl;
    }
    else
    {

      std::cout << "could not open" << filepath << std::endl;
    }
  }

  /*--------------------------------------
  Description: start of a json object
  ---------------------------------------*/
  void JsonWriter::StartObject()
  {
    AddNewLine();
    writer.StartObject();
    ++tabs;
  }

  /*--------------------------------------
  Description: end of a json object
  ---------------------------------------*/
  void JsonWriter::EndObject()
  {
    --tabs;
    AddNewLine();
    writer.EndObject();
  }

  /*--------------------------------------
  Description: start of a json array
  ---------------------------------------*/
  void JsonWriter::StartArray()
  {
    writer.StartArray();
    ++tabs;
  }

  /*--------------------------------------
  Description: end of a jsom array
  ---------------------------------------*/
  void JsonWriter::EndArray()
  {
    --tabs;
    AddNewLine();
    writer.EndArray();
  }


  /*--------------------------------------
  Description: writes a string with a key
  Params:
  key - the key
  String - the string
  ---------------------------------------*/
  void JsonWriter::WriteString(const char* key, const char* string)
  {
    AddNewLine();
    writer.Key(key);
    writer.String(string);
  }

  /*--------------------------------------
  Description: writes a float with a key
  Params:
  key - the key
  number - the number you want to write
  ---------------------------------------*/
  void JsonWriter::WriteFloat(const char* key, float number)
  {
    AddNewLine();
    writer.Key(key);
    writer.Double(number);
  }

  /*--------------------------------------
  Description: writes a int with a key
  Params:
  key - the key
  number - the number you want to write
  ---------------------------------------*/
  void JsonWriter::WriteInt(const char* key, int number)
  {
    AddNewLine();
    writer.Key(key);
    writer.Int(number);
  }

  /*--------------------------------------
  Description: writes a vec2 with a key
  Params:
  key - the key
  x - first number you want to write
  y - second number you want to write
  ---------------------------------------*/
  void JsonWriter::WriteVec2(const char* key, float x, float y)
  {
    
    AddNewLine();
    writer.Key(key);
    writer.StartArray();
    writer.Double(x);
    writer.Double(y);
    writer.EndArray();
  }

  /*--------------------------------------
  Description: writes a vec3 with a key
  Params:
  key - the key
  x - first number you want to write
  y - second number you want to write
  z - third number you want to write
  ---------------------------------------*/
  void JsonWriter::WriteVec3(const char* key, float x, float y, float z)
  {
    AddNewLine();
    writer.Key(key);
    writer.StartArray();
    writer.Double(x);
    writer.Double(y);
    writer.Double(z);
    writer.EndArray();
  }

  /*--------------------------------------
  Description: writes a vec4 with a key
  Params:
  key - the key
  x - first number you want to write
  y - second number you want to write
  z - third number you want to write
  w - fourth number you want to write
  ---------------------------------------*/
  void JsonWriter::WriteVec4(const char* key, float x, float y, float z, float w)
  {
    AddNewLine();
    writer.Key(key);
    writer.StartArray();
    writer.Double(x);
    writer.Double(y);
    writer.Double(z);
    writer.Double(w);
    writer.EndArray();
  }

  /*--------------------------------------
  Description: writes a key
  Params:
  key - the key
  ---------------------------------------*/
  void JsonWriter::WriteKey(const char* key)
  {
    AddNewLine();
    writer.Key(key);
  }

  /*--------------------------------------
  Description: writes a bool
  Params:
  key - the key
  value - the bool you want to write
  ---------------------------------------*/
  void JsonWriter::WriteBool(const char* key, bool value)
  {
    AddNewLine();
    writer.Key(key);
    writer.Bool(value);
  }

  void JsonWriter::WriteJustString(const char* string)
  {
    writer.String(string);
  }

  /*--------------------------------------
  Description: adds a new ling in the file
  ---------------------------------------*/
  void JsonWriter::AddNewLine()
  {
    buffer.Put('\n');
    for (unsigned i = 0; i < tabs; ++i) {
      buffer.Put('\t');
    }
  }



} // End Of Dapper Namespace