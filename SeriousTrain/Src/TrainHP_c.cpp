#include "TrainHP_c.h"
#include "Scene.h"
#include "Game.h"
#include "GraphicsManager.h"

TrainHP_c::TrainHP_c(int life): Component(ComponentType::TRAINHP), life_(life)
{
}

TrainHP_c::~TrainHP_c()
{
	_myEntity->getScene()->removeListener(MsgId::DAMAGE_TRAIN, this);
}

void TrainHP_c::start() 
{

	_myEntity->getScene()->addListiner(MsgId::DAMAGE_TRAIN, this);
	GraphicsManager::getInstance()->getGUI()->createTextTrainHp(Ogre::Vector4(0.6, 0.05, 0.1, 0.1),
		"Train hp: " + std::to_string(life_), GUIndilla::POSITION_TYPE::PT_REL, 25, 0.5, 0.7, 0.5);
} 
void TrainHP_c::listen(Msg_Base * msg)
{
	switch (msg->id) 
	{
	case MsgId::DAMAGE_TRAIN:
	{
		
		Msg::DamageTrain* p = static_cast<Msg::DamageTrain*>(msg);
		life_ -= p->dmg_;
		float r, g, b;
		if (life_ <= 50) {
			r = 1.0;
			g = 0.7;
			b = 0.0;
		}
		else {
			r = 0.5;
			g = 0.7;
			b = 0.5;
		}
		if (life_ <= 25) {
			r = 1.0;
			g = 0.0;
			b = 0.0;
		}
		GraphicsManager::getInstance()->getGUI()->removeContainer(GraphicsManager::getInstance()->
			getGUI()->getOverlaytrainhpContainer());
		GraphicsManager::getInstance()->getGUI()->createTextTrainHp(Ogre::Vector4(0.6, 0.05, 0.1, 0.1),
			"Train hp: " + std::to_string(life_), GUIndilla::POSITION_TYPE::PT_REL, 25, r, g, b);

		if (life_ <= 0) {
			//resetear nivel
			
			Game::getInstance()->gameOver();
			_myEntity->getScene()->endGame();
			
		}
	}
		break;
	default:
		break;
	}
}
