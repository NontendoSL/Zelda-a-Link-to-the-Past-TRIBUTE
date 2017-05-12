#pragma once
#ifndef __TRAININGDOLL_H_
#define __TRAININGDOLL_H_

#include "PokemonCombat.h"

class TrainingDoll : public PokemonCombat
{
public:
	TrainingDoll();

	~TrainingDoll();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	//bool PostUpdate();

	void Draw();

	// Called before quitting
	bool CleanUp();

	bool Idle();

	void OnCollision(Collider*, Collider*);

private:

};

#endif //__TRAININGDOLL_H_

