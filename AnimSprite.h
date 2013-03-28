#ifndef ANIM_SPRITE_H
#define ANIM_SPRITE_H

class AnimSprite : public sf::Sprite
{
public:
	enum Direction {
		Up    = 0,
		Right = 1,
		Down  = 2,
		Left  = 3,
	};

private:

	size_t frames;
	size_t width, height;
	size_t offset;

	float frameTime;

	float curTime;

	Direction direction;

public:
	AnimSprite();

	void SetSize(size_t w, size_t h)
	{
		width = w;
		height = h;

		SetCenter(w / 2.0f, static_cast<float>(h));
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

	size_t GetWidth() const
	{
		return width;
	}

	size_t GetHeight() const
	{
		return height;
	}

	void SetDirection(Direction dir)
	{
		direction = dir;
	}
};

#endif //ANIM_SPRITE_H
