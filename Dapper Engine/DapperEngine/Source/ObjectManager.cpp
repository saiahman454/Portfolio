

#include "ObjectManager.h"
#include "Object.h"


void ManagerTest()
{
	//Manager<BaseObjectPtr> * managerObj = new Manager<BaseObjectPtr>();
	


	
}




// Lambda Function, every object					
//in list
// uses template from Manager Class
// include typeinfo specific playerstuff
// No need to have a group/ objType enum
 // Use typeinfo, separate inheritedclasses
// for each object type. 

	
/*
	objList.execute
	([](EngObjPtr obj) // Lambda Function, every object in list
	{							  // uses template from Manager Class
		if(isType(obj, PlayerObjPtr)) // include typeinfo specific player stuff
		{							  // No need to have a group/ objType enum
			// Player				  // Use typeinfo, separate inherited classes
									  // for each object type. 
		}
		obj->Update();
	}
	objList.render
	([] (EngObjPtr obj)
	{
		
	}
	);*

	if (isType(obj, PlayerObjPtr))
	{
		// Player

	}

/*
 * reserve space for Cmps that need to be accessed very fast (Graphics)
 * custom Cmp, be able to add on to end.
 * "Boilerplate" object, then build from there

for(int i = 0; i < objList.size(); ++i)
{
	obj[i]->update();
}

for (ObjPtr obj : objList) // "for each" is faster
{
	obj->update();
}
*/