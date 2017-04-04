#include "j1Creature.h"
#include "j1Collision.h"
#include "j1App.h"
#include "j1Scene.h"
#include "j1Player.h"

bool Creature::Move(int x, int y)
{
	bool ret = true;

	position.x += x;
	if (App->map->CheckTileCost(x, y) == 1)
	{
		position.x -= x;
		ret = false;
	}

	position.y += y;
	if (App->map->CheckTileCost(x, y) == 1)
	{
		position.y -= y;
		ret = false;
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


