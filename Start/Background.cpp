
#include "stdafx.h"
#include "SDL.h"
#include "Background.h"
#include "GameManager.h"


Background::Background(GameManager* pMan): Actor (pMan)
{

}


void Background::Load()
{
	// Change this to your background
	sBackground = SDL_LoadBMP( "background.bmp" );
	tBackground = SDL_CreateTextureFromSurface(m_pGM->getRenderer(), sBackground);
	SDL_FreeSurface(sBackground);  // we got the texture now -> free surface

	rBackground.x = 0;
	rBackground.y = 0;
	rBackground.w = 2048;
	rBackground.h = 1160;

}

Background::~Background(){}

void Background::Draw()
{
		SDL_RenderCopy(m_pGM->getRenderer(), tBackground, NULL, &rBackground);   	
}

void Background::Update()
{	  
	rBackground.x = m_posX + m_offX;
	rBackground.y = m_posY + m_offY;

}

	
