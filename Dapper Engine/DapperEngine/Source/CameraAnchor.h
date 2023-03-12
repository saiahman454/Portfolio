//------------------------------------------------------------------------------
//
// File Name:	CameraAnchor.h
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "IComponent.h"
#include "Engine.h"
#include "IDManager.h"
#include "IJsonWriter.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper 
{
	class CameraAnchor : public IComponent
	{
	public:
		CameraAnchor();
		~CameraAnchor();
		// Inherited via IComponent
		virtual void Init() override;
		virtual void Update(float dt) override;
		inline virtual int GetParentID() const override { return parentID; }
		inline virtual void SetParentID(int ID) override { parentID = ID; }
		inline virtual const std::string& GetName() const override { return name; }
		virtual void Read(Deserializer& deserial,
			const rapidjson::Value& jsonVal) override;
		inline virtual void SetID(int ID) override { my_ID = ID; }
		virtual void Write(IJsonWriter& writer) const override;
		virtual void ImGuiDisplay(float scale) override;
		inline const int GetID() const override { return my_ID; }

		inline void SetPause(bool Condition)override { paused = Condition; }
		inline bool GetPause() const override { return paused; }

		inline bool IsFollowing() { return follow; }
		inline void SetFollowing(bool new_follow) { follow = new_follow; }

	private:
		inline static const std::string name = "CameraAnchor";
		int parentID; // The ID of the parent Game Object
		int my_ID; // The ID number of the camera anchor
		IDManager ID_manager;
		bool paused = false;
		bool follow = true;

	};//CameraAnchor

  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


