
#include "stdafx.h"
#include "GameManager.h"
#include "Actor.h"


#define MAX_ACTORS 20  // total allowable actors


GameManager* pGameManager; // game manager

Actor* Actors[MAX_ACTORS];  // actors
int numActors;  // number of actors


void initialize()
{

	pGameManager = new GameManager();  // create new game manager
	if (pGameManager == NULL) {
		// panic
		exit(0);
	}

	numActors = pGameManager->Initialize(Actors);  // instantiate actors

	// call actors load routines
	for (int i = 0; i< numActors; i++)
	{
		Actors[i]->Load();
	}

}


void update()
{ // call update for every actor, then call the game manager's update
	for (int i=0; i< numActors; i++)
	{
		Actors[i]->Update();
	}

	pGameManager->Update();

}

void draw()
{ // call draw for every actor, then call the game manager's draw

  	for (int i=0; i< numActors; i++)
	{
		Actors[i]->Draw();
	}
 
	 pGameManager->Draw();

}


int main( int argc, char* args[] ) 
{ 

	initialize();


	while ( !pGameManager->gameOver() ) {
		// main game loop
		update();
		draw();

	}

	// clean up and exit
	pGameManager->endGraphics();
	return 0; 
}