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
	texmapfile_name = conf.child("day-night").attribute("file").as_string("");
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
	changeResolution = false;
	player_texture = App->tex->Load(tex_player_file_name.c_str());
	maptex = App->tex->Load(texmapfile_name.c_str());
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
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		changeResolution = !changeResolution;
	}

	App->render->Blit(player_texture, pos.x, pos.y, &Rect_player);
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
		SDL_RenderSetLogicalSize(App->render->renderer, 200, 300);
		if (changeResolution)
			SDL_SetWindowSize(App->win->window, 200, 300);
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

	return ret;
}

bool j1Player::CleanUp()
{
	bool ret = true;


	return ret;
}
