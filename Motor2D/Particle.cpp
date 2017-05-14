#include "Particle.h"
#include "CombatManager.h"
#include "PokemonCombat.h"

//Constructor
Particle::Particle(fPoint respawn, iPoint area, iPoint timelife, fPoint speed_particle, P_Direction p_direction, SDL_Rect pos_rect, int size, int num_tex_pixel, bool active, Wind dir, iPoint num_tex)
{
	//with random change position
	if (area.x == 0)
		position.x = respawn.x;
	else
		position.x = respawn.x - (rand() % area.x) + (rand() % area.x);

	if (area.y == 0)
		position.y = respawn.y;
	else
		position.y = respawn.y - (rand() % area.y) + (rand() % area.y);

	//TimeLife of Particle

	if (active)
		degrade.y = MAX_TIMELIFE;
	else
		degrade.y = 0;

	if (timelife.x == 0)
	{
		degrade.x = timelife.y;
	}
	else
	{
		degrade.x = (rand() % timelife.x + timelife.y);
	}


	//Set Speed
	speed = speed_particle;
	if (p_direction == P_UP)
	{
		speed.y = - (((float)rand() / (float)(RAND_MAX)) * speed.y);
	}
	else if (p_direction == P_DOWN)
	{
		speed.y = (((float)rand() / (float)(RAND_MAX)) * speed.y);
	}
	else if (p_direction == P_LEFT)
	{
		speed.x =  - (((float)rand() / (float)(RAND_MAX)) * speed.x);
	}
	else if (p_direction == P_RIGHT)
	{
		speed.x = (((float)rand() / (float)(RAND_MAX)) * speed.x);
	}
	else if (p_direction == P_RANDOM_X)
	{
		speed.x = (((float)rand() / (float)(RAND_MAX)) * speed.x) - (((float)rand() / (float)(RAND_MAX)) * speed.x);
	}
	else if (p_direction == P_RANDOM_Y)
	{
		speed.y = (((float)rand() / (float)(RAND_MAX)) * speed.y) - (((float)rand() / (float)(RAND_MAX)) * speed.y);
	}
	else if (p_direction == P_RANDOM)
	{
		speed.x = (((float)rand() / (float)(RAND_MAX)) * speed.x) - (((float)rand() / (float)(RAND_MAX)) * speed.x);
		speed.y = (((float)rand() / (float)(RAND_MAX)) * speed.y) - (((float)rand() / (float)(RAND_MAX)) * speed.y);
	}
	else if(p_direction == P_RANDOM_FIREWORK)
	{
		speed.x = (((float)rand() / (float)(RAND_MAX)) * speed.x) - (((float)rand() / (float)(RAND_MAX)) * speed.x);
		speed.y = - ((((float)rand() / (float)(RAND_MAX)) * speed.y) + NORMAL_SPEED_Y);
	}

	not_repeat = true;
	if (num_tex.x != 0 && num_tex.y != 0)
	{
		//Set SDL_Rect
		switch (rand() % num_tex.y + num_tex.x)
		{
		case 0: rect = { pos_rect.x, pos_rect.y, size, size }; break;
		case 1: rect = { pos_rect.x + size, pos_rect.y, size, size }; break;
		case 2: rect = { pos_rect.x + size * 2, pos_rect.y, size, size }; break;
		case 3: rect = { pos_rect.x + size * 3, pos_rect.y, size, size }; break;
		case 4: rect = { pos_rect.x + size * 4, pos_rect.y, size, size }; break;
		case 5: rect = { pos_rect.x + size * 5, pos_rect.y, size, size }; break;
		case 6: rect = { pos_rect.x + size * 6, pos_rect.y, size, size }; break;
		case 7: rect = { pos_rect.x + size * 7, pos_rect.y, size, size }; break;
		}
	}
	else
	{
		//Set SDL_Rect
		switch (rand() % num_tex_pixel)
		{
		case 0: rect = { pos_rect.x, pos_rect.y, size, size }; break;
		case 1: rect = { pos_rect.x + size, pos_rect.y, size, size }; break;
		case 2: rect = { pos_rect.x + size * 2, pos_rect.y, size, size }; break;
		case 3: rect = { pos_rect.x + size * 3, pos_rect.y, size, size }; break;
		case 4: rect = { pos_rect.x + size * 4, pos_rect.y, size, size }; break;
		case 5: rect = { pos_rect.x + size * 5, pos_rect.y, size, size }; break;
		case 6: rect = { pos_rect.x + size * 6, pos_rect.y, size, size }; break;
		case 7: rect = { pos_rect.x + size * 7, pos_rect.y, size, size }; break;
		}
	}
	initial_rect = pos_rect;
	size_rect = size;
}

