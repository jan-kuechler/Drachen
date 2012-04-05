#ifndef ANIM_SPRITE_H
#define ANIM_SPRITE_H

class AnimSprite : public sf::Sprite
{
	size_t frames;
	size_t width, height;
	size_t offset;

	float frameTime;

	float curTime;

public:
	AnimSprite();

	void SetSize(size_t w, size_t h)
	{
		width = w;
		height = h;
	}

	void SetOffset(size_t off)
	{
		offset = off;
	}

	void SetNumFrames(size_t n)
	{
		frames = n;
	}

	void SetFrameTime(float ft)
	{
		frameTime = ft;
	}

	void Update(float elapsed);
};

#endif //ANIM_SPRITE_H