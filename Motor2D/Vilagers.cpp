#include "Vilagers.h"
#include "j1EntityElementsScene.h"

Vilager::Vilager()
{
}

Vilager::~Vilager()
{
}

bool Vilager::Awake(pugi::xml_node& conf)
{
	position.x = conf.attribute("pos_x").as_int(0);
	position.y = conf.attribute("pos_y").as_int(0);
	name = conf.attribute("name").as_string("");
	active = conf.attribute("active").as_bool(false);

	dialog = conf.attribute("dialog").as_string(" ERROR LOAD DIALOG ");

	return true;
}

bool Vilager::Start()
{
	direction = DOWN;
	state = V_IDLE;
	anim_state = V_IDLE;

	if (active)
		collision_feet = App->collision->AddCollider({ position.x, position.y, 15, 21 }, COLLIDER_TRAINER, this);

	return true;
}

bool Vilager::Update(float dt)
{
	return false;
}

void Vilager::Draw()
{
	if (active)
	{
		if (direction == UP)
		{
			SDL_Rect r = { 1,1,14,21 };
			App->render->Blit(App->entity_elements->texture_trainer, position.x, position.y, &r);
		}
		if (direction == DOWN)
		{
			SDL_Rect r = { 46,1,14,21 };
			App->render->Blit(App->entity_elements->texture_trainer, position.x, position.y, &r);
		}
		if (direction == LEFT)
		{
			SDL_Rect r = { 31,23,14,21 };
			App->render->Blit(App->entity_elements->texture_trainer, position.x, position.y, &r);
		}
		if (direction == RIGHT)
		{
			SDL_Rect r = { 16,45,14,21 };
			App->render->Blit(App->entity_elements->texture_trainer, position.x, position.y, &r);
		}
	}
}

bool Vilager::CleanUp()
{
	return false;
}
