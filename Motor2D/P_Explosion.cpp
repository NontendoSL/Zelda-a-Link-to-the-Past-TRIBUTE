#include "P_Explosion.h"
#include "Particle.h"

P_Explosion::P_Explosion(SceneElement* element, iPoint* object, iPoint position_static, SDL_Rect initial_rect, Explosion_Type type, iPoint perimeter_object, iPoint timelife_particle, fPoint speed_particle, P_Direction p_direction, int num_particles, int num_textures)
{
	if (element != nullptr)
	{
		pos.x = element->position.x;
		pos.y = element->position.y;
		element_to_follow = element;
		object = nullptr;
	}
	else if (object != nullptr)
	{
		pos.x = object->x;
		pos.y = object->y;
		object_follow = object;
		element_to_follow = nullptr;
	}
	else
	{
		pos.x = position_static.x;
		pos.y = position_static.y;
		object_follow = nullptr;
		element_to_follow = nullptr;
	}
	//
	timelife = timelife_particle;
	number_particles = num_particles;
	godelete = false;
	size_rect = initial_rect.w;
	n_textures = num_textures;

	type_explosion = type;
	//CIRCLE
	if (type == CIRCLE)
	{
		pos.x -= num_particles * 2;
		pos.y -= num_particles * 3;
		fPoint save_pos = pos;
		speed = speed_particle;
		float part_entre = (num_particles - 4) / 4;
		float speed_modify = -speed.y / part_entre;
		int time_quart = num_particles / 2;

		int num_test = 0;
		float r = num_particles;
		float pr = 2; // pr is the aspected pixel ratio which is almost equal to 2
		for (int i = -r; i <= r; i++) // loop for horizontal movement
		{
			for (int j = -r; j <= r; j++) // loop for vertical movement
			{
				float d = ((i*pr) / r)*((i*pr) / r) + (j / r)*(j / r); //multiplying the i variable with pr to equalize pixel-width with the height
				if (d >0.95 && d<1.08) // approximation
				{
					for (int k = 0; k < num_particles / 4; k++)
					{
						Particle* temp = new Particle(pos, iPoint(3, 3), timelife, speed, p_direction, initial_rect, size_rect, num_textures, true);
						particle.push_back(temp);
						num_test++;
					}
					pos.x += 3;
				}
				else
				{
					pos.x += 2;
				}
			}
			pos.x = save_pos.x;
			pos.y += 3;
		}
		number_particles = num_test;
	}
	else if (type == CROSS)
	{

		speed = speed_particle;

		int num_line = num_particles / 2;
		int space_x = (perimeter_object.x * 2) / num_line;
		int temp_x = -num_particles;
		int space_y = (perimeter_object.y * 2) / num_line;
		int temp_y = -num_particles;
		int mid_pos = pos.x;
		pos.x += temp_x;
		bool setpos_y = false;
		for (int i = 0; i < num_particles; i++)//
		{
			if (i < num_line)
			{
				if (i < num_line / 2)
				{
					speed.x = -60;
					speed.y = 0;
					Particle* temp = new Particle(pos, iPoint(0, 0), timelife, speed, p_direction, initial_rect, size_rect, num_textures, true);
					particle.push_back(temp);
				}
				else
				{
					speed.x = 60;
					speed.y = 0;
					Particle* temp = new Particle(pos, iPoint(0, 0), timelife, speed, p_direction, initial_rect, size_rect, num_textures, true);
					particle.push_back(temp);
				}

				pos.x += space_x;
				setpos_y = true;
			}
			else
			{
				pos.x = mid_pos;
				if (setpos_y)
				{
					pos.y += temp_y;
					setpos_y = false;
				}
				if (i < num_line + num_line / 2)
				{
					speed.y = -60;
					speed.x = 0;
					Particle* temp = new Particle(pos, iPoint(0, 0), timelife, speed, p_direction, initial_rect, size_rect, num_textures, true);
					particle.push_back(temp);
				}
				else
				{
					speed.y = 60;
					speed.x = 0;
					Particle* temp = new Particle(pos, iPoint(0, 0), timelife, speed, p_direction, initial_rect, size_rect, num_textures, true);
					particle.push_back(temp);
				}
				pos.y += space_y;
			}
		}
	}
	else if (type == RANDOM)
	{
		speed = speed_particle;
		for (int i = 0; i < num_particles; i++)//
		{
			Particle* temp = new Particle(pos, perimeter_object, timelife, speed, p_direction, initial_rect, size_rect, num_textures, true);
			particle.push_back(temp);
		}
	}
}

P_Explosion::~P_Explosion()
{
}

bool P_Explosion::Update(float dt)
{
	MoveParticles();

	return true;
}

bool P_Explosion::PostUpdate()
{
	render(pos);
	return true;
}

void P_Explosion::render(fPoint pos)
{
	int num_dead = 0;
	//Check if the particle dead
	for (int i = 0; i < number_particles; i++)
	{
		if (particle[i]->isDead())
		{
			num_dead++;
		}
	}
	if (num_dead == number_particles)
	{
		godelete = true;
	}

	//Draw particles
	for (int i = 0; i < number_particles; i++)
	{
		particle[i]->render();
	}
}

void P_Explosion::MoveParticles()
{
	if (type_explosion == RANDOM)
	{
		for (int i = 0; i < number_particles; i++)
		{
			particle[i]->SetSpeedGreavity(fPoint(0, 5));
		}
	}
	for (int i = 0; i < number_particles; i++)
	{
		float temp = App->GetDT();
		particle[i]->Move(fPoint(particle[i]->GetSpeed().x * temp, particle[i]->GetSpeed().y * temp));
	}
}
