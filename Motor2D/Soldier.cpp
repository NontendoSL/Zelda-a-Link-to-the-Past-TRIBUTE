#include "Soldier.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1App.h"
#include "p2Defs.h"
#include "j1Input.h"
#include "j1Item.h"
#include "j1Collision.h"

Soldier::Soldier():NPC()
{
	name = "enemies"; //TODO need change name to "Soldier".
	type = ENEMY;
}

Soldier::~Soldier()
{}

bool Soldier::Awake(pugi::xml_node &conf, uint id)
{
	bool stop_search = false;
	for (int s_id = conf.attribute("id").as_int(0); stop_search == false; s_id = conf.next_sibling().attribute("id").as_int(0))
	{
		if (id == s_id)
		{
			std::string temp = conf.attribute("file").as_string("");
			texture = App->tex->Load(temp.c_str());
			hp = conf.attribute("hp").as_int(0);
			position.x = conf.attribute("pos_x").as_int(0);
			position.y = conf.attribute("pos_y").as_int(0);
			temp = conf.attribute("dir").as_string("");
			if (temp == "up")
				dir = UP;
			else if (temp == "down")
				dir = DOWN;
			else if (temp == "left")
				dir = LEFT;
			else
				dir = RIGHT;

			npc_id = id;
			stop_search = true;
		}
	}

	return true;
}

bool Soldier::Start()
{
	collision_enemy = App->collision->AddCollider({ position.x, position.y, 15, 15 }, COLLIDER_ENEMY, this);

	soldier_right = { 17,0,18,28 };
	soldier_left = {36,0,18,28};
	soldier_up = {55,0,16,28};
	soldier_down = { 0,0,16,28 };

	soldier_left_2 = {26,20,21,28 };
	soldier_right_2 = {74,20,21,28 };
	soldier_up_2 = {48,20,25,28};
	soldier_down_2 = {0,18,25,30};
	marge = 12;
	return true;
}

bool Soldier::Update()
{
	/*if (App->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
	{
		position.x -= 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		position.y += 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	{
		position.x += 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
	{
		position.y -= 2;
	}
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		hp -= 2;
	}
	*/

	return true;
}

void Soldier::Draw()
{
	if (npc_id == 1)
	{
		if (hp > 0)
		{
			if (dir == UP)
			{
				App->render->Blit(texture, position.x, position.y - marge, &soldier_up);
			}
			if (dir == DOWN)
			{
				App->render->Blit(texture, position.x, position.y - marge, &soldier_down);
			}
			if (dir == RIGHT)
			{
				App->render->Blit(texture, position.x, position.y - marge, &soldier_right);
			}
			if (dir == LEFT)
			{
				App->render->Blit(texture, position.x, position.y - marge, &soldier_left);
			}
		}
	}
	if (npc_id == 2)
	{
		if (hp > 0)
		{
			if (dir == UP)
			{
				App->render->Blit(texture, position.x, position.y - marge, &soldier_up_2);
			}
			if (dir == DOWN)
			{
				App->render->Blit(texture, position.x, position.y - marge, &soldier_down_2);
			}
			if (dir == RIGHT)
			{
				App->render->Blit(texture, position.x, position.y - marge, &soldier_right_2);
			}
			if (dir == LEFT)
			{
				App->render->Blit(texture, position.x, position.y - marge, &soldier_left_2);
			}
		}
	}

}

void Soldier::AddItem(Item* item)
{
	item_inside = item;
	item->canBlit = false;
}

void Soldier::Drop_item()
{
	item_inside->canBlit = true;
	item_inside->position.x = position.x;
	item_inside->position.y = position.y;
	item_inside = NULL;
}

bool Soldier::CleanUp()
{
	return true;
}




