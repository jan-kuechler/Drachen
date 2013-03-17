#include "pch.h"
#include "Tower.h"
#include "Utility.h"

#include "ArrowTower.h"

/*static*/ std::unique_ptr<Tower> Tower::CreateTower(const TowerSettings* settings, std::vector<std::shared_ptr<Enemy>>* enemies, std::vector<Projectile>* projectiles)
{
	const std::string& type = settings->type;

	if (type == "archer")
		return std::unique_ptr<Tower>(new ArrowTower(settings, enemies, projectiles));
	else
		throw GameError() << ErrorInfo::Note("Unknown tower type '" + type + "'");
}

Tower::Tower(const TowerSettings* settings, std::vector<std::shared_ptr<Enemy>>* enemies, std::vector<Projectile>* projectiles)
: settings(settings), enemies(enemies), projectiles(projectiles), stage(0)
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

void Tower::ApplyStage()
{
	assert(stage < settings->stage.size());

	cooldown = settings->stage[stage].cooldown;
	range = settings->stage[stage].range;

	Image* img = settings->stage[stage].image;
	SetImage(*img);
	SetSize(img->GetWidth(), img->GetHeight());
	SetCenter(GetWidth() / 2.f, GetHeight() - GetWidth() / 3.f);
}

void Tower::Attack()
{
	for (auto it = enemies->begin(); it != enemies->end(); ++it) {
		if (!(*it)->IsIrrelevant() && norm((*it)->GetPosition() - GetPosition()) <= range) {
			Projectile p(*it);
			p.SetImage(*settings->stage[stage].projectile);
			p.SetPosition(GetPosition());
			projectiles->push_back(p);
			break;
		}
	}
}
