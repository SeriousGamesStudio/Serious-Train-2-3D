#ifndef _H_GRAPHICSMANAGER_H_
#define _H_GRAPHICSMANAGER_H_


class Game;

class GraphicsManager
{
public:
	GraphicsManager(Game* game);
	~GraphicsManager();

	void update();
private:
};

#endif //!_H_GRAPHICSMANAGER_H_

