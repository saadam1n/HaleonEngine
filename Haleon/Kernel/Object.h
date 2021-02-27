#ifndef HALEON_CORE_OBJECT_H // TODO: rename the folder to core
#define HALEON_CORE_OBJECT_H



namespace Haleon {
	/*
	Java style objects: 
	Every haleon object inherits from the object class
	Functions will not be virtual since I don't want to add like 12 bytes to every object in existance
	I do this so I can do some useful functions occasionally that requires any sort of object in existance
	Also one requirement of big goal of this class is not to use any memory added onto the derived class
	*/
	
	class BaseObject {
	public:
	protected:
	private:
	};

}

#endif
