#include "pch.h"
#include "Projectile.h"
#include "Utility.h"

static const float HIT_DISTANCE = 10.f;

static const Vector2f LEFT(-1.0f, 0.0f);

Projectile::Projectile(Enemy* target)
: speed(100.f), target(target), hit(false)
{
	assert(target);
	target->AddProjectile();
}

void Projectile::SetImage(const Image& img)
{
	AnimSprite::SetImage(img);
	SetCenter(img.GetWidth() / 2.0f, img.GetHeight() / 2.0f);
}

void Projectile::Update(float elapsed)
{
	AnimSprite::Update(elapsed);

	if (hit || !target)
		return;

	Vector2f dir = target->GetPosition() - GetPosition();
	float r = norm(dir);
	if (r < HIT_DISTANCE) {
		hit = true;
		target->ReleaseProjectile();
		return;
	}

	dir /= r;
	float angle = acosf(dot(LEFT, dir));
	if (dir.y < 0)
		angle = -angle;

	Move(dir * speed * elapsed);
	SetRotation(angle * 180/PI);
}
