
#include "Actor.h"

class Pad: public Actor
{
public:		
	Pad(GameManager* pMan, float xMin, float xMax, float y);

	~Pad ();

	virtual void Update();

	virtual void Draw();

	virtual void Load();

	float returnXPos();
	float returnWidth();


private:
	SDL_Rect rPad;


};