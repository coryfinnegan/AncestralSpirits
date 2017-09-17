#include "stdafx.h"
#include "SDL.h"
#include "Pad.h"
#include "GameManager.h"


Pad::Pad(GameManager* pMan, float xMin, float xMax, float y): Actor (pMan)
{
	rPad.h = 64;
	rPad.w = 64;
	rPad.x = m_pGM->getWidth() / 2;
	rPad.y = m_pGM->getHeight() - rPad.h;
}


void Pad::Load()
{  // nothing to load
  
}


Pad::~Pad(){}

void Pad::Draw()
{
	SDL_SetRenderDrawColor(m_pGM->getRenderer(), 255, 0, 255, 255);
	SDL_RenderFillRect(m_pGM->getRenderer(), &rPad);
       	
}

void Pad::Update()
{  // do nothing
 
}
float Pad::returnXPos(){
	return rPad.x;
}
float Pad::returnWidth(){
	return rPad.w;
}
	
