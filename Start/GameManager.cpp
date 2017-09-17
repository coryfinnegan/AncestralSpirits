#include "stdafx.h"
#include "SDL.h"
#include "GameManager.h"
#include "time.h"
#include "Rocket.h"
#include "Background.h"
#include "Pad.h"
#include "sound.h"
#include "Alien.h"
#include "MissileManager.h"
#include <stdio.h>


GameManager::GameManager(){}
GameManager::~GameManager(){}


int GameManager::Initialize(Actor* Actors[])
{  // create actors used in the game.  Init screen size, time, and SDL
	ActorsPtr = Actors;
	nActors = 0;
	m_numPads = 0;

	// create background
	Actors[nActors++] = (Actor*) new Background(this);

	// create rocket
	m_pRocket = new Rocket(this);
	Actors[nActors++] = (Actor*)m_pRocket;

	// creaqte landing pads
	m_Pads[m_numPads++] = new Pad(this, 50.0, 100.0, 450.0);
	Actors[nActors++] = (Actor*)m_Pads[m_numPads - 1];

	m_Pads[m_numPads++] = new Pad(this, 400., 450.0, 400.0);
	Actors[nActors++] = (Actor*)m_Pads[m_numPads - 1];

	//create alien
	alien = new Alien(this);
	Actors[nActors++] = (Actor*)alien;

	//make missileManager
	missileManager = new MissileManager(this);
	Actors[nActors++] = (Actor*)missileManager;

	// init variables
	m_iFrame = 0;

	m_worldMaxX = 2048.;
	m_worldMaxY = 1160.;
	margin = 200.;
	yScreenSize = 768.;
	xScreenSize = 1024.;
	xOffset = 0;
	yOffset = 0;

	m_gameState = INPLAY;

	m_oTime = m_fTime = clock();
	m_elapsedTime = 0.0;

	initializeSDL();

	done = 0;

	return nActors;
}
void GameManager::updateOffsets(){
	float rocketX = m_pRocket->X();
	float rocketY = m_pRocket->Y();

	if ((m_worldMaxX + xOffset) < xScreenSize){
		xOffset = (m_worldMaxX - m_screenWidth);
	}

	if (rocketX > (xScreenSize - margin - xOffset)){
		xOffset = xScreenSize - margin - rocketX;


	}
	if (rocketY > (yScreenSize - margin - yOffset)){
		yOffset = yScreenSize - margin - rocketY;
	}

	if (rocketX < margin - xOffset){
		xOffset = margin - rocketX;
	}
	if (rocketY < margin - yOffset){
		yOffset = margin - rocketY;
	}
	if ((m_worldMaxX + xOffset) < xScreenSize){
		xOffset = -(m_worldMaxX - xScreenSize);
	}
	if ((m_worldMaxY + yOffset) < yScreenSize){
		yOffset = -(m_worldMaxY - yScreenSize);
	}
	if ((rocketX < margin)){
		xOffset = 0;
	}
	if (rocketY < margin){
		yOffset = 0;
	}
}

