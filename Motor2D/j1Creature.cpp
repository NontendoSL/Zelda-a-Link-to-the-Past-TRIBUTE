#include "j1Creature.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Scene.h"
#include "j1Player.h"

bool Creature::Move(int x, int y)
{
	bool ret = true;
	if (x < 0) //LEFT
	{
		if (App->map->MovementCost(collision_feet->rect.x + x, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			position.x += x;
		}
	}
	else //RIGHT
	{
		if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + x, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
		{
			position.x += x;
		}
	}

	if (y > 0) //DOWN
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + y, collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
		{
			position.y += y;
		}
	}
	else // UP
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + y, collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
		{
			position.y += y;
		}
	}

	return ret;
}

bool Creature::GoTo(iPoint destination, int speed)
{
	destination = App->map->MapToWorld(destination.x, destination.y);

	iPoint toMove = { 0, 0 };

	if (destination.x > position.x)
	{
		toMove.x = 1;
		direction = RIGHT;
	}
	else if (destination.x < position.x)
	{
		toMove.x = -1;
		direction = LEFT;
	}

	if (destination.y > position.y)
	{
		toMove.y = 1;
		direction = DOWN;
	}

	else if (destination.y < position.y)
	{
		toMove.y = -1;
		direction = UP;
	}

	if (toMove.IsZero())
		return false;

	Move(ceil(speed)*toMove.x, ceil(speed)*toMove.y);

	return true;
}

bool Creature::Orientate()
{
	iPoint player_pos = App->scene->player->position;

	Direction prev_dir = direction;
	if (abs(player_pos.x - position.x) < abs(player_pos.y - position.y))
	{
		if (player_pos.y > position.y)
		{
			direction = DOWN;
		}

		else
		{
			direction = UP;
		}
	}

	else
	{
		if (player_pos.x < position.x)
		{
			direction = LEFT;
		}

		else
		{
			direction = RIGHT;
		}
	}

	if (prev_dir == direction)
	{
		return false;
	}

	else
	{
		return true;
	}


}

