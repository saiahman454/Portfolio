//------------------------------------------------------------------------------
//
// File Name:	Messenger.h
// Author(s):	Isaiah
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include <vector>
#include <functional>
#include <utility>
#include "IDManager.h"
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  template <typename ... Args>
  class Messenger
  {
  public:
    using func = std::function<void(Args...)>;

    /*Messenger() = default;

    Messenger(const Messenger& rhs) = default;

    Messenger& operator=(const Messenger& rhs) noexcept = default;

    Messenger(Messenger&& rhs) noexcept :
      functions(std::move(rhs.functions)),
      theManager(std::move(rhs.theManager))
    {}

    Messenger& operator=(Messenger&& rhs) noexcept
    {
      functions = std::move(rhs.functions);
      theManager = std::move(rhs.theManager);
      return *this;
    }*/

    int RegisterMessage(func frunk)
    {
      //functions.push_back(frunk);
        return theManager.Create(functions, frunk);
    }

    void SendMessage(Args... args)
    {
      for (unsigned i = 0; i < functions.size(); ++i)
      {
        functions[i](std::forward<Args>(args)...);
      }
    }
    void Remove(int Id)
    {
        theManager.Destroy(functions, Id);
    }
  private:
    std::vector<func> functions;
    IDManager theManager;

  };


} // End Of Dapper Namespace

//------------------------------------------------------------------------------


