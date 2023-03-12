//------------------------------------------------------------------------------
//
// File Name:	CameraAnchor.cpp
// Author(s):	Adam Tackett
//						
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "CameraAnchor.h"
#include "imgui.h"

// Dapper Namespace!
namespace Dapper 
{
	CameraAnchor::CameraAnchor()
	{
	}
	CameraAnchor::~CameraAnchor()
	{
	}
	void CameraAnchor::Init()
	{
	}

	void CameraAnchor::Update(float dt)
	{
	}

	void CameraAnchor::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
	{
	}

	void CameraAnchor::Write(IJsonWriter& writer) const
	{
		writer.WriteKey("CameraAnchor");
		writer.StartObject();
		writer.EndObject();
	}

	void CameraAnchor::ImGuiDisplay(float scale)
	{
		scale;
		ImGui::Text("No Editable Values");
	}

} // End Of Dapper Namespace