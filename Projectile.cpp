#include "pch.h"
#include "Projectile.h"
#include "Utility.h"

static const float HIT_DISTANCE = 10.f;

static const Vector2f LEFT(-1.0f, 0.0f);

Projectile::Projectile(const AnimSprite* target)
: speed(100.f), target(target), hit(false)
{ }

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
		return;
	}

	Vector2f prevPos = GetPosition();
	Move(dir * (speed / r) * elapsed);

	auto way = GetPosition() - prevPos;
	way /= norm(way);
	float angle = acosf(dot(LEFT, way));

	if (way.y < 0)
		angle = -angle;

	SetRotation(angle * 180/PI);
}
