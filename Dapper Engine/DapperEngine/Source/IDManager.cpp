//------------------------------------------------------------------------------
//
// File Name:	ManagerUtility.cpp
// Author(s):	
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "IDManager.h"


// Dapper Namespace!
namespace Dapper {


  /*--------------------------------------
  Description: Constructor for IDManager
  ---------------------------------------*/
  IDManager::IDManager()
  {
  }

  /*--------------------------------------
  Description: destructor for IDManager
  ---------------------------------------*/
  IDManager::~IDManager()
  {
  }

  /*--------------------------------------
  Description: ID to index operator
  ---------------------------------------*/
  int IDManager::operator[](int ID) const
  {
    auto iter = ID_to_index.find(ID);

    assert(iter != ID_to_index.end());
    return iter->second;
  }

  int IDManager::IndexToId(int index) const
  {
      auto theIndex = index_to_ID.find(index);

      assert(theIndex != index_to_ID.end());
      return theIndex->second;
  }

  bool IDManager::Has(int ID) const
  {
      // Checks if the ID is in the manager
      auto iter = ID_to_index.find(ID);
      if (iter != ID_to_index.end())
      {
          // Verify the ID in index_to_ID matches the ID in ID_to_index
          assert(index_to_ID.find(iter->second)->second == iter->first);
          return true;
      }
      return false;
  }

} // End Of Dapper Namespace