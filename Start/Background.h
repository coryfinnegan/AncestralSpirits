
#include "Actor.h"

class Background: public Actor
{
public:		
	Background(GameManager* pMan);

	~Background ();

	virtual void Update();

	virtual void Draw();

	virtual void Load();



private:

	// image pointers
	SDL_Surface* sBackground; 
	SDL_Texture* tBackground;

	// image locations
	SDL_Rect rBackground;




};