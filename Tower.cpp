#include "pch.h"
#include "Tower.h"
#include "Utility.h"

Image Tower::projectileImg;
bool Tower::imgLoaded = false;

Tower::Tower(std::vector<std::shared_ptr<Enemy>>* enemies, std::vector<Projectile>* projectiles)
: enemies(enemies), projectiles(projectiles), range(4.0f), cooldown(1.0f)
{ 
	if (!imgLoaded) {
		LoadFromFile(projectileImg, "data/models/arrow.png");
		imgLoaded = true;
	}
}

void Tower::Update(float elapsed)
{
	if (cooldown > 0)
		cooldown -= elapsed;

	if (cooldown <= 0) {
		for (auto it = enemies->begin(); it != enemies->end(); ++it) {
			if (!(*it)->IsIrrelevant() && norm((*it)->GetPosition() - GetPosition()) <= range) {
				Projectile p(*it);
				p.SetImage(projectileImg);
				p.SetPosition(GetPosition());
				projectiles->push_back(p);
				cooldown = 1.0f;
				break;
			}
		}
	}

	AnimSprite::Update(elapsed);
}
