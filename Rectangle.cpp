#include "pch.h"
#include "Rectangle.h"

using namespace sfext;

Rectangle::Rectangle(float width, float height)
: width(width), height(height)
{
	AddPoint(0, 0);
	AddPoint(width, 0);
	AddPoint(width, height);
	AddPoint(0, height);
}

void Rectangle::UpdatePoints()
{
	SetPointPosition(1, width, 0.0f);
	SetPointPosition(2, width, height);
	SetPointPosition(3, 0.0f, height);
}

