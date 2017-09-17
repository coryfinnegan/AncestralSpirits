#include "stdafx.h"
#include "Vector.h"


Vector::Vector()
{
	x = 0;
	y = 0;
}


Vector::~Vector()
{
}
float Vector::getX(){
	return x;
}
float Vector::getY(){
	return y;
}
void Vector::setX(float inValue){
	x = inValue
}
void Vector::setY(float inValue){
	y = inValue;
}
