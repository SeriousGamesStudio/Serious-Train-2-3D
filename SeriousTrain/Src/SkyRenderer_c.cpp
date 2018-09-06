#include "SkyRenderer_c.h"
#include "GraphicsManager.h"


SkyRenderer_c::SkyRenderer_c(bool active, std::string material):
	Component(ComponentType::SKYRENDERER)
{	
		GraphicsManager::getInstance()->getSceneManager()->setSkyBox(active, material);
	
}
SkyRenderer_c::~SkyRenderer_c() 
{
	
}