void GameManager::updateActors(){
	for (int i = 0; i < nActors; i++){
		ActorsPtr[i]->setOffX(xOffset);
		ActorsPtr[i]->setOffY(yOffset);
	
	}
}
int GameManager::gameOver()
{
	return done;
}
SDL_Renderer* GameManager::getRenderer()
{
	return renderer;
}
int GameManager::initializeSDL()
{
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return 1;

	// Create the window where we will draw.
	window = SDL_CreateWindow("G2342 Week 2",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1024, 768,
		0);
	
	// We must call SDL_CreateRenderer in order for draw calls to affect this window.
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	keystates = SDL_GetKeyboardState( NULL );


	SDL_AudioSpec explosionSound = setupAudioSpec();

	if (SDL_OpenAudio(&explosionSound, &obtained) < 0) {
		fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_PauseAudio(0);

	printf ("Starting...\n");

	return 1;
}
int GameManager::endGraphics()
{
	SDL_CloseAudio();
	//Quit SDL 
	SDL_Quit(); 


	return 1;
}
void GameManager::Update()
{  // get user input, update the game time, and do any global game update, i.e. check for a safe landing

	// get keyborad input
	
	// calc elapsed time, frame number
	m_currTime = clock();
	m_eTime = m_currTime - m_oTime;
	m_elapsedTime = (double) m_eTime / (double) CLOCKS_PER_SEC;
	m_oTime = m_currTime;
	m_iFrame++;

	if ((m_iFrame % 60) == 0)
	{
		float tpf = (float) ((m_currTime - m_fTime) / ((double) CLOCKS_PER_SEC * 60.0f));
		printf (" fps %f \n", 1.0f / tpf);
		m_fTime = m_currTime;
	}
	if (m_gameState == INPLAY){
		UserInput();
		detectCollision();
		updateOffsets();
		chasePlayer();
	}

	updateActors();

}
int GameManager::padIntersect(Pad* pPad, Actor* pRocket)
{  

	return 0;
	
}
void GameManager::Draw()
{ 
	SDL_RenderPresent(renderer);
}
void GameManager::UserInput()
{
	
	SDL_PumpEvents();
	m_pRocket->resetAcceleration();
	m_pRocket->setThrustingBool(false);
	m_pRocket->stopThrustingSound();
	
	if( keystates[ SDL_SCANCODE_LEFT ] || keystates[ SDL_SCANCODE_KP_4 ])
	{
		m_pRocket->setDirection(LEFT);
		m_pRocket->movePlayerHorizontally(-1);
		//m_pRocket->DecAngle();
	}
	if( keystates[ SDL_SCANCODE_RIGHT ] || keystates[ SDL_SCANCODE_KP_6 ])
	{
		//m_pRocket->IncAngle();
		m_pRocket->setDirection(RIGHT);
		m_pRocket->movePlayerHorizontally(1);
	}
	if( keystates[ SDL_SCANCODE_UP ] ) 
	{
		//m_pRocket->playThrustSound();
		//m_pRocket->Thrust();
		//m_pRocket->setThrustingBool(true);
		m_pRocket->setDirection(UP);
		m_pRocket->movePlayerVertically(-1);
		
	}
	if (keystates[SDL_SCANCODE_DOWN]){
		m_pRocket->setDirection(DOWN);
		m_pRocket->movePlayerVertically(1);
	}

	if(( keystates[ SDL_SCANCODE_ESCAPE ] ) || ( keystates[ SDL_SCANCODE_Q ] ))
	{
		done = 1;
	}
	if ((keystates[SDL_SCANCODE_H])){
		m_gameState = FLASH;
	}
	if (keystates[SDL_SCANCODE_SPACE]){
		if (m_pRocket->getShootBool()){
			missileManager->makeMissile(m_pRocket->getRect(), m_pRocket->getShootDirectionX(), m_pRocket->getShootDirectionY());
			m_pRocket->setShootBool(false);
		}
	}
	if (!keystates[SDL_SCANCODE_SPACE]){
		m_pRocket->setShootBool(true);
	}

}
float GameManager::getHeight(){
	return m_worldMaxY; //yes I know I should be using a variable here
}
float GameManager::getWidth(){
	return m_worldMaxX;//yes I know I should be using a variable here
}
void GameManager::setGameState(gameStateType inState){
	m_gameState = inState;
}
SDL_AudioSpec GameManager::setupAudioSpec(){
	SDL_AudioSpec fmt;
	//Set 16-bit stereo audio at 22Khz 
	fmt.freq = 44100;
	// fmt.freq = 22050;
	fmt.format = AUDIO_S16;
	fmt.channels = 2;
	fmt.samples = 512;        // A good value for games 
	fmt.callback = mixaudio;
	fmt.userdata = NULL;

	return fmt;
}
void GameManager::chasePlayer() {
	float tarX = m_pRocket->X();
	float tarY = m_pRocket->Y();
	if (!alien->getDead()){
		alien->followRocket(tarX, tarY);
	}

}
void GameManager::detectCollision(){
	SDL_Rect shipRect = m_pRocket->getRect();
	SDL_Rect alienRect = alien->getRect();
	SDL_Point shipCenter = m_pRocket->getCenter();
	SDL_Point alienCenter = alien->getCenter();
	float distanceBetween = sqrt(((shipRect.x + shipCenter.x) - (alienCenter.x + alienRect.x)) * ((shipRect.x + shipCenter.x) - (alienCenter.x + alienRect.x)) + (((shipRect.y + shipCenter.y) - (alienCenter.y + alienRect.y)) *((shipRect.y + shipCenter.y) - (alienCenter.y + alienRect.y))));
	//if (distanceBetween)
	//printf(" Distance %f \n", distanceBetween);
	//printf(" Ship Center %f \n", m_pRocket->getCenter());

	if (distanceBetween < 50){
		setGameState(FLASH);
	}
	missileManager->detectMissileCollision(*alien);
}