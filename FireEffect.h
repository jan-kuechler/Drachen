#ifndef FIRE_EFFECT_H
#define FIRE_EFFECT_H

class FireEffect : public sf::PostFX
{
	sf::Image *fire, *noise, *alpha;

	float frameTime;

	sf::Vector2f position;
	float width, height;

	float worldWidth, worldHeight;

public:
	FireEffect(float worldWidth, float worldHeight);

	void SetFireTexture(sf::Image *ft);
	void SetNoiseTexture(sf::Image *nt);
	void SetAlphaTexture(sf::Image *at);

	void SetPosition(sf::Vector2f pos);
	void SetPosition(float x, float y);

	Vector2f GetPosition() const
	{
		return position;
	}

	void SetWidth(float w);
	void SetHeight(float h);

	void Update(float elapsed);

	void SetDistortion(float scale = 0.25, float bias = 0.3);

private:
	void SetInitialParameters();
};

#endif //FIRE_EFFECT_H
