#include "Game.h"
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
	Game *Juego;
	Juego = new Game(800, 600, "Physics kit");
	Juego->Loop();

	return 0;
}