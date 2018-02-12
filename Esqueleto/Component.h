#ifndef _H_COMPONENT_H_
#define _H_COMPONENT_H_


#include <string>

class Component
{
public:
	Component()	 {};
	~Component() {};

	virtual void update() = 0;
};
#endif
