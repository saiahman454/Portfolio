//------------------------------------------------------------------------------
//
// File Name:	Trace.cpp
// Author(s):	Isaiah Dickison
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Trace.h"

// Dapper Namespace!
namespace Dapper
{
  Trace::Trace()
  {
  }

  Trace::~Trace()
  {
  }

  const std::string& Trace::Name() const
  {
    return name;
  }

  void Trace::Initialize()
  {
    tracelog.open("TraceLog.txt");
  }

  void Trace::Update(float dt)
  {
    dt;

  }

  void Trace::Render()
  {
  }

  Trace::operator std::ofstream&()
  {
    return tracelog;
  }

  std::ofstream& Trace::GetStream()
  {
    return tracelog;
  }


  bool Trace::UpdateWhilePaused() const
  {
    return true;
  }

} // End Of Dapper Namespace