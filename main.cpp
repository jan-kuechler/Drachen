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
		using boost::get_error_info;

		std::ofstream out("crash.log");
		out << "Fatal error detected";

		if (std::string const *desc = get_error_info<ErrorInfo::Desc>(ex)) {
			out << ": " << *desc << "\n";
		}
		else {
			out << "\n";
		}

		if (std::string const *note = get_error_info<ErrorInfo::Note>(ex)) {
			out << *note << "\n";
		}

		if (get_error_info<ErrorInfo::Loading>(ex)) {
			if (std::string const* fileName = get_error_info<boost::errinfo_file_name>(ex)) {
				out << "While loading '" << *fileName << "'\n";
			}
			else {
				out << "While loading an unknown file\n";
			}
		}

		out << "\nFull diagnostic information:\n";
		out << diagnostic_information(ex);
	}
//#endif
    
	return 0;
}
