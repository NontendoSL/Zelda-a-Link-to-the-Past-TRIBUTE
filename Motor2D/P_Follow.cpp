#include "P_Follow.h"
#include "Particle.h"
#include "j1Player.h"
#include "j1Window.h"

P_Follow::P_Follow(SceneElement* element, iPoint* object, SDL_Rect initial_rect, iPoint area_, iPoint timelife_, int num_textures, int num_particles, bool active_, bool mouse)
{
	if (element != nullptr)
	{
		pos.x = element->position.x;
		pos.y = element->position.y;
		element_to_follow = element;
		object_follow = nullptr;
	}
	else
	{
		pos.x = object->x;
		pos.y = object->y;
		object_follow = object;
		element_to_follow = nullptr;
	}
	area = area_;
	number_particles = num_particles;
	godelete = false;
	isMouse = mouse;
	active = active_;
	timelife = timelife_;
	n_textures = num_textures;
	size_rect = initial_rect.w;
	for (int i = 0; i < num_particles; i++)//
	{
		Particle* temp = new Particle(pos, area, timelife, fPoint(0,0), P_NON, initial_rect, size_rect, num_textures, active_);
		particle.push_back(temp);
	}
}

P_Follow::~P_Follow()
{
}

bool P_Follow::Update(float dt)
{
	if (element_to_follow != nullptr)
	{
		pos.x = element_to_follow->position.x;
		pos.y = element_to_follow->position.y;
	}
	else
	{
		Update_position(object_follow);
	}
	return true;
}

bool P_Follow::PostUpdate()
{
	render(pos);
	return true;
}

void P_Follow::render(fPoint pos)
{
	if (active)
	{
		for (int i = 0; i < number_particles; i++)
		{
			if (particle[i]->isDead())
			{
				particle[i]->Modify(pos, area, timelife, iPoint(0, n_textures));
			}
		}
	}

	//Draw particles
	for (int i = 0; i < number_particles; i++)
	{
		particle[i]->render();
	}
}

void P_Follow::Update_position(iPoint* element)
{
	if (isMouse == false)
	{
		pos.x = element->x;
		pos.y = element->y;
	}
	else
	{
		pos.x = element->x - App->render->camera.x / 2;
		pos.y = element->y - App->render->camera.y / 2;
	}

	LOG("%i - %i", element->x, element->y);
}
