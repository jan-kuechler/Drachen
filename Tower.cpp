#include "pch.h"
#include "Tower.h"
#include "Utility.h"

#include "ArrowTower.h"

static Color RangeCircleColor(255, 201, 0, 64);
static Color RangeCircleOutline(255, 201, 0, 128);

/*static*/ std::unique_ptr<Tower> Tower::CreateTower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles, bool highRange)
{
	const std::string& type = settings->type;

	if (type == "archer")
		return std::unique_ptr<Tower>(new ArrowTower(settings, enemies, projectiles, highRange));
	else
		throw GameError() << ErrorInfo::Note("Unknown tower type '" + type + "'");
}

Tower::Tower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles, bool highRange)
: settings(settings), enemies(enemies), projectiles(projectiles), hasHighRange(highRange), stage(0)
{
	ApplyStage();
}

void Tower::Update(float elapsed)
{
	if (cooldownTimer > 0)
		cooldownTimer -= elapsed;

	if (cooldownTimer <= 0) {
		Attack();
		cooldownTimer = cooldown;
	}

	AnimSprite::Update(elapsed);
}

void Tower::Upgrade()
{
	stage++;
	ApplyStage();
	if (cooldownTimer > cooldown)
		cooldownTimer = cooldown;
}

void Tower::ApplyStage()
{
	assert(stage < settings->stage.size());

	cooldown = settings->stage[stage].cooldown;
	range    = settings->stage[stage].range;
	attacks  = settings->stage[stage].attacks;
	power    = settings->stage[stage].power;

	if (hasHighRange)
		range *= HIGH_RANGE_FACTOR;

	Image* img = settings->stage[stage].image;
	SetImage(*img);
	SetSize(img->GetWidth(), img->GetHeight());
	SetSubRect(IntRect(0, 0, GetWidth(), GetHeight())); // reset the subrect incase the image size has changed
	SetCenter(settings->stage[stage].center);

	rangeCircle = Shape::Circle(GetPosition(), range, RangeCircleColor, 2.5f, RangeCircleOutline);
}

void Tower::Attack()
{
	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		if (!(*it)->IsIrrelevant() && norm((*it)->GetPosition() - GetPosition()) <= range) {
			Projectile p(*it);
			p.SetImage(*settings->stage[stage].projectile);
			p.SetPosition(GetPosition());
			projectiles.push_back(p);
			break;
		}
	}
}
