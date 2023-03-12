#pragma once
#include "Physics.h"

namespace Dapper
{
	typedef class Obj* ObjPtr;
	enum ObjState
	{
		objDestroy = -2, // change to this state to flag for destruction in the manager
		objInvalid,		 // Initial and error state 
		objRender,		 // Other calculations done, data ready to render.
		objCheckStatus,	 // Debug state?
		objUpdate,		 // Update the object. while in this state, will stay in
						 //	its update loop.
	};

	typedef class BaseObject
	{
	public:
		BaseObject() : state_(objInvalid), renderReady_(0) {}
		// Remember! The FSM does no rendering, only flags for render!
		void FSM();
		//virtual void checkStatus() = 0; // pure virtual
		//virtual void Render() = 0;
		//virtual void Execute() = 0;
		bool checkRenderReady();

		ObjState getState() { return state_; }
		bool isActive()
		{
			if ((getState() > objInvalid) && (getState() <= objRender))
			{
				return true;
			}
			else
				return false;
		}
		void setState(ObjState state)
		{
			state_ = state;
		}
		void operator++() { ++renderReady_; }


	private:
		ObjState state_;
		ObjState nextState_;
		// SpritePtr sprite_and_animation_data;
		int renderReady_; // bool?

	} *BaseObjectPtr;

	// &&&Jacob
	// Idk naming, This will be used for all physical objects in the game,
	// platforms, player and enemies, hazards etc
	// Point being that we can use this gameObject for anything with Physics, transform and etc
	typedef class GameObject : public BaseObject
	{
	public:
		GameObject(PhysicsPtr physics, TransformPtr transform) :
			physics_(physics), transform_(transform) {}


	private:
		PhysicsPtr physics_;
		TransformPtr transform_;
		//PoolPtr* health_and_oxygen_pool

	} *PlayerObjectPtr;
	
}







