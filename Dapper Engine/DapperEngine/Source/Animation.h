//------------------------------------------------------------------------------
//
// File Name:	Animation.h
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
#include "IComponent.h"
//------------------------------------------------------------------------------

// Dapper Namespace!
namespace Dapper {
	
	class Animation : public IComponent
	{
	public:
		// Constructor
		// Constructors and Destructor
		Animation(Engine* engine1);
		Animation(int index, Engine* engine1);
		~Animation() override;

		// Init function inherited from IComponent
		void Init() override;

		// Get the parent ID
		int GetParentID() override;

		// Set the parent ID
		void SetParentID(int ID);

		// Get the animation's name: "Animation"
		const std::string& GetName() override;

		// Update function inherited from IComponent
		void Update(float dt) override;

		// Read function inherited from IComponent
		void Read(Deserializer& deserializer, const rapidjson::Value& jsonVal) override;

		// Write function inherited from IComponent
		void Write(rapidjson::Value& jsonVal) override;

		// Render function
		void Render();

		// Play an animation sequence.
		void Play();

		// Check to see if the animation is done
		bool IsDone() const;

		// Get the ID
		unsigned GetID() const;

		// Get the parent game object's ID
		unsigned GetParentID() const;

		// Set the ID of the animation
		void SetID(int newID)override;

		// Set the current frame of the animation
		void SetFrame(unsigned new_frame);

		void ImGuiDisplay()override;

	private:
		// The currently displayed frame
		unsigned int frame_index;

		// The number of frames in the sequence
		unsigned int frame_count;

		// The time remaining in the currently displayed frame (if animation is playing)
		float frame_timer;

		// The amount of time to play in each frame
		float frame_duration;

		// Bool that determines if the animation is running
		bool is_running;

		// Bool that determines if the animation loops back to the beginning after it is done
		bool is_looping;

		// Bool that determines if the animation is done
		bool is_done;

		// The ID number of the animation
		int my_ID;

		// The ID of the game object
		int parent_ID;

		// "Animation"
		std::string name = "Animation";

		// Pointer to the engine
		Engine* engine;

	};
	
} // End Of Dapper Namespace

//------------------------------------------------------------------------------


