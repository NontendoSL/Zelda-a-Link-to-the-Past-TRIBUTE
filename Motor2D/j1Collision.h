#ifndef __j1Collision_H__
#define __j1Collision_H__

#include "j1Module.h"
#include "SDL/include/SDL_rect.h"
#include "SceneElements.h"

#define MAX_COLLIDERS 400

struct SDL_Rect;
class Arrow;

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
	COLLIDER_TRANINGDOLL,
	COLLIDER_POKECOMBAT,
	COLLIDER_POKEMON_ATTACK,
	COLLIDER_POKEMON_SPECIAL_ATTACK,
	COLLIDER_SWORD,
	COLLIDER_BCTROOPER,
	COLLIDER_BCTROOPER_MAZE,
	COLLIDER_VILAGER,
	COLLIDER_GANON,
	COLLIDER_GANON_ATTACK,
	COLLIDER_GMINION,
	COLLIDER_RMINION,
	COLLIDER_FIREBAT,
	COLLIDER_PC,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect;

	bool to_delete = false;
	COLLIDER_TYPE type;
	SceneElement* callback = nullptr;
	Arrow* arrow_callback = nullptr;


	Collider(COLLIDER_TYPE type, SceneElement* callback = nullptr, Arrow* arrow_callback = nullptr) :
		type(type),callback(callback),arrow_callback(arrow_callback){}

	virtual bool CheckCollision(const Collider* c) const = 0;
	virtual void SetPos(int x, int y) = 0;
};

struct ColliderRect : public Collider
{
	ColliderRect(SDL_Rect rectangle, COLLIDER_TYPE type, SceneElement* callback = nullptr, Arrow* a_callback = nullptr) : Collider(type, callback, a_callback)
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

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, SceneElement* callback = nullptr, Arrow* a_callback = nullptr);
	bool EraseCollider(Collider* collider);
	void EreseAllColiderPlayer();
	void DebugDraw();
	bool Getdebug() const;

	uint num_colliders = 0;
	bool waittodelete;

private:

	std::vector<Collider*> colliders;
	bool matrix[MAX_COLLIDERS][MAX_COLLIDERS];
	bool debug = false;
};


#endif