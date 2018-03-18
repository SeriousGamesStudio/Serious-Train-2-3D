#include "SceneManager.h"
#include "Game.h"


SceneManager::SceneManager(Game* game_) :
	game(game_)
{
	
	
}

SceneManager::~SceneManager()
{
	while (!scenes.empty())
	{
		scenes.pop();
	}
}

void SceneManager::tick()
{
	if (!scenes.empty())
		scenes.top().tick();
}

Scene& SceneManager::currentScene()
{
	return scenes.top();
}

void SceneManager::pushScene(Scene& newScene)
{
	scenes.push(newScene);
}

void SceneManager::popScene()
{
	if (!scenes.empty())
		scenes.pop();
}

void SceneManager::changeScene(Scene& newScene)
{
	if (!scenes.empty())
		scenes.pop();
	scenes.push(newScene);
}
