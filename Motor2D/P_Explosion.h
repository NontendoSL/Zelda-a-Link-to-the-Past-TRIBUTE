#pragma once
#ifndef _P_EXPLOSION_H_
#define _P_EXPLOSION_H_

#include "ParticleManager.h"
#include "SceneElements.h"


class Particle;

class P_Explosion
{
public:
	P_Explosion(SceneElement* element, iPoint* object_follow, iPoint position_static, SDL_Rect initial_rect = { 0,0,8,2 }, Explosion_Type type = RANDOM, iPoint perimeter = { 20, 20 }, iPoint timelife = { 20,10 }, fPoint speed = { 0,0 }, P_Direction p_direction = P_NON, int num_particles = 20, int num_textures = 4);
	~P_Explosion();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	void render(fPoint pos);

	void MoveParticles();

public:
	std::vector<Particle*> particle;
	SceneElement* element_to_follow;
	//Num of particles
	int number_particles;
	Explosion_Type type_explosion;
	//test
	iPoint* object_follow;

	//timelife
	iPoint timelife;

	//position
	fPoint pos;
	int size_rect;
	int n_textures;

	//velocity
	fPoint speed;
	bool active;

	bool godelete;

};


#endif /*_P_EXPLOSION_H_*/