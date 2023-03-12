//------------------------------------------------------------------------------
//
// File Name:	SpineAnimation.cpp
// Author(s):	Natalie Bouley
//						
//
//
// Copyright ?2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "AnimationSpine.h"
#include <spine/spine.h>
#include "Texture.h"
#include "SpineTextureLoader.h"
#include "Utility.h"
#include "MeshManager.h"
#include "Mesh.h"
#include "Sprite.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Transform.h"


// The face indices for a standard square
static const unsigned __standardSquare_faces[] = { 0, 1, 2, 2, 3, 0 };

spine::SpineExtension* spine::getDefaultExtension() {
  return new spine::DefaultSpineExtension();
}

// Dapper Namespace!
namespace Dapper {

  AnimationSpine::AnimationSpine(int ID, Engine* engine_)
    : data(nullptr)
    , instance(nullptr)
    , is_looping(false)
    , mesh_ID(-1)
    , my_ID(ID)
    , parent_ID(-1)
    , filename({})
    , engine(engine_)
  {
    
  }

  AnimationSpine::~AnimationSpine()
  {
    delete instance;
    delete data;
  }

  void AnimationSpine::Update(float dt)
  {
    if (!instance) return;

    if (mesh_ID == -1)
    {
      mesh_ID = GetComponentFromParent<Sprite>(*engine, parent_ID, "Sprite")->GetMesh();
    }
    Transform* transform = GetComponentFromParent<Transform>(*engine, parent_ID, "Transform");

    // Get the skeleton and state from the instance
    spine::Skeleton* skeleton = instance->GetSkeleton();
    spine::AnimationState* state = instance->GetAnimationState();

    if (!skeleton || !state) return;

    // When the computeWorldVertices function is called, this will hold the vertices of an attachment
    int vec_offset = 0; // Offset of the vector to read from
    int total_vertices = 0; // The total amount of vertices that are in the entire mesh

    // update the spine instance

    float scale_x = 0.5f / transform->GetScale().x;
    float scale_y = 0.5f / transform->GetScale().y;
    skeleton->setScaleX(scale_x * 0.375f);
    skeleton->setScaleY(scale_y * 0.375f);
    skeleton->updateWorldTransform();
    state->update(dt);
    state->apply(*skeleton);

    // Get a reference to the slot vector in the order we want to draw it
    spine::Vector<spine::Slot*>& slots = skeleton->getDrawOrder();
    int slot_count = slots.size();

    // Precompute the amount of vertices in the mesh
    for (int i = 0; i < slot_count; ++i)
    {
      spine::Attachment* attachment = slots[i]->getAttachment();

      if (!attachment)
      {
        continue;
      }
      if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti))
      {
        total_vertices += 20;
      }
      else if (attachment->getRTTI().isExactly(spine::MeshAttachment::rtti))
      {
        spine::MeshAttachment* meshAttachment = (spine::MeshAttachment*)attachment;
        total_vertices += meshAttachment->getWorldVerticesLength() / 2 * 5;
      }
    }

    // Resize and clear the vectors to update with the new ones
    vertices.resize(total_vertices);
    faces.clear();

    // Loop through all the attachments in the skeleton
    for (int i = 0; i <= slot_count - 1; i++)
    {
      spine::Slot* slot = slots[i];
      spine::Attachment* attachment = slot->getAttachment();
      // If no attachment is found, check the next slot.
      if (!attachment) continue;

      // Check if the attachment is a mesh or region attachment.
      if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti))
      {
        spine::RegionAttachment* region_attachment = (spine::RegionAttachment*)attachment;
        // Compute attachment vertices with the computeWorldVertices function.
        region_attachment->computeWorldVertices(slots[i]->getBone(), &vertices[vec_offset], 0, 5);

        int uvoffset = 0;
        // Set the vertices to match the region attachment's vertices.
        for (int j = 0; j < 4; ++j)
        {
          vertices[j * 5 + vec_offset + 3] = region_attachment->getUVs()[uvoffset];
          vertices[j * 5 + vec_offset + 4] = region_attachment->getUVs()[uvoffset + 1];
          uvoffset += 2;
        }
        // Set the faces to match the region attachment's faces.
        for (int j = 0; j < 6; ++j)
        {
          faces.push_back(__standardSquare_faces[j] + vec_offset / 5);
        }
        vec_offset += 20;
      }
      else if(attachment->getRTTI().isExactly(spine::MeshAttachment::rtti))
      {
        spine::MeshAttachment* mesh_attachment = (spine::MeshAttachment*)attachment;

        int num_vertices = mesh_attachment->getWorldVerticesLength() / 2;
        int uvoffset = 0;
        // Compute attachment vertices with the computeWorldVertices function.
        mesh_attachment->computeWorldVertices(*(slots[i]), 0, num_vertices * 2, &vertices[vec_offset], 0, 5);

        // Set the vertices to match the mesh attachment's vertices.
        for (int j = 0; j < num_vertices; ++j)
        {
          vertices[j * 5 + vec_offset + 3] = mesh_attachment->getUVs()[uvoffset];
          vertices[j * 5 + vec_offset + 4] = mesh_attachment->getUVs()[uvoffset + 1];
          uvoffset += 2;
        }

        // Update the faces for the mesh.
        spine::Vector<unsigned short>& new_faces = mesh_attachment->getTriangles();
        int faces_num = new_faces.size();
        for (int j = 0; j < faces_num; ++j)
        {
          faces.push_back(new_faces[j] + vec_offset / 5);
        }

        vec_offset += num_vertices * 5;
      }
    }

    // Get the mesh from the Mesh Manager, and call the SetVertices and SetFaces functions
    MeshManager* mesh_manager = GetSystemFromEngine<MeshManager*>(*engine, "MeshManager");
    Mesh& mesh = (*mesh_manager)[mesh_ID];
    mesh.SetVertices(vertices);
    mesh.SetFaces(faces);
  }

  // Play an animation
  void AnimationSpine::Play()
  {
    instance->GetAnimationState()->setAnimation(0, name.c_str(), is_looping);
  }

  // Initialize a Spine Animation instance
  void AnimationSpine::Init()
  {
    if (!filename.empty())
    {
      // Allocate memory for the instance and data
      data = new SpineData(filename, this);
      instance = new SpineInstance(data);
    }

    // Play the spine animation with the first default animation
    //instance->GetAnimationState()->addAnimation(0, "Idle", true, 0);
  }

  // Read a spine instance from the json
  void AnimationSpine::Read(Deserializer& deserial, const rapidjson::Value& jsonVal)
  {
    // Read the filepath name from the json
    auto object = jsonVal.GetObject();
    filename = object["Name"].GetString();

    deserial;
  }

  // Write the data of the spine instance to a json file
  void AnimationSpine::Write(IJsonWriter& writer) const
  {
    writer;
    writer.WriteKey("Animation");
    writer.StartObject();
    writer.WriteString("Type", "Spine");
    writer.WriteString("Name", filename.c_str());
    writer.EndObject();
  }

  // Display the data of the Spine Instance to the ImGui window
  void AnimationSpine::ImGuiDisplay(float scale)
  {
    scale;
    ImGui::Text("Spine Animation:");
    ImGui::InputText(" ", &filename);
    if (ImGui::Button("Change Source", { 100 * scale,50 * scale }))
    {
      delete data;
      delete instance;
      data = new SpineData(filename, this);
      instance = new SpineInstance(data);
    }
  }
} // End Of Dapper Namespace