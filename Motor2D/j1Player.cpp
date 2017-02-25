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

//Constructor
Player::Player(iPoint position) :j1SceneElement(position)
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
	texmapfile_name = conf.child("day-night").attribute("file").as_string("");
	hp = conf.child("stats").attribute("hp").as_int(0);
	attack = conf.child("stats").attribute("attack").as_int(0);
	Rect_player = { 984,189,66,90 };

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

	width = 17;
	height = 25;
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

	return ret;
}

bool Player::Update()//TODO HIGH -> I delete dt but i thing that we need.
{

	bool ret = true;
	//TEST MOVE LINK
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Camera_follow_player = !Camera_follow_player;
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) != KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (App->map->MovementCost(position.x - 2, position.y) == 0)
			{
				if (Camera_follow_player)
					App->render->camera.x += 2;
				position.x -= 2;
				state = WALKING;
				dir = LEFT;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			if (App->map->MovementCost(position.x, position.y + (2 + height)) == 0)
			{
				if (Camera_follow_player)
					App->render->camera.y -= 2;
				position.y += 2;
				state = WALKING;
				dir = DOWN;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (App->map->MovementCost(position.x + (2 + width), position.y) == 0)
			{
				if (Camera_follow_player)
					App->render->camera.x -= 2;
				position.x += 2;
				state = WALKING;
				dir = RIGHT;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			if (App->map->MovementCost(position.x, position.y - 2) == 0)
			{
				if (Camera_follow_player)
					App->render->camera.y += 2;
				position.y -= 2;
				state = WALKING;
				dir = UP;
			}
		}
	}

	

	//TEST CHANGE RESOLUTION AND SIZE OF SCREEN
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
		if(changeResolution)
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



	//TEST DRAW LIVE OF LINK 
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		hp -= 2;
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
		attack *= 2;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
		Save();

	//TODO MID -> I thing that use a stats_temp all time clear and insert, clear-insert, is not a good idea...
	/*stats_temp.clear();
	stats_temp = std::to_string(hp);
	stats_temp.insert(0, "HP LINK -> ");
	hp_text->Write(stats_temp.c_str());
	stats_temp.clear();
	stats_temp = std::to_string(attack);
	stats_temp.insert(0, "ATTACK OF LINK -> ");
	attack_text->Write(stats_temp.c_str());*/

	
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