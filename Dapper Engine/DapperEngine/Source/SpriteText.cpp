//------------------------------------------------------------------------------
//
// File Name:	SpriteText.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "SpriteText.h"
#include "IComponent.h"
#include "Engine.h"
#include "GameObjectManager.h"
#include "TransformManager.h"
#include "MeshManager.h"
#include "SpriteSource.h"
#include "Utility.h"
#include "Renderer.h"
#include "imgui.h"
#include "imgui_stdlib.h"

// Dapper Namespace!
namespace Dapper {

    SpriteText::SpriteText()
    {
    }

    SpriteText::SpriteText(int ID, Engine* _engine)
        : my_ID(ID)
        , engine(_engine)
    {
    }

    SpriteText::SpriteText(Engine* _engine)
        : engine(_engine)
    {
    }

    void SpriteText::Init()
    {
    }

    void SpriteText::Update(float dt)
    {

    }

    // Render the sprite text
    void SpriteText::Render(Engine& engine)
    {
        if (isEnabled == false)
        {
            return;
        }
        // Pass the draw data to the mesh render function
        // Grab the transform and game object.
        GameObjectManager* gameObjectManager = dynamic_cast<GameObjectManager*>(engine.Get("GameObjectManager"));
        TransformManager* transformManager = dynamic_cast<TransformManager*>(engine.Get("TransformManager"));
        SpriteSource* spriteSource = GetComponentFromParent<SpriteSource>(engine, parent_ID, "SpriteSource");

        GameObject& gameObject = (*gameObjectManager)[parent_ID];
        int transformID = gameObject.GetComponentID("Transform");
        Transform& transform = (*transformManager)[transformID];

        // Grab the position and scale from the game object's transform.
        glm::vec3 position = transform.GetTranslation();
        glm::vec2 scale = transform.GetScale();
        float x_pos = position.x;

        // Call the renderer's draw from the engine.
        Renderer* renderer = dynamic_cast<Renderer*>(engine.Get("Renderer"));

        // For each letter, set the proper U and V values.

        for (auto& ch : text)
        {
            // Get the UV values of the spot in the texture of the letter
            int letter_index = ch - 32;
            auto theUV = spriteSource->CalculateUVWithIndex(letter_index);

            // Set all of the draw data.
            DrawData draw_data;
            draw_data.position = glm::vec3(x_pos, position.y, position.z);
            draw_data.scale = scale;
            draw_data.meshIndex = meshID;
            draw_data.textureID = spriteSource->GetTexture();
            draw_data.rotation = transform.GetRotation();
            draw_data.theU = theUV.first;
            draw_data.theV = theUV.second;
            draw_data.theMinUV = spriteSource->GetCustomMin();
            draw_data.theMaxUV = spriteSource->GetCustomMax();
            draw_data.theColor = { tint.r, tint.g, tint.b, tint.a };
            renderer->Draw(draw_data);

            // Increase x position so letters draw side-to-side
            x_pos += scale.x;
        }
    }

    void SpriteText::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
    {
        auto object = jsonVal.GetObject();
        // Read all of the necessary private variables in the class.
        MeshManager* theMeshManager = GetSystemFromEngine<MeshManager*>
            (deserial.GetEngine(), "MeshManager");

        auto meshInfo = deserial.Deserialize("Mesh", object["Mesh"]);
        meshID = meshInfo.second;

        auto new_name = object["Text"].GetString();
        text.clear();
        text += new_name;
        if (text.empty())
        {
            text = "Sample Text";
        }

        auto tint_array = object["Tint"].GetArray();
        tint.r = tint_array[0].GetFloat();
        tint.g = tint_array[1].GetFloat();
        tint.b = tint_array[2].GetFloat();
        tint.a = tint_array[3].GetFloat();
    }

    void SpriteText::Write(IJsonWriter& writer) const
    {
        writer.WriteKey("SpriteText");
        writer.StartObject();

        MeshManager& theMeshManager = *GetSystemFromEngine<MeshManager*>(*engine, "MeshManager");
        writer.WriteString("Mesh", theMeshManager[meshID].GetName().c_str());

        writer.WriteVec4("Tint", tint.r, tint.g, tint.g, tint.a);

        writer.WriteString("Text", text.c_str());
        writer.EndObject();
    }

    void SpriteText::ImGuiDisplay(float scale)
    {
        ImGui::InputFloat4("Tint", &tint[0]);
        glm::clamp(tint, 0.0f, 1.0f);

        ImGui::InputText("Text", &text);
    }

    void SpriteText::OnMeshReallocation(MeshManager& mesh_manager)
    {
        if (meshID == -1) return;
        mesh = &mesh_manager[meshID];
    }


} // End Of Dapper Namespace