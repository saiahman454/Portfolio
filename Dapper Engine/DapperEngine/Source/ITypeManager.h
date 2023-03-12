//------------------------------------------------------------------------------
//
// File Name:	ITypeManager.h
// Author(s):	Isaiah Dickison
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include "ISystem.h"
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
  template<typename TypeToManage> 
  class ITypeManager : public ISystem
  {
    virtual TypeToManage& operator[](int index) = 0;

    virtual int GetNew() = 0;
  };
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


