#include "stdafx.h"
#include "SDL.h"
#include "Rocket.h"
#include "GameManager.h"
#include "math.h"
#include "SDL_image.h"
#include "sound.h"


Rocket::Rocket(GameManager* pMan)  : Actor (pMan)
{
	reset();

	// add code to initialize variables in here
	sShip = NULL;
	sShipThrust = NULL;
	sExplosion = NULL;
	shipSurfaceBack = NULL;
	shipSurfaceSide = NULL;
	shipForwardSurface = NULL;
	shipSurfaceRight = NULL;
	shipSpeed = 5;
	m_posX = 300;
	m_posY = 300;
	velocityX = 0;
	velocityY = 0;
	accelX = 0;
	accelY = 0;
	gravity = 10;
	shipAngle = 0;
	turnSpeed = 5;
	pi = 3.14159265358979323846;
	currentRow = 0;
	currentColumn = 0;
	playSound = false;
	shootDirectionX = 0;
	shootDirectionY = 0;
	shootBool = true;
	bulletSpeed = 10;
	direction = DOWN;
    
	

}


Rocket::~Rocket(){}

int Rocket::reset()
{
	//   add code to reset rocket to start position 
	rShip.x = 100;
	rShip.y = 100;
	
	rest();

	return 1;

}

int Rocket::rest()
{
	//  add code to set rocket velocity and angle to 0	
	velocityX = 0;
	velocityY = 0;
	shipAngle = 0;
	gravity = 10;
	return 1;

}

void Rocket::Load()
{
	//  add code to load rocket image
	sShip = SDL_LoadBMP("Eskimo.bmp");
	shipForwardSurface = SDL_LoadBMP("Eskimo.bmp");
	sShipThrust = SDL_LoadBMP("spaceShipBlasting.bmp");
	shipSurfaceBack = SDL_LoadBMP("EskimoBack.bmp");
	shipSurfaceSide = SDL_LoadBMP("EskimoSide.bmp");
	shipSurfaceRight = SDL_LoadBMP("EskimoSideRight.bmp");
	
	SDL_SetColorKey(sShip, SDL_TRUE, 0x0000ff00);
	SDL_SetColorKey(sShipThrust, SDL_TRUE, 0x0000ff00);
	SDL_SetColorKey(shipSurfaceBack, SDL_TRUE, 0x0000ff00);
	SDL_SetColorKey(shipSurfaceSide, SDL_TRUE, 0x0000ff00);
	SDL_SetColorKey(shipForwardSurface, SDL_TRUE, 0x0000ff00);
	SDL_SetColorKey(shipSurfaceRight, SDL_TRUE, 0x0000ff00);


	tExplosion = IMG_LoadTexture(m_pGM->getRenderer(), "explosion.png");
	tShip = SDL_CreateTextureFromSurface(m_pGM->getRenderer(), sShip);
	shipTextureBack = SDL_CreateTextureFromSurface(m_pGM->getRenderer(), shipSurfaceBack);
	shipTextureSide = SDL_CreateTextureFromSurface(m_pGM->getRenderer(), shipSurfaceSide);
	tShipThrust = SDL_CreateTextureFromSurface(m_pGM->getRenderer(), sShipThrust);
	shipForwardTexture = SDL_CreateTextureFromSurface(m_pGM->getRenderer(), shipForwardSurface);
	shipTextureRight = SDL_CreateTextureFromSurface(m_pGM->getRenderer(), shipSurfaceRight);
	
	engineIndex = LoadSound("explosion.wav", m_pGM);
	rocketThrustSound = LoadSound("ambience_inside_fan.wav", m_pGM);

	rShip.x = 100;
	rShip.y = 100;
	rShip.w = 64;
	rShip.h = 64;
	destinRect = rShip;
	shipCenter.x = sShip->w / 2;
	shipCenter.y = sShip->h / 2;
	rExplosion.x = 0;
	rExplosion.y = 0;
	rExplosion.w = 64;
	rExplosion.h = 64;
	SDL_FreeSurface(sShip);
	reset();

}


