#include "ParticleManager.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Audio.h"
#include "j1Timer.h"
#include "P_Follow.h"
#include "P_Fire.h"
#include "P_Explosion.h"
#include "P_Firework.h"

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

bool ParticleManager::Awake()
{
	return true;
}

bool ParticleManager::Start()
{
	//texture
	atlas_particle = App->tex->Load("Particles/atlas_particle.png");
	//Mouse
	//Group_Fire.push_back(new P_Fire(nullptr, App->input->GetMousePosition_pointer(), iPoint(0,0), SDL_Rect{ 0,2,2,0 }, iPoint(5, 2), iPoint(12, 4), fPoint(60, -60), P_RANDOM, 65, 4, true, W_NON));
	return true;
}

bool ParticleManager::PreUpdate()
{
	return true;
}

bool ParticleManager::Update(float dt)
{
	//Iterate all list
	//Group FOLLOW -------------------------------------------------
	std::list<P_Follow*>::iterator item = Group_Follow.begin();
	while (item != Group_Follow.end())
	{
		item._Ptr->_Myval->Update(dt);
		item++;
	}

	//Group FIRE -------------------------------------------------
	std::list<P_Fire*>::iterator item_2 = Group_Fire.begin();
	while (item_2 != Group_Fire.end())
	{
		item_2._Ptr->_Myval->Update(dt);
		item_2++;
	}

	//Group EXPLOSION -------------------------------------------------
	std::list<P_Explosion*>::iterator item_3 = Group_Explosion.begin();
	while (item_3 != Group_Explosion.end())
	{
		if (item_3._Ptr->_Myval->godelete)
		{
			item_3._Ptr->_Myval->particle.clear();
			Group_Explosion.erase(item_3);
		}
		else
		{
			item_3._Ptr->_Myval->Update(dt);
		}
		item_3++;
	}

	//Group FIREWORK -------------------------------------------------
	std::list<P_Firework*>::iterator item_4 = Group_Firework.begin();
	while (item_4 != Group_Firework.end())
	{
		if (item_4._Ptr->_Myval->godelete)
		{
			item_4._Ptr->_Myval->particle.clear();
			Group_Firework.erase(item_4);
		}
		else
		{
			item_4._Ptr->_Myval->Update(dt);
		}
		item_4++;
	}
	return true;
}

bool ParticleManager::PostUpdate()
{
	//Iterate all list
	//Group FOLLOW -------------------------------------------------
	std::list<P_Follow*>::iterator item = Group_Follow.begin();
	while (item != Group_Follow.end())
	{
		item._Ptr->_Myval->PostUpdate();
		item++;
	}
	//Group FRIE -------------------------------------------------
	std::list<P_Fire*>::iterator item_2 = Group_Fire.begin();
	while (item_2 != Group_Fire.end())
	{
		item_2._Ptr->_Myval->PostUpdate();
		item_2++;
	}
	//Group EXPLOSION -------------------------------------------------
	std::list<P_Explosion*>::iterator item_3 = Group_Explosion.begin();
	while (item_3 != Group_Explosion.end())
	{
		item_3._Ptr->_Myval->PostUpdate();
		item_3++;
	}
	//Group FIREWORK -------------------------------------------------
	std::list<P_Firework*>::iterator item_4 = Group_Firework.begin();
	while (item_4 != Group_Firework.end())
	{
		item_4._Ptr->_Myval->PostUpdate();
		item_4++;
	}


	return true;
}

bool ParticleManager::CleanUp()
{
	return true;
}

//CREATE GROUPS -------------------------------------------------------------------------------------------------------------------------------------

void ParticleManager::CreateFollow_P(SceneElement* element_to_follow, iPoint* object_follow, SDL_Rect initial_rect, iPoint area, iPoint timelife, int num_textures, int num_particles, bool active, bool isMouse)
{
	Group_Follow.push_back(new P_Follow(element_to_follow, object_follow, initial_rect, area, timelife, num_textures, num_particles, active, isMouse));
}

void ParticleManager::CreateFire_Particle(SceneElement* element_to_follow, iPoint* object_follow, iPoint position_static, SDL_Rect initial_rect, iPoint area, iPoint timelife, fPoint speed, P_Direction p_direction, int num_particles, int num_textures, bool active, Wind w_dir)
{
	Group_Fire.push_back(new P_Fire(element_to_follow, object_follow, position_static, initial_rect, area, timelife, speed, p_direction, num_particles, num_textures, active, w_dir));
}

void ParticleManager::CreateExplosion_Particle(SceneElement* element_to_follow, iPoint* object_follow, iPoint position_static, SDL_Rect initial_rect, Explosion_Type type, iPoint perimeter, iPoint timelife, fPoint speed, P_Direction p_direction, int num_particles, int num_textures)
{
	Group_Explosion.push_back(new P_Explosion(element_to_follow, object_follow, position_static, initial_rect, type, perimeter, timelife, speed, p_direction, num_particles, num_textures));
}

void ParticleManager::CreateFirework_Particle(SceneElement* element_to_follow, iPoint* object_follow, iPoint position_static, SDL_Rect initial_rect, iPoint timelife, fPoint speed, P_Direction p_direction, int num_particles, int num_textures, iPoint next_textures, iPoint last_textures)
{
	Group_Firework.push_back(new P_Firework(element_to_follow, object_follow, position_static, initial_rect, timelife, speed, p_direction, num_particles, num_textures, next_textures, last_textures));
}


//DELETE GROUPS -------------------------------------------------------------------------------------------------------------------------------------

bool ParticleManager::DeleteFollow_p(P_Follow* group)
{
	std::list<P_Follow*>::iterator item = Group_Follow.begin();
	while (item != Group_Follow.end())
	{
		if (item._Ptr->_Myval == group)
		{
			Group_Follow.erase(item);
		}
		item++;
	}
	return true;
}

bool ParticleManager::DeleteFire_p(P_Fire* group)
{
	std::list<P_Fire*>::iterator item = Group_Fire.begin();
	while (item != Group_Fire.end())
	{
		if (item._Ptr->_Myval == group)
		{
			Group_Fire.erase(item);
		}
		item++;
	}
	return true;
}

bool ParticleManager::DeleteExplosion_p(P_Explosion* group)
{
	std::list<P_Explosion*>::iterator item = Group_Explosion.begin();
	while (item != Group_Explosion.end())
	{
		if (item._Ptr->_Myval == group)
		{
			Group_Explosion.erase(item);
		}
		item++;
	}
	return true;
}

bool ParticleManager::DeleteFirework_p(P_Firework* group)
{
	std::list<P_Firework*>::iterator item = Group_Firework.begin();
	while (item != Group_Firework.end())
	{
		if (item._Ptr->_Myval == group)
		{
			Group_Firework.erase(item);
		}
		item++;
	}
	return true;
}