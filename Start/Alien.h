
#include "Actor.h"

enum enemyPlayState { ALIVE, DEAD, DEADER };
class Alien : public Actor
{
public:
	Alien(GameManager* pMan);

	~Alien();

	virtual void Update();
	virtual void Draw();
	virtual void Load();

	void IncAngle();  // rot right
	void DecAngle();  // rot left
	void Thrust();    // forward
	void calculateVelocity();
	void calculateGravity();
	void resetAcceleration();
	void shipBoundaries();
	int safeLanding();  // check for safe landing
	SDL_Point returnCenter();
	void calculateScreenPos();
	void flash();
	void explode();
	void stopMoving();

	int reset();
	int rest();
	void playThrustSound();
	void setThrustingBool(bool inValue);
	bool getThrustingBool();
	void setThrustingTexture();
	void spawn();
	void followRocket(float tarX, float tarY);
	float speed;
	SDL_Rect getRect();
	void blowUp();
	void setDead(bool inBool);
	bool getDead();
	enemyPlayState alivePlayState;


private:

	// add rocket variables here
	float shipSpeed;
	float shipX;
	float shipY;
	float velocityX;
	float velocityY;
	float accelX;
	float accelY;
	float gravity;
	float shipAngle;
	float turnSpeed;
	float pi;
	bool thrusting;

	// ship screen location
	SDL_Rect destinRect;
	SDL_Rect rShip;
	SDL_Texture* tShip;
	SDL_Surface* sShip;
	SDL_Surface* sShipThrust;
	SDL_Texture* tShipThrust;
	SDL_Rect rExplosion;
	SDL_Texture* tExplosion;
	SDL_Surface* sExplosion;
	float rocketScreenPosX;
	float rocketScreenPosY;
	float explosionTime;
	int currentRow;
	int currentColumn;
	int engineIndex;
	int rocketThrustSound;
	bool dead;



};