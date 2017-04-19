#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "Pokemon.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "p2Log.h"

j1Collision::j1Collision()
{
	name = "collision";

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ITEM] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_BOMB] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_SWITCH_MAP] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_DYNOBJECT] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_POKEMON] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_TRAINER] = true;

	matrix[COLLIDER_SWORD][COLLIDER_POKEMON] = true;
	matrix[COLLIDER_SWORD][COLLIDER_DYNOBJECT] = true;

	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_SWORD] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_BOMB] = true;
	matrix[COLLIDER_ENEMY][COLLIDER_ENEMY] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_DYNOBJECT] = false;
	matrix[COLLIDER_ENEMY][COLLIDER_SWITCH_MAP] = false;

	matrix[COLLIDER_POKEMON][COLLIDER_SWITCH_MAP] = false;
	matrix[COLLIDER_POKEMON][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_POKEMON][COLLIDER_BOMB] = true;
	matrix[COLLIDER_POKEMON][COLLIDER_POKEMON] = false;
	matrix[COLLIDER_POKEMON][COLLIDER_POKEMON_ATTACK] = true;
	matrix[COLLIDER_POKEMON][COLLIDER_SWORD] = true;

	matrix[COLLIDER_POKEMON_ATTACK][COLLIDER_POKEMON] = false;
}
j1Collision::~j1Collision() {}

bool j1Collision::Awake()
{
	return true;
}

bool j1Collision::Start()
{
	waittodelete = false;
	return true;
}

bool j1Collision::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			if(num_colliders > 1)
				num_colliders--;
		}
	}
	waittodelete = false;
	return true;
}

bool j1Collision::Update(float dt)
{
	BROFILER_CATEGORY("Update_Collision", Profiler::Color::Orange)
	// Called before render is available
	Collider* c1 = nullptr;
	Collider* c2 = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];


		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2) == true)
			{
				if (matrix[c1->type][c2->type] == true && c1->callback != nullptr && waittodelete == false)
				{
					c1->callback->OnCollision(c1, c2);
				}

				if (matrix[c2->type][c1->type] == true && c2->callback != nullptr && waittodelete == false)
				{
					c2->callback->OnCollision(c2, c1);
				}
			}
		}
	}

	return true;
}

bool j1Collision::PostUpdate()
{
	DebugDraw();
	return true;
}

bool j1Collision::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
	return true;
}


void j1Collision::DebugDraw()
{

	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_PLAYER: // white
		{
			if (App->scene->combat == false)
				App->render->DrawQuad(colliders[i]->rect, 0, 180, 125, alpha);
			break;
		}
		case COLLIDER_SWORD: // white
		{
			App->render->DrawQuad(colliders[i]->rect, 0, 180, 125, alpha);
			break;
		}

		case COLLIDER_ENEMY: // white
			App->render->DrawQuad(colliders[i]->rect, 177, 255, 140, alpha);
			break;
		case COLLIDER_DYNOBJECT: // white
			App->render->DrawQuad(colliders[i]->rect, 177, 255, 140, alpha);
			break;
		case COLLIDER_ITEM: // white
			App->render->DrawQuad(colliders[i]->rect, 120, 50, 140, alpha);
			break;
		case COLLIDER_HOOKSHOT: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_BOMB: //RED
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);
			break;
		case COLLIDER_SWITCH_MAP: // Green
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_POKEMON: // Green
		{
			Pokemon* temp = (Pokemon*)colliders[i]->callback;
			if (temp->active || App->scene->combat == false)
				App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		}

		case COLLIDER_TRAINER: // Pink
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
			break;

		case COLLIDER_POKEMON_ATTACK:
			App->render->DrawQuad(colliders[i]->rect, 125, 0, 50, alpha);
			break;
		}
	}
}


Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, SceneElement* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new ColliderRect(rect, type, callback);
			if (colliders[i]->to_delete == true)
			{
				colliders[i]->to_delete = false;
			}
			num_colliders++;
			break;
		}
	}

	return ret;
}


bool j1Collision::EraseCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			if (num_colliders > 1)
				num_colliders--;
			return true;
		}
	}
	return false;
}

void j1Collision::EreseAllColiderPlayer()
{
	for (uint i = 1; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			colliders[i]->to_delete = true;
		}
	}
}

// -----------------------------------------------------

bool ColliderRect::CheckCollision(const Collider* c) const
{

	return (rect.x < c->rect.x + c->rect.w &&
		rect.x + rect.w > c->rect.x &&
		rect.y < c->rect.y + c->rect.h &&
		rect.h + rect.y > c->rect.y);
}
