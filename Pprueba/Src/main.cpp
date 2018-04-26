#include "Game.h"
#include "TrashCollector.h"

#ifdef _DEBUG
int main(int argc, char *argv) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks
#else //RELEASE
int WINAPI WinMain(HINSTANCE hInstance,    // HANDLE TO AN INSTANCE.  This is the "handle" to YOUR PROGRAM ITSELF.  More in the GLOSSARY at the bottom.
	HINSTANCE hPrevInstance,// USELESS on modern windows (totally ignore hPrevInstance)
	LPSTR szCmdLine,        // Command line arguments.  Explained near BOTTOM of this file.
	int iCmdShow)          // Start window maximized, minimized, etc.
{
#endif 
	Game* game = Game::getInstance();
	delete game;
	return 0;
}
