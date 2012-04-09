#ifndef RECTANGLE_H
#define RECTANGLE_H

namespace sfext 
{

class Rectangle : public Shape
{
	float width, height;

public:
	Rectangle(float width, float height);

	void SetWidth(float w)
	{
		width = w;
		UpdatePoints();
	}

	void SetHeight(float h)
	{
		height = h;
		UpdatePoints();
	}

	float GetWidth() const
	{
		return width;
	}

	float GetHeight() const
	{
		return height;
	}

private:
	void UpdatePoints();
};

}

#endif //RECTANGLE_H
