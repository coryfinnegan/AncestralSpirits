#pragma once

class Vector
{
public:
	Vector();
	~Vector();
	float getX();
	float getY();
	void setX(float);
	void setY(float);
private:
	float x;
	float y;
};

