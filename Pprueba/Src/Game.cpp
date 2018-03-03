#include "Game.h"

Game::Game() :
	gameManager(this), inputManager(this),
	dataManager(this), graphicsManager(this),
	physicsManager(this)
{
}

Game::~Game()
{

}

bool Game::start()
{
	return true;
}

bool Game::stop()
{
	return false;
}

void Game::run()
{

}
GameManager Game::getGameManager() const
{
	return gameManager;
}

InputManager Game::getInputManager() const
{
	return inputManager;
}

GraphicsManager Game::getGraphicsManager() const
{
	return graphicsManager;
}

DataManager Game::getDataManager() const
{
	return dataManager;
}

PhysicsManager Game::getPhysicsManager() const
{
	return physicsManager;
}

