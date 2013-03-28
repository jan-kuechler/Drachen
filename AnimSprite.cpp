#include "pch.h"
#include "AnimSprite.h"

AnimSprite::AnimSprite()
: width(0), height(0), offset(0), frames(1), frameTime(0), curTime(0), direction(Up)
{ }

void AnimSprite::Update(float elapsed)
{
	size_t frame = 0;

	if (frameTime == 0)
		return;

	curTime += elapsed;
	while (curTime > frames * frameTime) {
		curTime -= frames * frameTime;
	}

	frame = static_cast<size_t>(curTime / frameTime);

	size_t xoffs = frame * width + (frame + 1) * offset;
	size_t yoffs = static_cast<int>(direction) * height + (static_cast<int>(direction) + 1) * offset;

	IntRect subrect(xoffs, yoffs, xoffs + width, yoffs + height);
	SetSubRect(subrect);
}
