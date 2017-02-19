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
#include "j1Player.h"

//Constructor
j1Player::j1Player() : j1Module()
{
	name="player";
}

// Destructor
j1Player::~j1Player()
{}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& conf)
{
	LOG("Loading Texture Player");
	bool ret = true;

	tex_player_file_name = conf.child("atlas").attribute("file").as_string("");
	Rect_player = { 984,189,66,90 };
	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	bool ret = true;
	pos.x = 500;
	pos.y = 200;
	Camera_follow_player = true;
	player_texture = App->tex->Load(tex_player_file_name.c_str());
	return ret;
}

bool j1Player::Update(float dt)
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Camera_follow_player = !Camera_follow_player;
	if (App->input->GetKey(SDL_SCANCODE_LCTRL) != KEY_REPEAT)
	{
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			if (Camera_follow_player)
				App->render->camera.x += 2;
			pos.x -= 2;
		}
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			if (Camera_follow_player)
				App->render->camera.y -= 2;
			pos.y += 2;
		}
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			if (Camera_follow_player)
				App->render->camera.x -= 2;
			pos.x += 2;
		}
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			if (Camera_follow_player)
				App->render->camera.y += 2;
			pos.y -= 2;
		}
	}


	App->render->Blit(player_texture, pos.x, pos.y, &Rect_player);

	return ret;
}

bool j1Player::CleanUp()
{
	bool ret = true;


	return ret;
}
