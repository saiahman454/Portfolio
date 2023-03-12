//------------------------------------------------------------------------------
//
// File Name:	Utility.cpp
// Author(s):	Isaiah Dickison, Adam Tackett
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Utility.h"

// Dapper Namespace!
namespace Dapper {



  float RandomRange(float min, float max)
  {
    float randomNum = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
    return randomNum;
  }

} // End Of Dapper Namespace