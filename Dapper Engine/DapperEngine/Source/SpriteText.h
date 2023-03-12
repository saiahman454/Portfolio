//------------------------------------------------------------------------------
//
// File Name:	SpriteText.h
// Author(s):	Natalie Bouley
//						
//						
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "IComponent.h"
#include "Mesh.h"
#include "MeshManager.h"
#include "Messenger.h"
#include <glm/vec4.hpp>
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {

    class SpriteText : public IComponent
    {
    public:

        SpriteText();

        SpriteText(int ID, Engine* engine);

        SpriteText(Engine* _engine);

        // Inherited from IComponent
        virtual void Init();

        virtual void Update(float dt);

        void Render(Engine& engine);

        virtual ~SpriteText() {}

        inline virtual int GetParentID() const { return parent_ID; }

        inline virtual void SetParentID(int ID) { parent_ID = ID; }

        inline virtual const std::string& GetName() const { return name; }

        virtual void Read(Deserializer& deserial, const rapidjson::Value& jsonVal);

        virtual void SetID(int ID) { my_ID = ID; }

        //dont implement until level editor is up
        virtual void Write(IJsonWriter& writer) const;

        virtual void ImGuiDisplay(float scale);

        inline virtual const int GetID() const { return my_ID; }

        inline virtual void SetPause(bool Condition) { paused = Condition; }
        inline virtual bool GetPause() const { return paused; }
        inline void SetText(std::string new_text) { text = new_text; }

        void OnMeshReallocation(MeshManager& mesh_manager);

        // Manager needs this so it's public
        Messenger<SpriteText*> on_reallocate; // When the component is moved in memory

    private:
        int my_ID; // The ID of the component
        int parent_ID; // The ID of the parent game object
        Engine* engine; // Can reference the engine
        static inline const std::string name = "SpriteText"; // Component Name
        bool paused = false; // If component is paused

        std::string text = ""; // Text that is displayed
        glm::vec4 tint; // The tint of the sprite text
        int meshID; // The meshID
        Mesh* mesh; // The mesh
        int num_cols = 16; // Columns in font sheet
        int num_rows = 6; // Rows in font sheet
    };
  
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


