#include "stdafx.h"
#include "SDL.h"
#include "SDL_image.h"
#include "MissileManager.h"
#include "GameManager.h"
#include "Alien.h"




MissileManager::MissileManager(GameManager* pMan) : Actor(pMan)
{
	missileSurface = NULL;
	missileTexture = NULL;
}


MissileManager::~MissileManager()
{
}

void MissileManager::makeMissile(SDL_Rect missileMakerPosition, float velocityX, float velocityY){
	
	SDL_Rect missileRect;
	SDL_Point missileVelocity;
	
	missileRect.x = missileMakerPosition.x;
	missileRect.y = missileMakerPosition.y;
	missileVelocity.x = velocityX;
	missileVelocity.y = velocityY;

	missileRect.x = missileMakerPosition.x;
	missileRect.y = missileMakerPosition.y;
	missileRect.w = 26;
	missileRect.h = 17;
	missileRects.push_back(missileRect);
	missilePositions.push_back(missileVelocity);
	missileAliveValues.push_back(true);
}
void MissileManager::Draw(){

	SDL_SetRenderDrawColor(m_pGM->getRenderer(), 255, 0, 255, 255);
	for (int i = 0; i < missileAliveValues.size(); i++){
		if (missileAliveValues[i]){
			//SDL_RenderCopy(m_pGM->getRenderer(), missileTexture, &missileRects[i], &missileRects[i]);
			SDL_RenderCopyEx(m_pGM->getRenderer(), missileTexture, NULL, &missileRects[i], NULL, &missilePositions[i], SDL_FLIP_NONE);
		}
	}
}
void MissileManager::Load(){
	missileSurface = SDL_LoadBMP("Bullet.bmp");
	SDL_SetColorKey(missileSurface, SDL_TRUE, 0x0000ff00);
	missileTexture = SDL_CreateTextureFromSurface(m_pGM->getRenderer(), missileSurface);
}
void MissileManager::Update(){
	for (int i = 0; i < missileAliveValues.size(); i++){
		if (missileAliveValues[i]){
			missileRects[i].x += missilePositions[i].x;
			missileRects[i].y += missilePositions[i].y;
		}
	}
	//
}
void MissileManager::detectMissileCollision(Alien &alien){
	SDL_Point alienCenter = alien.getCenter();
	SDL_Rect alienRect = alien.getRect();
	for (int i = 0; i < missileRects.size(); i++){

		float distanceBetween = sqrt(((missileRects[i].x) - (alienCenter.x + alienRect.x)) * ((missileRects[i].x) - (alienCenter.x + alienRect.x)) + (((missileRects[i].y) - (alienCenter.y + alienRect.y)) *((missileRects[i].y) - (alienCenter.y + alienRect.y))));
		if (distanceBetween < 50 && missileAliveValues[i]){
			missileAliveValues[i] = false;
			alien.blowUp();
		}
		printf(" Distance %f \n", distanceBetween);
	}
	
}