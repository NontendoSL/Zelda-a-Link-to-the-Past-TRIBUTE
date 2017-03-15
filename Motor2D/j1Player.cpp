#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "j1GuiElements.h"
#include "j1Player.h"
#include "j1FileSystem.h"
#include "j1AnimationManager.h"
#include "j1Collision.h"

//Constructor
Player::Player() :SceneElement()
{

	type = PLAYER;
	name = "player";
}

// Destructor
Player::~Player()
{}

// Called before render is available
bool Player::Awake(pugi::xml_node& conf)
{
	LOG("Loading Texture Player");
	bool ret = true;

	tex_player_file_name = conf.child("atlas").attribute("file").as_string("");
	file_hit = conf.child("hit").attribute("file").as_string("");
	texmapfile_name = conf.child("day-night").attribute("file").as_string("");
	hp = conf.child("stats").attribute("hp").as_int(0);
	attack = conf.child("stats").attribute("attack").as_int(0);
	speed = conf.child("stats").attribute("speed").as_int(0);
	position.x = conf.child("stats").attribute("pos_x").as_int(0);
	position.y = conf.child("stats").attribute("pos_y").as_int(0);

	return ret;
}

// Called before the first frame
bool Player::Start()
{
	bool ret = true;
	Camera_follow_player = true;
	changeResolution = false;
	player_texture = App->tex->Load(tex_player_file_name.c_str());
	maptex = App->tex->Load(texmapfile_name.c_str());
	hit_tex = App->tex->Load(file_hit.c_str());
	dir = UP;
	
	scale = App->win->GetScale();
	width = 15;
	height = 15;
	//TEST TAKE STATS BY CONFIG.XML AND IMPLEMENTED IN GAME
	/*stats_temp.clear();
	stats_temp = std::to_string(hp);
	stats_temp.insert(0, "HP LINK -> ");
	hp_text = App->gui->CreateText(stats_temp.c_str(), { 300,150 }, 23);
	stats_temp.clear();//Delete string temp
	stats_temp = std::to_string(attack);
	stats_temp.insert(0, "ATTACK OF LINK -> ");
	attack_text = App->gui->CreateText(stats_temp.c_str(), { 300,200 }, 23);
	*/

	collision_player = App->collision->AddCollider({ position.x, position.y, 15, 15 }, COLLIDER_PLAYER, this);

	return ret;
}

