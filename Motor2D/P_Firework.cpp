#include "P_Firework.h"
#include "Particle.h"

P_Firework::P_Firework(SceneElement* element, iPoint* object, iPoint position_static, SDL_Rect initial_rect, iPoint timelife_, fPoint speed_particle, P_Direction p_direction, int num_particles, int num_textures, iPoint next_textture_, iPoint last_textures_)
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

	speed = speed_particle;
	i_rect = initial_rect;
	next_textures = next_textture_;
	last_textures = last_textures_;
	//
	timelife = timelife_;
	number_particles = num_particles;
	number_multifirework = num_particles;
	godelete = false;
	n_textures = num_textures;
	size_rect = initial_rect.w;
	for (int i = 0; i < 1; i++)//
	{
		Particle* temp = new Particle(pos, iPoint(0, 0), timelife, speed, p_direction, initial_rect, size_rect, n_textures, true);
		particle.push_back(temp);
	}


}

P_Firework::~P_Firework()
{



}

bool P_Firework::Update(float dt)
{

	MoveParticles();


	return true;
}

bool P_Firework::PostUpdate()
{
	if (particle.size() == 1)
	{
		render(particle[0]->GetPosition());
	}
	else
	{
		render(pos);
	}

	return true;
}

void P_Firework::render(fPoint pos)
{
	if (particle.size() == 1)
	{
		if (particle[0]->isDead())
		{
			timelife.y += 1;
			for (int i = 1; i < number_particles; i++)
			{
				speed.x = 100;
				speed.y = 100;
				Particle* temp = new Particle(pos, iPoint(0, 0), timelife, speed, P_RANDOM_FIREWORK, i_rect, size_rect, n_textures, true, W_NON, next_textures);
				particle.push_back(temp);
			}
			timelife.y -= 1;
		}
	}
	else
	{
		for (int i = 1; i < number_particles; i++)
		{
			if (particle[i]->isDead() && particle[i]->GetRepeat())
			{
				particle[i]->SetRepeat(false);
				for (int k = 0; k < MULITFIREWORK; k++)
				{
					speed.x = 100;
					speed.y = 100;
					Particle* temp = new Particle(particle[i]->GetPosition(), iPoint(0, 0), timelife, speed, P_RANDOM_FIREWORK, i_rect, size_rect, n_textures, true, W_NON, last_textures);
					particle.push_back(temp);
					number_multifirework++;
				}
			}
		}
	}

	if (particle.size() == 1)
	{
		//Draw particles
		for (int i = 0; i < 1; i++)
		{
			particle[i]->render();
		}
	}
	else if (particle.size() == number_particles)
	{
		//Draw particles
		for (int i = 0; i < number_particles; i++)
		{
			particle[i]->render();
		}
	}
	else //	number_multifirework;
	{
		//Draw particles
		for (int i = 0; i < number_multifirework; i++)
		{
			particle[i]->render();
		}
	}


}

void P_Firework::MoveParticles()
{
	if (particle.size() == 1)
	{
		for (int i = 0; i < 1; i++)
		{
			particle[i]->SetSpeedGreavity(fPoint(0, 5));
		}
		for (int i = 0; i < 1; i++)
		{
			float temp = App->GetDT();
			particle[i]->Move(fPoint(particle[i]->GetSpeed().x * temp, particle[i]->GetSpeed().y * temp));
		}
	}
	else if (particle.size() == number_particles)
	{
		for (int i = 1; i < number_particles; i++)
		{
			particle[i]->SetSpeedGreavity(fPoint(0, 5));
		}
		for (int i = 1; i < number_particles; i++)
		{
			float temp = App->GetDT();
			particle[i]->Move(fPoint(particle[i]->GetSpeed().x * temp, particle[i]->GetSpeed().y * temp));
		}
	}
	else
	{
		for (int i = number_particles; i < number_multifirework; i++)
		{
			particle[i]->SetSpeedGreavity(fPoint(0, 5));
		}
		for (int i = number_particles; i < number_multifirework; i++)
		{
			float temp = App->GetDT();
			particle[i]->Move(fPoint(particle[i]->GetSpeed().x * temp, particle[i]->GetSpeed().y * temp));
		}
	}

}
