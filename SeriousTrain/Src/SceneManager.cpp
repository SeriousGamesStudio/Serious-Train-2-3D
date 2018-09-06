#include "SceneManager.h"
#include "Game.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager()
{	
}

SceneManager::~SceneManager()
{
	while (!scenes.empty())
	{
		delete scenes.top();
		scenes.pop();
	}
	instance = nullptr;
}

SceneManager * SceneManager::getInstance()
{
	if (!instance) {
		instance = new SceneManager();
	}
	return instance;
}

void SceneManager::tick()
{
	if (!scenes.empty())
		scenes.top()->tick();
}

Scene* SceneManager::currentScene()
{
	return scenes.empty() ? nullptr : scenes.top();
}

void SceneManager::pushScene(Scene* newScene)
{
	scenes.push(newScene);
}

void SceneManager::popScene()
{
	if (!scenes.empty())
		scenes.pop();
}

void SceneManager::changeScene(Scene* newScene)
{
	if (!scenes.empty())
		scenes.pop();
	scenes.push(newScene);
}