Particle::~Particle()
{
}

bool Particle::Modify(fPoint respawn, iPoint area, iPoint timelife, iPoint num_tex_p)
{
	//with random change position
	if (area.x == 0)
		position.x = respawn.x;
	else
		position.x = respawn.x - (rand() % area.x) + (rand() % area.x);

	if (area.y == 0)
		position.y = respawn.y;
	else
		position.y = respawn.y - (rand() % area.y) + (rand() % area.y);

	//TimeLife of Particle
	degrade.y = MAX_TIMELIFE;
	degrade.x = (rand() % timelife.x + timelife.y);

	//Set SDL_Rect
	switch (rand() % num_tex_p.y + num_tex_p.x)
	{
	case 0: rect = { initial_rect.x, initial_rect.y, size_rect, size_rect }; break;
	case 1: rect = { initial_rect.x + size_rect, initial_rect.y, size_rect, size_rect }; break;
	case 2: rect = { initial_rect.x + size_rect * 2, initial_rect.y, size_rect, size_rect }; break;
	case 3: rect = { initial_rect.x + size_rect * 3, initial_rect.y, size_rect, size_rect }; break;
	case 4: rect = { initial_rect.x + size_rect * 4, initial_rect.y, size_rect, size_rect }; break;
	case 5: rect = { initial_rect.x + size_rect * 5, initial_rect.y, size_rect, size_rect }; break;
	case 6: rect = { initial_rect.x + size_rect * 6, initial_rect.y, size_rect, size_rect }; break;
	case 7: rect = { initial_rect.x + size_rect * 7, initial_rect.y, size_rect, size_rect }; break;
	}
	return true;
}

bool Particle::isDead()
{
	//return is dead;
	return degrade.y == 0;
}

void Particle::render()
{
	degrade.y -= MIN(degrade.y, degrade.x);
	if (degrade.y <= 0)
	{
		degrade.y = 0;
	}
	SDL_SetTextureAlphaMod(App->particlemanager->atlas_particle, degrade.y);
	if (App->scene->combat)
	{
		if (App->combatmanager->pokemon_active_trainer->name == "SALAMANCE" && App->combatmanager->pokemon_active_trainer->GetState() == PC_SPECIAL
			&& (App->combatmanager->pokemon_active_trainer->direction == UP || App->combatmanager->pokemon_active_trainer->direction == DOWN))
		{
			App->render->Blit(App->particlemanager->atlas_particle, position.x, position.y, &rect, 1.0f, true, 90);
		}
		else
		{
			App->render->Blit(App->particlemanager->atlas_particle, position.x, position.y, &rect);
		}
	}
	else
	{
		App->render->Blit(App->particlemanager->atlas_particle, position.x, position.y, &rect);
	}
}

void Particle::Move(fPoint speed, Wind dir, bool Move_alternative)
{
	if (Move_alternative == false)
	{
		position.x += speed.x;
		position.y += speed.y;
	}
	else
	{
		if (dir == W_UP)
		{
			position.x += speed.x;
			if (degrade.y < TIME_WIND)
			{
				position.y += speed.y;
			}
		}
		else if (dir == W_DOWN)
		{
			position.x += speed.x;
			if (degrade.y < TIME_WIND)
			{
				position.y -= speed.y;
			}
		}
		else if (dir == W_RIGHT)
		{
			position.y += speed.y;
			if (degrade.y < TIME_WIND)
			{
				position.x += speed.x;
			}
		}
		else //dir == W_LEFT
		{
			position.y += speed.y;
			if (degrade.y < TIME_WIND)
			{
				position.x -= speed.x;
			}
		}

	}
}

fPoint Particle::GetPosition()
{
	return position;
}

fPoint Particle::GetSpeed()
{
	return speed;
}

void Particle::SetSpeedGreavity(fPoint speed_modify)
{
	speed.x += speed_modify.x;
	speed.y += speed_modify.y;
}

void Particle::SetRepeat(bool temp)
{
	not_repeat = temp;
}

bool Particle::GetRepeat()
{
	return not_repeat;
}
