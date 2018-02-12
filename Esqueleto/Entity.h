#ifndef _H_ENTITY_H_
#define _H_ENTITY_H_


#include <vector>
#include <string>

#include "Component.h"
class Entity
{
public:
	Entity(std::string id/*Y supongo que aqui habrá que hacer las movidas de inicializar los componentes*/);
	~Entity();

	void update();

	inline std::string getId() { return id; };
private:
	std::vector<Component> components;
	std::string id;
};

#endif // !_H_ENTITY_H_
