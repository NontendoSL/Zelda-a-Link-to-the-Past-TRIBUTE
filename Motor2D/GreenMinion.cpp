#include "GreenMinion.h"
#include "j1Scene.h"

GreenMinion::GreenMinion()
{
	name = "GreenMinion";
	type = CREATURE;
}


GreenMinion::~GreenMinion()
{
}

bool GreenMinion::Start()
{
	//Load initial position & direction & movement speed
	position.x = 0;
	position.y = 0;
	direction = DOWN;
	speed = 0;

	//Animation
	state = GM_SPAWNING;
	anim_state = GM_WALKING;
	animation = *App->anim_manager->GetAnimStruct(GREENMINION);

	//Set Collision
	offset_x = 0;
	offset_y = 0;
	collision_feet = nullptr;

	return true;
}

bool GreenMinion::Update(float dt)
{
	if (App->scene->gamestate == INGAME)
	{
		switch (state)
		{
		case GM_SPAWNING:
		{
			Movebyhit(dt);
			break;
		}
		case GM_IDLE:
		{
			Idle();
			break;
		}
		case GM_WALKING:
		{
			Walking(dt);
			break;
		}
		case GM_HIT:
		{
			Movebyhit(dt);
			break;
		}
		case GM_DYING:
		{
			Die();
			break;
		}
		default:
		{
			break;
		}
		}
	}
	return true;
}

void GreenMinion::Draw()
{
	if (direction == UP)
	{
		anim_rect = animation.anim[anim_state].North_action.GetCurrentFrame();
		pivot = animation.anim[anim_state].North_action.GetCurrentOffset();
	}
	else if (direction == DOWN)
	{
		anim_rect = animation.anim[anim_state].South_action.GetCurrentFrame();
		pivot = animation.anim[anim_state].South_action.GetCurrentOffset();
	}
	else if (direction == LEFT)
	{
		anim_rect = animation.anim[anim_state].West_action.GetCurrentFrame();
		pivot = animation.anim[anim_state].West_action.GetCurrentOffset();
	}
	else if (direction == RIGHT)
	{
		anim_rect = animation.anim[anim_state].East_action.GetCurrentFrame();
		pivot = animation.anim[anim_state].East_action.GetCurrentOffset();
	}

	App->render->Blit(animation.graphics, position.x - pivot.x, position.y - pivot.y, &anim_rect);
}

bool GreenMinion::Idle()
{
	return true;
}

bool GreenMinion::Walking(float dt)
{
	return true;
}

bool GreenMinion::Move(float dt)
{
	return true;
}

bool GreenMinion::Movebyhit(float dt)
{
	return true;
}

bool GreenMinion::Die()
{
	return true;
}



