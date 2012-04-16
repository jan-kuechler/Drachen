#ifndef TREASURE_H
#define TREASURE_H

#include "AnimSprite.h"

class Treasure : public AnimSprite
{

public:
	bool Available() const
	{
		return true;
	}
};

#endif //TREASURE_H