bool Player::Update()//TODO HIGH -> I delete dt but i thing that we need.
{
	bool ret = true;
	//TEST MOVE LINK
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Camera_follow_player = !Camera_follow_player;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP)
	{
		state = IDLE;
		dir = LEFT;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP)
	{
		state = IDLE;
		dir = RIGHT;
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_UP)
	{
		state = IDLE;
		dir = UP;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		state = IDLE;
		dir = DOWN;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		if (App->map->MovementCost(position.x - speed, position.y, LEFT) == 0)
		{
			state = WALKING;
			dir = LEFT;
			if (Camera_inside())
				App->render->camera.x += speed * scale;
			position.x -= speed;
			state = WALKING;
			dir = LEFT;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		if (App->map->MovementCost(position.x, position.y + (speed + height), DOWN) == 0)
		{
			state = WALKING;
			dir = DOWN;
			if (Camera_inside())
				App->render->camera.y -= speed * scale;
			position.y += speed;
		}
		state = WALKING;
		dir = DOWN;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		if (App->map->MovementCost(position.x + (speed + width), position.y, RIGHT) == 0)
		{
			state = WALKING;
			dir = RIGHT;
			if (Camera_inside())
				App->render->camera.x -= speed * scale;
			position.x += speed;
		}
		state = WALKING;
		dir = RIGHT;
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		if (App->map->MovementCost(position.x, position.y - speed, UP) == 0)
		{
			state = WALKING;
			dir = UP;
			if (Camera_inside())
				App->render->camera.y += speed * scale;
			position.y -= speed;
		}
		state = WALKING;
		dir = UP;
	}
	/*if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{

		App->render->camera.h;
			switch (dir) 
			{
			case UP:
				if (App->map->MovementCost(position.x, position.y - speed, UP) == 2)
				{
					App->map->EditCost(position.x + 8, position.y - speed, 0);
				}
				break;
			case DOWN:
				if (App->map->MovementCost(position.x, position.y + (speed + height), DOWN) == 2)
				{
					App->map->EditCost(position.x + 8, position.y + (speed + height), 0);
				}
				break;
			case LEFT:
				if (App->map->MovementCost(position.x - speed, position.y, UP) == 2)
				{
					App->map->EditCost(position.x - speed, position.y + 8, 0);
				}
				break;
			case RIGHT:
				if (App->map->MovementCost(position.x + (speed + width), position.y , UP) == 2)
				{
					App->map->EditCost(position.x , position.y + 8, 0);
				}
				break;
			}
	}*/

	//Provisional gem provider
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && gems<999)
	{
		gems++;
	}
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT && bombs<99)
	{
		bombs++;
	}
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT && arrows<99)
	{
		arrows++;
	}
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		App->scene->dialog->PushLine(true);
	}

	/*//TEST CHANGE RESOLUTION AND SIZE OF SCREEN
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		changeResolution = !changeResolution;
	}

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	{
		App->render->Blit(maptex, -App->render->camera.x, -App->render->camera.y);
	}
	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		SDL_RenderSetLogicalSize(App->render->renderer, 800, 400);
		if (changeResolution)
			SDL_SetWindowSize(App->win->window, 800, 400);
	}
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		SDL_RenderSetLogicalSize(App->render->renderer, 300, 300);
		if (changeResolution)
			SDL_SetWindowSize(App->win->window, 300, 300);
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		SDL_RenderSetLogicalSize(App->render->renderer, 500, 200);
		if (changeResolution)
			SDL_SetWindowSize(App->win->window, 500, 200);
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		SDL_RenderSetLogicalSize(App->render->renderer, 1200, 500);
		if (changeResolution)
			SDL_SetWindowSize(App->win->window, 1200, 500);
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
	{
		SDL_RenderSetLogicalSize(App->render->renderer, 1300, 800);
		if (changeResolution)
			SDL_SetWindowSize(App->win->window, 1300, 800);
	}
	if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		SDL_SetWindowSize(App->win->window, 800, 300);
	}
	*/


	/*//TEST DRAW LIVE OF LINK 
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		hp -= 2;
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		attack *= 2;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		Save();*/

	//TODO MID -> I thing that use a stats_temp all time clear and insert, clear-insert, is not a good idea...
	/*stats_temp.clear();
	stats_temp = std::to_string(hp);
	stats_temp.insert(0, "HP LINK -> ");
	hp_text->Write(stats_temp.c_str());
	stats_temp.clear();
	stats_temp = std::to_string(attack);
	stats_temp.insert(0, "ATTACK OF LINK -> ");
	attack_text->Write(stats_temp.c_str());*/

	//Collision follow the Ms Pac Man
	collision_player->SetPos(position.x, position.y);

	return ret;
}

void Player::Draw()
{
	App->anim_manager->PlayerSelector(state, dir, position);
	//App->render->Blit(player_texture, position.x, position.y/*, &Rect_player);
}

bool Player::CleanUp()
{
	bool ret = true;


	return ret;
}


bool Player::Save()
{
	App->entity_elements->XML.child("config").child("player").child("stats").attribute("hp").set_value(hp);

	App->entity_elements->XML.save_file("config.xml");
	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == collision_player && c2->type == COLLIDER_ENEMY)
	{
		if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y + 1 >= c2->rect.y + c2->rect.h)
		{
			App->render->camera.y -= speed * 2;
			position.y = position.y + 1;
		}
		else if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h - 1 <= c2->rect.y)
		{
			App->render->camera.y += speed * 2;
			position.y = position.y - 1;
		}
		else if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w - 1 <= c2->rect.x)
		{
			App->render->camera.x += speed * 2;
			position.x = position.x - 1;
		}
		else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x + 1 >= c2->rect.x + c2->rect.w)
		{
			App->render->camera.x -= speed * 2;
			position.x = position.x + 1;
		}
	}
}

bool Player::Camera_inside()
{
	//256x224
	iPoint temp = App->map->MapToWorld(App->map->data.height, App->map->data.width);
	if (dir == UP)
	{
		if (position.y < temp.y - (224 / 2))
		{
			if ((0 < -(App->render->camera.y + 2) && temp.y + 64 > -(App->render->camera.y + 2)) == false)
			{
				return false;
			}
		}
		else
			return false;
	}
	if (dir == DOWN)
	{
		if (position.y > 110)
		{
			if ((0 < -(App->render->camera.y - 2) && temp.y + 64 > -(App->render->camera.y - 2)) == false)
			{
				return false;
			}
		}
		else
			return false;

	}
	if (dir == LEFT)
	{
		if (position.x < temp.x - 130)
		{
			if ((0 < -(App->render->camera.x + 2) && temp.x > -(App->render->camera.x + 2)) == false)
			{
				return false;
			}
		}
		else
			return false;
	}
	if (dir == RIGHT)
	{
		if (position.x > 128)
		{
			if ((0 < -(App->render->camera.x - 2) && temp.x > -(App->render->camera.x - 2)) == false)
			{
				return false;
			}
		}
		else
			return false;
	}
	return true;
}