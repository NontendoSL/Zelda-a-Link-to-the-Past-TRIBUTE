#pragma once
#ifndef __ENEMIES_H_
#define __ENEMIES_H_

#include "j1SceneElements.h"


class j1Enemy : public j1SceneElement
{
public:
	j1Enemy(iPoint position, int hp, int attack, float speed);

	~j1Enemy();

	void Update();

	void Draw();

private:
	int hp, attack;
	//Item* drop;
	float speed;
	bool stunned=false;
};

#endif