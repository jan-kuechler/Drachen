#include "pch.h"
#include "Game.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int main(int argc, char **argv)
{
	try {
		GlobalStatus status;
		status.level = "test";

		RenderWindow window(sf::VideoMode(800, 600, 32), "Drachen");
		window.SetFramerateLimit(100);

		Game game(window, status);

		State state = ST_GAME;
		bool newState = true;

		while (window.IsOpened()) {
			switch (state) {
			case ST_GAME:
				if (newState) {
					game.Reset();
					newState = false;
				}
				game.Run();
				if (!game.IsRunning()) {
					state = game.GetNextState();
					newState = true;
				}
				break;

			case ST_QUIT:
				window.Close();
				break;
			}
		}
	}
	catch (std::runtime_error err) {
		MessageBoxA(NULL, err.what(), "Fatal Error", MB_ICONERROR | MB_OK);
	}

	return 0;
}
