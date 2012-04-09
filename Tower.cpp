#include "pch.h"
#include "Tower.h"
#include "Utility.h"

Color Tower::ColorInvalidPosition(255, 0, 0, 128);
Color Tower::ColorValidPosition(0, 255, 0, 128);
Color Tower::ColorPlaced(255, 255, 255);
Color Tower::ColorRangeCircle(64, 255, 64, 128);

Image Tower::projectileImg;
bool Tower::imgLoaded = false;

Tower::Tower(const Map* map, std::vector<Enemy>* enemies, std::vector<Projectile>* projectiles)
: map(map), enemies(enemies), projectiles(projectiles), placed(false), validPosition(false), range(4.0f * map->GetBlockSize()), cooldown(1.0f)
{ 
	SetColor(ColorInvalidPosition);

	rangeCircle = Shape::Circle(GetPosition(), range, ColorRangeCircle);

	if (!imgLoaded) {
		LoadFromFile(projectileImg, "data/models/arrow.png");
		imgLoaded = true;
	}
}
bool Tower::HandleEvent(Event& event)
{
	if (placed)
		return false;

	if (event.Type == Event::MouseMoved) {
		Vector2f pos(static_cast<float>(event.MouseMove.X), static_cast<float>(event.MouseMove.Y));
		Vector2i tpos = map->PostionToTowerPos(pos);

		if (map->MayPlaceTower(tpos)) {
			SetPosition(map->TowerPosToPosition(tpos));
			validPosition = true;
			SetColor(ColorValidPosition);
		}
		else {
			SetPosition(pos);
			validPosition = false;
			SetColor(ColorInvalidPosition);
		}
		rangeCircle.SetPosition(GetPosition());
	}
	else if (event.Type == Event::MouseButtonPressed && validPosition && event.MouseButton.Button == Mouse::Left) {
		placed = true;
		SetColor(ColorPlaced);
	}
	else {
		return false;
	}
	return true;
}

void Tower::DrawRangeCircle(RenderTarget& target)
{
	//if (!placed)
		target.Draw(rangeCircle);
}

void Tower::Update(float elapsed)
{
	if (cooldown > 0)
		cooldown -= elapsed;

	if (cooldown <= 0) {
		for (auto it = enemies->begin(); it != enemies->end(); ++it) {
			if (norm(it->GetPosition() - GetPosition()) <= range) {
				Projectile p(&(*it));
				//p.SetSize(projectileImg.GetHeight(), projectileImg.GetWidth());
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
