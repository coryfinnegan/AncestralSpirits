
#include "time.h"
#include "SDL.h"
#include "SDL_audio.h"

class Rocket;
class Actor;
class Pad;
class Alien;
class MissileManager;

enum gameStateType { START, INPLAY, FLASH, EXPLODING, NOTHING, LANDED };

#define MAXLAND 20

class GameManager
{
public:		
	GameManager();

	~GameManager ();

	int Initialize(Actor* Actors[]);

	void Update();
	void Draw();

	int padIntersect(Pad* pPad, Actor* pRocket);

	void UserInput();

	gameStateType getGameState() { return m_gameState;}
	void setGameState(gameStateType g);

	clock_t getCurrTime() { return m_currTime;}
	double getElapsedTime() { return m_elapsedTime;}

	int initializeSDL();

	int endGraphics();

	int gameOver();

	float getWidth();
	float getHeight();

	SDL_Renderer* getRenderer();
	void updateOffsets();
	void updateActors();

	SDL_AudioSpec getAudioSpec() { return obtained; };
	SDL_AudioSpec setupAudioSpec();
	void chasePlayer();
	void detectCollision();

private:



	SDL_Window* window;
	SDL_Renderer* renderer;
	const Uint8 *keystates;
	const Uint8 *prevkeystates;
	SDL_Event event;

	long int m_iFrame;

	// time vairiables
	clock_t m_currTime, m_oTime, m_eTime, m_fTime;
	clock_t m_exploTime;
	double m_elapsedTime;


	int m_screenWidth;
	int m_screenHeight;
	float m_worldMaxX;
	float m_worldMaxY;

	Pad* m_Pads[MAXLAND];  // landing pads
	int m_numPads;

	gameStateType m_gameState; //  { INPLAY, FLASH, EXPLODING, NOTHING, LANDED };


	Rocket* m_pRocket;
	Alien* alien;
	MissileManager* missileManager;


	int done;
	//Game variables
	float margin;
	float xScreenSize;
	float yScreenSize;
	float xOffset;
	float yOffset;
	Actor* *ActorsPtr;
	int nActors;
	SDL_AudioSpec obtained;

};