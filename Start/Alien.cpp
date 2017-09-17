#include "stdafx.h"
#include "SDL.h"
#include "Alien.h"
#include "GameManager.h"
#include "math.h"
#include "SDL_image.h"
#include "sound.h"



Alien::Alien(GameManager* pMan) : Actor(pMan)
{
	reset();

	// add code to initialize variables in here
	sShip = NULL;
	sExplosion = NULL;
	shipSpeed = 150;
	m_posX = 1500;
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
	speed = 2;
	dead = false;
	alivePlayState = ALIVE;

}


Alien::~Alien() {}

int Alien::reset()
{
	//   add code to reset rocket to start position 
	rShip.x = 100;
	rShip.y = 100;

	rest();

	return 1;

}

int Alien::rest()
{
	//  add code to set rocket velocity and angle to 0	
	velocityX = 0;
	velocityY = 0;
	shipAngle = 0;
	return 1;

}

void Alien::Load()
{
	//  add code to load rocket image
	sShip = SDL_LoadBMP("Spirit.bmp");
	tExplosion = IMG_LoadTexture(m_pGM->getRenderer(), "explosion.png");
	SDL_SetColorKey(sShip, SDL_TRUE, 0x0000ff00);
	tShip = SDL_CreateTextureFromSurface(m_pGM->getRenderer(), sShip);

	engineIndex = LoadSound("explosion.wav", m_pGM);
	//rocketThrustSound = LoadSound("rocketMoving.wav", m_pGM);



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


void Alien::Draw()
{
	//if (!m_Visible) return;

	switch (alivePlayState)
	{
	case ALIVE: // play
		//if (!dead){
		SDL_RenderCopyEx(m_pGM->getRenderer(), tShip, NULL, &rShip, shipAngle, &shipCenter, SDL_FLIP_NONE);
		//}
		//SDL_RenderCopy(m_pGM->getRenderer(), tShip, &rShip, NULL);

		break;
		
	case DEAD: // explosion start
	    flash();

		break;

	case DEADER: // exploding
		explode();


		break;
/*
	case NOTHING: // nothing
		stopMoving();
		reset();

		break;
		*/
	}

}

void Alien::Update()
{
	//calculateGravity();
	//calculateVelocity();
	//shipBoundaries();
	m_posX = m_posX + velocityX *m_pGM->getElapsedTime();
	m_posY = m_posY + velocityY *m_pGM->getElapsedTime();
	rShip.x = m_posX + m_offX;
	rShip.y = m_posY + m_offY;
}


void Alien::IncAngle()
{
	shipAngle += turnSpeed;
}

void Alien::DecAngle()
{
	shipAngle -= turnSpeed;
}

void Alien::Thrust()
{
	//accelY = 0;
	//accelX = 0;

	accelY = sin(pi * (shipAngle / 180) + -(pi * 90 / 180)) * shipSpeed;
	accelX = cos(pi * (shipAngle / 180) + -(pi * 90 / 180)) * shipSpeed;


}


int Alien::safeLanding()
{
	// return true if slow enough and good enough angle
	if ((accelY < 100) && ((int)shipAngle % 360 == 0)) {
		return 1;
	}
	else
	{
		return 0;
	}
}
void Alien::calculateVelocity() {
	velocityX += (accelX * m_pGM->getElapsedTime());
	velocityY += (accelY * m_pGM->getElapsedTime());
}
void Alien::calculateGravity() {
	accelY += gravity;

}
void Alien::resetAcceleration() {
	accelY = 0;
	accelX = 0;
}
void Alien::shipBoundaries() {
	if (m_posY < 0) {
		m_posY = 0 + 1;
		accelY = 0;
	}
	if (m_posY > m_pGM->getHeight() - rShip.h) {
		m_posY = (m_pGM->getHeight() - rShip.h) - 1;
		stopMoving();
		gravity = 10;

		//m_pGM->setGameState(FLASH);
	}
	if (m_posX + rShip.w < 0) {
		//m_posX += m_pGM->getWidth();
		m_posX = 0 + 2;

		stopMoving();
		gravity = 10;
	}

	if ((m_posX - rShip.w) > m_pGM->getWidth()) {
		m_posX = m_pGM->getWidth() - 32;
		stopMoving();
		gravity = 10;
	}
}
SDL_Point Alien::returnCenter() {
	return shipCenter;
}
void Alien::calculateScreenPos() {
	rocketScreenPosX = (m_posX + m_offX);
	rocketScreenPosY = (m_posX + m_offY);
}
void Alien::flash() {
	printf("Flashing");
	destinRect.w = 64;
	destinRect.h = 64;
	destinRect.x = rShip.x;
	destinRect.y = rShip.y;
	explosionTime = m_pGM->getCurrTime();

	SDL_RenderCopyEx(m_pGM->getRenderer(), tExplosion, &rExplosion, &destinRect, 0, &shipCenter, SDL_FLIP_NONE);
	alivePlayState = DEADER;

	PlaySound(engineIndex);

}
void Alien::explode() {
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
	if (currentFrame > totalSprites) {
		//m_pGM->setGameState(NOTHING);
	}


}
void Alien::stopMoving() {
	velocityX = 0;
	velocityY = 0;
	gravity = 0;
	accelY = 0;
	accelX = 0;
	shipSpeed = 0;
}
void Alien::spawn(){

}
void Alien::followRocket(float tarX, float tarY) {
	float x = tarX - m_posX;
	float y = tarY - m_posY;
	x = (x / sqrt(x*x + y*y));
	y = (y / sqrt(x*x + y*y));
	m_posX += x * speed;
	m_posY += y * speed;
}
SDL_Rect Alien::getRect(){
	return rShip;
}
void Alien::blowUp(){
	alivePlayState = DEAD;
	//flash();
	setDead(true);
	stopMoving();
}
void Alien::setDead(bool inBool){
	dead = inBool;
}
bool Alien::getDead(){
	return dead;
}