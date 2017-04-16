#ifndef __j1Collision_H__
#define __j1Collision_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include "SceneElements.h"

#define MAX_COLLIDERS 200

struct SDL_Rect;

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_PLAYER,
	COLLIDER_ENEMY,
	COLLIDER_ITEM,
	COLLIDER_DYNOBJECT,
	COLLIDER_HOOKSHOT,
	COLLIDER_ARROW,
	COLLIDER_BOMB,
	COLLIDER_SWITCH_MAP,
	COLLIDER_POKEMON,
	COLLIDER_TRAINER,
	COLLIDER_POKEMON_ATTACK,
	COLLIDER_SWORD,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;

	bool to_delete = false;
	COLLIDER_TYPE type;
	SceneElement* callback = nullptr;

	Collider(COLLIDER_TYPE type, SceneElement* callback = nullptr) :type(type),callback(callback){}

	virtual bool CheckCollision(const Collider* c) const = 0;
	virtual void SetPos(int x, int y) = 0;
};

struct ColliderRect : public Collider
{
	ColliderRect(SDL_Rect rectangle, COLLIDER_TYPE type, SceneElement* callback = nullptr) : Collider(type, callback)
	{
		rect = rectangle;
	}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const Collider* c) const;

};


class j1Collision : public j1Module
{
public:

	j1Collision();
	~j1Collision();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, SceneElement* callback = nullptr);
	bool EraseCollider(Collider* collider);
	void EreseAllColiderPlayer();
	void DebugDraw();

	uint num_colliders = 0;
	bool waittodelete;

private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};


#endif