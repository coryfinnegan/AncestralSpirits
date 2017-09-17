
#ifndef ACTORH
#define ACTORH


class GameManager;

class Actor
{  // base class for level actors
public:		
	Actor();
	Actor(GameManager* pMan);

	~Actor ();

	// virtual functions must be implemented by derived class
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Load() = 0;

	// functions for getting the object's position or offset
	float X() { return m_posX;}
	float Y() { return m_posY;}
	float OX() { return m_offX;}
	float OY() { return m_offY;}
	void setX(float inValueX) { m_posX = inValueX; }
	void setY(float inValueY) { m_posY = inValueY; }
	void setOffX(float inValueX) { m_offX = inValueX; }
	void setOffY(float inValueY) { m_offY = inValueY; }
	SDL_Point getCenter();
	SDL_Rect getRect();

protected:

	GameManager* m_pGM;  // pointer to game manager

	// object world position
	float m_posX;
	float m_posY;

	// object world offset usually
	float m_offX;
	float m_offY;

	SDL_Point shipCenter;
	SDL_Rect rShip;

};

#endif  #ifndef ACTORH