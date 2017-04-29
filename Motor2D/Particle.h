#pragma once
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "ParticleManager.h"

#define TIME_WIND 160

class Particle
{
public:
	//Initialize position and animation
	//Constructor Particle
	Particle(fPoint respawn, iPoint area_respawn = { 12, 2 }, iPoint timelife = { 15,5 }, fPoint speed = { 0,0 }, P_Direction p_direction = P_NON, SDL_Rect initial_rect = { 0,0,8,2 }, int size = 2, int num_tex_p = 4, bool active = false, Wind dir = W_NON, iPoint num_tex = { 0,0 });

	//Reactive this only when the particle is created
	bool Modify(fPoint respawn, iPoint area_respawn = { 12, 2 }, iPoint timelife = { 15,5 }, iPoint num_tex_p = { 0, 4 });

	//Checks if particle is dead
	bool isDead();

	//render particle
	void render();

	//Move particle
	void Move(fPoint speed, Wind dir = W_NON, bool Move_alternative = false);

	//GetPosition
	fPoint GetPosition();
	//GetSpeed
	fPoint GetSpeed();
	//Set Gravity (Simulation)
	void SetSpeedGreavity(fPoint speed);

	//Only Firework --------------------
	void SetRepeat(bool);
	bool GetRepeat();

private:
	//position
	fPoint position;

	//Texture
	SDL_Rect rect;
	SDL_Rect initial_rect;

	//velocity
	fPoint speed;
	int size_rect;

	//Rect

	bool not_repeat; //ONLY FIREWORK

	//Time Life
	iPoint degrade;
};

#endif /*_PARTICLE_H_*/