void Rocket::Draw()
{
	//if (!m_Visible) return;
	

	gameStateType gState = m_pGM->getGameState();

	switch(gState)
	{
	case INPLAY: // play

		//rShip.x = (int) m_posX;
		//rShip.y = (int) m_posY;
		if (!thrusting){
			SDL_RenderCopyEx(m_pGM->getRenderer(), tShip, NULL, &rShip, shipAngle, &shipCenter, SDL_FLIP_NONE);
		}/*
		if (thrusting){
			SDL_RenderCopyEx(m_pGM->getRenderer(), tShipThrust, NULL, &rShip, shipAngle, &shipCenter, SDL_FLIP_NONE);
		}
		*/

		break;

	case FLASH: // explosion start
		flash();

		break;
		
	case EXPLODING: // exploding
		explode();


		break;

	case NOTHING: // nothing
		stopMoving();
		reset();
		
		break;

	}

}

void Rocket::Update()
{
	//calculateGravity();
	calculateVelocity();
	shipBoundaries();
	setShootDirection();
	m_posX = m_posX + velocityX *m_pGM->getElapsedTime();
	m_posY = m_posY + velocityY *m_pGM->getElapsedTime();
	rShip.x = m_posX + m_offX;
	rShip.y = m_posY + m_offY;
}


void Rocket::IncAngle()
{
	shipAngle += turnSpeed;
}

void Rocket::DecAngle()
{
	shipAngle -= turnSpeed;
}

void Rocket::Thrust()
{
	//accelY = 0;
	//accelX = 0;

	accelY = sin(pi * (shipAngle / 180) + -(pi * 90 / 180)) * shipSpeed;
 	accelX = cos(pi * (shipAngle / 180) + -(pi * 90 / 180)) * shipSpeed;
	//PlaySound(rocketThrustSound);
	//playThrustSound();

}



