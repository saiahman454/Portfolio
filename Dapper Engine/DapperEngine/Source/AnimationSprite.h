//------------------------------------------------------------------------------
//
// File Name:	AnimationSprite.h
// Author(s):	Natalie Bouley
//						
//						
//
// Copyright 2021 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------
#include "IAnimation.h"

//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
	
	class AnimationSprite : public IAnimation
	{
	public:
		// Constructor and Destructor
		AnimationSprite(int index, Engine* engine1);
		~AnimationSprite() override;

		// Get and Set functions
		inline int GetParentID() const override { return parent_ID; }
		inline void SetParentID(int ID) { parent_ID = ID; }
		inline bool IsDone() const { return is_done; }
		inline void SetID(int newID)override { my_ID = newID; }
		inline void SetFrame(unsigned new_frame) { frame_index = new_frame; }
		inline const int GetID() const override { return my_ID; }
		inline const std::string& GetName() const override { return name; }

		// Init function inherited from IComponent
		void Init() override;

		// Update function inherited from IComponent
		void Update(float dt) override;

		// Read function inherited from IComponent
		void Read(Deserializer& deserializer, const rapidjson::Value& jsonVal) override;

		// Write function inherited from IComponent
		void Write(IJsonWriter& writer) const override;

		// Render function
		void Render();

		// Play an animation sequence.
		void Play();

		// Display function for ImGui
		void ImGuiDisplay(float scale)override;

		inline void SetPause(bool Condition)override { paused = Condition; }
		inline bool GetPause() const override { return paused; }

	private:
		static inline const  std::string name = "AnimationSprite";
		unsigned int frame_index; // The currently displayed frame
		unsigned int frame_count; // The number of frames in the sequence
		float frame_timer; // The time remaining in the currently displayed frame (if animation is playing)
		float frame_duration; // The amount of time to play in each frame
		bool is_running; // Bool that determines if the animation is running
		bool is_looping; // Bool that determines if the animation loops back to the beginning after it is done
		bool is_done; // Bool that determines if the animation is done
		int my_ID; // The ID number of the animation
		int parent_ID; // The ID of the game object
		Engine* engine; // Pointer to the engine
		bool paused = false;
	};
	
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


