#include "Object.h"

namespace Dapper
{
	void BaseObject::FSM()
	{
		switch (this->getState())
		{
		case(objDestroy):
		{


		}break;
		case(objInvalid):
		{


		}break;
		case(objRender):
		{

		}break;
		case(objCheckStatus):
		{

		}break;
		case(objUpdate):
		{
			//check physics, input, etc.
			// 


		}break;



		}
	}
}



