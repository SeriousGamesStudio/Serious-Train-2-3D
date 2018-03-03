#ifndef _H_GAME_H_
#define _H_GAME_H_
#pragma once
class GameManager;
class DataManager;
class InputManager;
class GraphicsManager;
class PhysicsManager;


class Game
{
public:
	Game();
	~Game();

	bool start();
	bool stop();

	inline GameManager getGameManager() const { return gameManager; }
	inline InputManager getInputManager() const { return inputManager; }
	inline GraphicsManager getGraphicsManager() const { return graphicsManager; }
	inline DataManager getDataManager() const { return dataManager; }


private:
	void run();

private:
	GameManager gameManager;
	InputManager  inputManager;
	GraphicsManager graphicsManager;
	PhysicsManager physicsManager;
	DataManager dataManager;

};
#endif //!_H_GAME_H_