#include "GameManager.h"
#include "Game.h"


GameManager::GameManager(Game* game_) :
	game(game_)
{

}

GameManager::~GameManager()
{
	while (!scenes.empty())
	{
		scenes.pop();
	}
}

void GameManager::tick()
{
	if (!scenes.empty())
		scenes.top().tick();

}

Scene& GameManager::currentScene()
{
	return scenes.top();
}

void GameManager::pushScene(Scene& newScene)
{
	scenes.push(newScene);
}

void GameManager::popScene()
{
	if (!scenes.empty())
		scenes.pop();
}

void GameManager::changeScene(Scene& newScene)
{
	if (!scenes.empty())
		scenes.pop();
	scenes.push(newScene);
}
