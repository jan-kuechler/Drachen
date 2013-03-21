#ifndef STATE_H
#define STATE_H

enum State
{
	ST_MAIN_MENU, ST_GAME, ST_OPTIONS_MENU, ST_WIN, ST_LOOSE, ST_QUIT,
};

class StateDef
{
public:
	virtual void Reset() = 0;
	virtual void Run() = 0;
	virtual bool IsRunning() = 0;
	virtual State GetNextState() = 0;
};

#endif //STATE_H
