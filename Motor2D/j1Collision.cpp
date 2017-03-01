#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "p2Log.h"

j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_WALL][COLLIDER_SPHERE_RIGHT] = true;
	matrix[COLLIDER_WALL][COLLIDER_SPHERE_LEFT] = true;
	matrix[COLLIDER_WALL][COLLIDER_LATERAL_WALL] = false;

	matrix[COLLIDER_PLAYER][COLLIDER_ENEMY] = true;

	matrix[COLLIDER_ENEMY][COLLIDER_PLAYER] = true;


	matrix[COLLIDER_SPHERE_RIGHT][COLLIDER_WALL] = true;
	matrix[COLLIDER_SPHERE_RIGHT][COLLIDER_SPHERE_RIGHT] = true;
	matrix[COLLIDER_SPHERE_RIGHT][COLLIDER_LATERAL_WALL] = true;
	matrix[COLLIDER_SPHERE_RIGHT][COLLIDER_SPHERE_LEFT] = true;

	matrix[COLLIDER_LATERAL_WALL][COLLIDER_WALL] = false;
	matrix[COLLIDER_LATERAL_WALL][COLLIDER_SPHERE_RIGHT] = true;
	matrix[COLLIDER_LATERAL_WALL][COLLIDER_SPHERE_LEFT] = true;
	matrix[COLLIDER_LATERAL_WALL][COLLIDER_LATERAL_WALL] = false;

	matrix[COLLIDER_SPHERE_LEFT][COLLIDER_SPHERE_LEFT] = true;
	matrix[COLLIDER_SPHERE_LEFT][COLLIDER_SPHERE_RIGHT] = false;
	matrix[COLLIDER_SPHERE_LEFT][COLLIDER_WALL] = true;
	matrix[COLLIDER_SPHERE_LEFT][COLLIDER_LATERAL_WALL] = true;

}
j1Collision::~j1Collision() {}

bool j1Collision::Awake()
{
	return true;
}

bool j1Collision::Start()
{
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
		}
	}
	return true;
}

bool j1Collision::Update(float dt)
{
	// Called before render is available
	Collider* c1;
	Collider* c2;

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
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
			}
		}
	}
	DebugDraw();

	return true;
}

bool j1Collision::PostUpdate()
{
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
			App->render->DrawQuad(colliders[i]->rect, 255, 180, 125, alpha);
			break;
		case COLLIDER_ENEMY: // white
			App->render->DrawQuad(colliders[i]->rect, 177, 255, 140, alpha);
			break;
		}
	}
}


Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1SceneElement* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new ColliderRect(rect, type, callback);
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
			return true;
		}
	}
	return false;
}

// -----------------------------------------------------

bool ColliderRect::CheckCollision(const Collider* c) const
{

	return (rect.x < c->rect.x + c->rect.w &&
		rect.x + rect.w > c->rect.x &&
		rect.y < c->rect.y + c->rect.h &&
		rect.h + rect.y > c->rect.y);
}