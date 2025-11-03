#include <iostream>
#include "UserGame.h"
#include "GameLoad.h"


int main(int argc, char** argv)
{

	bool isLoad = argc > 1 && std::string(argv[1]) == "-load";
	bool isSave = argc > 1 && std::string(argv[1]) == "-save";
	bool isSilent = isLoad && argc > 2 && std::string(argv[2]) == "-silent";
	Game* g;

	if (isLoad)
		g = new GameLoad(isSilent);
	else
		g = new UserGame(isSave);

	g->start();

	delete g;
}
