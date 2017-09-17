
#include "stdafx.h"
#include "SDL.h"

#include "Actor.h"

// no definitions needed for draw, load, or update - they'll be in derived classes

Actor::Actor(){}

Actor::Actor(GameManager* pMan)
{  // base constructor
// set pointer to game manager, and init position to 0,0

	m_pGM = pMan; 

	m_posX = 0.0;
	m_posY = 0.0;
	
	m_offX = 0.0;	
	m_offY = 0.0;

}


Actor::~Actor(){}

SDL_Point Actor::getCenter(){
	return shipCenter;
}
SDL_Rect Actor::getRect(){
	return rShip;
}