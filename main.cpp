#include "pch.h"
#include "Game.h"
#include "Utility.h"

int main(int argc, char **argv)
{
//#ifndef _DEBUG
	try {
//#endif
		GlobalStatus status;
		status.level = "test";
		status.startLives = 6;

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
//#ifndef _DEBUG
	}
	catch (std::runtime_error err) {
		std::ofstream out("crash.log");
		out << err.what() << "\n";
	}
	catch (boost::exception& ex) {
		std::ofstream out("crash.log");
		out << "Fatal error detected\n";
		out << diagnostic_information(ex);
	}
//#endif
    
	return 0;
}
