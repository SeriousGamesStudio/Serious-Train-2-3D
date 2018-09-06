#ifndef _H_SKYRENDERER_H_
#define	_H_SKYRENDERER_H_
#include "Component.h"
#include "Components.h"

class GraphicsManager;
class SkyRenderer_c :
	public Component
{
public:
	SkyRenderer_c(bool active, std::string material);
	~SkyRenderer_c();
};
#endif // !_H_SKYRENDERER_H_