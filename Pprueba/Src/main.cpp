#include "Game.h"
#include "TrashCollector.h"


int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks
	Game game;
	game.start();
	return 0;
}
