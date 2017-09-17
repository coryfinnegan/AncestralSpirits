
#include "Actor.h"

enum directionFacing { LEFT, RIGHT, UP, DOWN };
class Rocket: public Actor
{
public:		
	Rocket(GameManager* pMan);

	~Rocket ();

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
	void stopThrustingSound();
	void movePlayerHorizontally(float inValue);
	void movePlayerVertically(float inValue);
	SDL_Point getCenter();
	float getVelocityX();
	float getVelocityY();
	SDL_Rect getRect();
	void setDirection(directionFacing direction);
	void setShootDirection();
	float getShootDirectionX();
	float getShootDirectionY();
	bool getShootBool();
	void setShootBool(bool inBool);

	

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
	SDL_Point shipCenter;
	float turnSpeed;
	float pi;
	bool thrusting; 
	bool playSound;
	
	// ship screen location
	SDL_Rect destinRect;
	SDL_Rect rShip;
	SDL_Texture* tShip;
	SDL_Surface* shipForwardSurface;
	SDL_Surface* shipSurfaceRight;
	SDL_Texture* shipTextureRight;
	SDL_Texture* shipForwardTexture;
	SDL_Surface* sShip;
	SDL_Surface* sShipThrust;
	SDL_Texture* tShipThrust;
	SDL_Rect rExplosion;
	SDL_Texture* tExplosion;
	SDL_Surface* sExplosion;
	SDL_Surface* shipSurfaceBack;
	SDL_Surface* shipSurfaceSide;
	SDL_Texture* shipTextureBack;
	SDL_Texture* shipTextureSide;


	float rocketScreenPosX;
	float rocketScreenPosY;
	float explosionTime;
	int currentRow;
	int currentColumn;
	int engineIndex;
	int rocketThrustSound;
	directionFacing direction;
	float shootDirectionX;
	float shootDirectionY;
	bool shootBool;
	float bulletSpeed;


	
};