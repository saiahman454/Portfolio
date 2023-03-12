//------------------------------------------------------------------------------
//
// File Name:	ManagerUtility.h
// Author(s):	Isaiah Dickison
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once
#include <unordered_map>
#include <assert.h>
//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
  class IDManager
  {
  public:
    IDManager();
    ~IDManager();

    template<typename T>
    int Create(std::vector<T>& vector, const T& obj);

    template<typename T>
    int Create(std::vector<T>& vector, T&& obj);
    
    int operator[](int ID) const;

    template<typename T>
    void Destroy(std::vector<T>& vector, unsigned int id);

    template<typename T>
    void Resort(std::vector<T>& ID_manager);

    int IndexToId(int index) const;

    bool Has(int ID) const;

  private:
    std::unordered_map<unsigned int, unsigned int> ID_to_index;
    std::unordered_map<unsigned int, unsigned int> index_to_ID;
    unsigned int ID_counter = 0;
  };
  
  template<typename T>
  int IDManager::Create(std::vector<T>& vector, const T& obj)
  {
    int index = (int)vector.size();
    ID_to_index[ID_counter] = index;
    index_to_ID[index] = ID_counter;
    vector.push_back(obj);
    return ID_counter++;
  }

  template<typename T>
  int IDManager::Create(std::vector<T>& vector, T&& obj)
  {
    int index = (int)vector.size();
    ID_to_index[ID_counter] = index;
    index_to_ID[index] = ID_counter;
    vector.push_back(std::move(obj));
    return ID_counter++;
  }

  template<typename T>
  void IDManager::Destroy(std::vector<T>& vector, unsigned int id)
  {
    //possible issue index might be passed not ID
    assert(ID_to_index.find(id)!= ID_to_index.end());
    // get the index of the item to delete
    unsigned int index = ID_to_index[id];
    assert(index_to_ID.find(index) != index_to_ID.end());
    // get the address of the back item for later use
    unsigned int back_address = index_to_ID[(int)vector.size() - 1];
    // remove item from the vector by copying over it with the back item
    vector[index] = std::move(vector.back());
    // pop the back item
    vector.pop_back();
    // update index_to_address using the index we just removed, and the previously acquired address
    index_to_ID[index] = back_address;
    // update address_to_index using the address previously acquired and the new index
    ID_to_index[back_address] = index;

    index_to_ID.erase((unsigned int)vector.size());
    ID_to_index.erase(id);
  }

  template<typename T>
  void IDManager::Resort(std::vector<T>& vec)
  {
      // Clear the maps and then resort them
      int vec_size = vec.size();
      ID_to_index.clear();
      index_to_ID.clear();
      for (int i = 0; i < vec_size; ++i)
      {
          int ID = vec[i].GetID();
          ID_to_index[ID] = i;
          index_to_ID[i] = ID;
      }
  }

} // End Of Dapper Namespace

//------------------------------------------------------------------------------


