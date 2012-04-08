#include "pch.h"
#include "AnimSprite.h"

AnimSprite::AnimSprite()
: width(0), height(0), offset(0), frames(1), frameTime(0), curTime(0)
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

	IntRect subrect(frame * width + (frame + 1) * offset, offset, (frame + 1) * width + (frame + 1) * offset, height);
	SetSubRect(subrect);
}
