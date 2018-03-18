#include "SceneManager.h"
#include "Game.h"


m_SceneManager::m_SceneManager(Game* game_) :
	game(game_)
{
	
	
}

m_SceneManager::~m_SceneManager()
{
	while (!scenes.empty())
	{
		scenes.pop();
	}
}

void m_SceneManager::tick()
{
	if (!scenes.empty())
		scenes.top().tick();
}

Scene& m_SceneManager::currentScene()
{
	return scenes.top();
}

void m_SceneManager::pushScene(Scene& newScene)
{
	scenes.push(newScene);
}

void m_SceneManager::popScene()
{
	if (!scenes.empty())
		scenes.pop();
}

void m_SceneManager::changeScene(Scene& newScene)
{
	if (!scenes.empty())
		scenes.pop();
	scenes.push(newScene);
}