void Rocket::calculateVelocity(){
	velocityX += (accelX * m_pGM->getElapsedTime());
	velocityY += (accelY * m_pGM->getElapsedTime());
}
void Rocket::calculateGravity(){
	accelY += gravity;

}
void Rocket::resetAcceleration(){
	accelY = 0;
	accelX = 0;
}
void Rocket::shipBoundaries(){
	if (m_posY < 0) {
		m_posY = 0 + 1;
		accelY = 0;
	}
	if (m_posY > m_pGM->getHeight() - rShip.h){
		m_posY = (m_pGM->getHeight() - rShip.h) - 1;
		stopMoving();
	
		
		//m_pGM->setGameState(FLASH);
	}
	if (m_posX + rShip.w < 0){
		//m_posX += m_pGM->getWidth();
		m_posX = 0 +2;

		stopMoving();
		gravity = 10;
	}

	if ((m_posX-rShip.w) > m_pGM->getWidth()){
		m_posX = m_pGM->getWidth() - 32;
		stopMoving();
		gravity = 10;
	}
}
SDL_Point Rocket::returnCenter(){
	return shipCenter;
}
void Rocket::calculateScreenPos(){
	rocketScreenPosX = (m_posX + m_offX);
	rocketScreenPosY = (m_posX + m_offY);
}
void Rocket::flash(){
	stopMoving();
	destinRect.w = 64;
	destinRect.h = 64;
	destinRect.x = rShip.x;
	destinRect.y = rShip.y;
	explosionTime = m_pGM->getCurrTime();
	
	SDL_RenderCopyEx(m_pGM->getRenderer(), tExplosion, &rExplosion, &destinRect, 0, &shipCenter, SDL_FLIP_NONE);
	m_pGM->setGameState(EXPLODING);
	PlaySound(engineIndex);

}
void Rocket::explode(){
	int imageHeight = 320;
	int imageWidth = 320;
	int imageSize = imageHeight * imageWidth;
	int totalRows = 5;
	int totalColumns = 5;

	int totalSprites = totalRows * totalColumns;
	int spriteWidth = imageWidth / totalColumns;
	int spriteHeight = imageHeight / totalRows;
	float lengthOfExplosion = 2 * CLOCKS_PER_SEC;
	float currentTime = m_pGM->getCurrTime();
	float elapsedTime = currentTime - explosionTime;
	int currentFrame = (int)(totalSprites * (elapsedTime / lengthOfExplosion));
	
	float upperLeftCornerOfCurrentFrameX = (currentFrame % totalRows) * spriteWidth;
	float upperLeftCornerOfCurrentFrameY = ((int)(currentFrame / totalColumns)) * spriteHeight;
	rExplosion.x = upperLeftCornerOfCurrentFrameX;
	rExplosion.y = upperLeftCornerOfCurrentFrameY;
	SDL_RenderCopyEx(m_pGM->getRenderer(), 
		tExplosion, 
		&rExplosion,
		&destinRect,
		0, 
		&shipCenter, 
		SDL_FLIP_NONE);
	if (currentFrame > totalSprites){
		m_pGM->setGameState(NOTHING);
	}


}
void Rocket::stopMoving(){
	velocityX = 0;
	velocityY = 0;
	gravity = 0;
	accelY = 0;
	accelX = 0;
}
void Rocket::playThrustSound(){
	if (playSound){
		PlaySound(rocketThrustSound);
		playSound = false;
	}
	PlaySound(rocketThrustSound);
	//StopSound(rocketThrustSound);
}
void Rocket::setThrustingBool(bool inValue){
	thrusting = inValue;
}
bool Rocket::getThrustingBool(){
	return thrusting;
}
void Rocket::setThrustingTexture(){

}
void Rocket::stopThrustingSound(){
	StopSound(rocketThrustSound);
	//playSound = true;
}
void Rocket::movePlayerHorizontally(float inValue){
	if (inValue < 0){
		direction = LEFT;
	}
	if (inValue > 0){
		direction = RIGHT;
	}
	m_posX += inValue * shipSpeed;
}
void Rocket::movePlayerVertically(float inValue){
	if (inValue < 0){
		direction = UP;
	}
	if (inValue > 0){
		direction = DOWN;
	}
	m_posY += inValue * shipSpeed;
}
SDL_Point Rocket::getCenter(){
	return shipCenter;
}
float Rocket::getVelocityX(){
	return velocityX;
}
float Rocket::getVelocityY(){
	return velocityY;
}
SDL_Rect Rocket::getRect(){
	return rShip;
}
void Rocket::setShootDirection(){
	switch (direction){
		case UP:
			shootDirectionX = 0;
			shootDirectionY = -1 * bulletSpeed;
			tShip = shipTextureBack;
			break;

		case DOWN:
			shootDirectionX = 0;
			shootDirectionY = 1 * bulletSpeed;
			tShip = shipForwardTexture;
			break;

		case LEFT:
			shootDirectionX = -1 * bulletSpeed;
			shootDirectionY = 0;
			tShip = shipTextureSide;
			break;
		case RIGHT:
			tShip = shipTextureRight;
			//SDL_RenderCopyEx(m_pGM->getRenderer(), shipTextureSide, &rShip, &destinRect, 0, &shipCenter, SDL_FLIP_VERTICAL);
			shootDirectionX = 1 * bulletSpeed;
			shootDirectionY = 0;
	}
}
void Rocket::setDirection(directionFacing inDirection){
	direction = inDirection;
}
float Rocket::getShootDirectionY(){
	return shootDirectionY;
}
float Rocket::getShootDirectionX(){
	return shootDirectionX;

}
bool Rocket::getShootBool(){
	return shootBool;
}
void Rocket::setShootBool(bool inBool){
	shootBool = inBool;
}
