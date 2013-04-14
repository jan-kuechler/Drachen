#ifndef STATE_H
#define STATE_H

enum State
{
	ST_MAIN_MENU, ST_GAME, ST_OPTIONS_MENU, ST_LEVEL_PICKER, ST_TEXT_DISPLAY, ST_WIN, ST_LOOSE, ST_QUIT,
};

class StateDef
{
public:
	virtual void Reset() = 0;
	virtual void Run() = 0;
	virtual bool IsRunning() = 0;
	virtual State GetNextState() = 0;
};

static std::ostream& operator << (std::ostream& out, State state)
{
	switch (state) {
	case ST_MAIN_MENU:
		return out << "Main Menu";
	case ST_GAME:
		return out << "Game";
	case ST_OPTIONS_MENU:
		return out << "Options Menu";
	case ST_LEVEL_PICKER:
		return out << "Level Picker";
	case ST_TEXT_DISPLAY:
		return out << "Text Display";
	case ST_WIN:
		return out << "Win";
	case ST_LOOSE:
		return out << "Loose";
	case ST_QUIT:
		return out << "Quit";
	}
	return out;
}

#endif //STATE_H
