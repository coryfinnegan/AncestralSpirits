#pragma once
#include "Actor.h"
#include <vector>

class Alien;
using namespace std;
class MissileManager : public Actor
{
public:
	MissileManager(GameManager* pMan);
	~MissileManager();
	virtual void Update();
	virtual void Draw();
	virtual void Load();

	void makeMissile(SDL_Rect missileMakerPosition, float, float);
	void setVisible(SDL_Rect inRect);
	void moveMissle(vector<SDL_Rect>&missileRects);
	void detectMissileCollision(Alien &alien);

private:
	std::vector<SDL_Rect>missileRects;
	std::vector<SDL_Point>missilePositions;
	std::vector<bool>missileAliveValues;
	SDL_Surface* missileSurface;
	SDL_Texture* missileTexture;

};

