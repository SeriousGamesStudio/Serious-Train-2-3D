#include "GameManager_c.h"
#include "Game.h"
#include "Components.h"
GameManager_c::GameManager_c(): Component(ComponentType::GAMEMANAGER)
{
	
}

GameManager_c::~GameManager_c()
{
}

void GameManager_c::start()
{
	//grM_ = GraphicsManager::getInstance();
	_myEntity->getScene()->addListiner(MsgId::TOGGLE_MOUSE, this);
}

void GameManager_c::update()
{
}

void GameManager_c::listen(Msg_Base * msg)
{
	switch (msg->id)
	{
	case MsgId::TOGGLE_MOUSE:
	{
		Msg::ToggleMouse* t = static_cast<Msg::ToggleMouse*>(msg);
		active_ = GraphicsManager::getInstance()->getGUI()->getMousePointer()->isActive();
		active_ = !active_;
		GraphicsManager::getInstance()->toggleMouse(active_);
		break;
	}
	default:
		break;
	}
}

void GameManager_c::closeGame()
{
	Game::getInstance()->stop();
}
