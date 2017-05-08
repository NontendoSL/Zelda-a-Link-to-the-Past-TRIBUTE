#include "Vilager.h"
#include "j1EntityElementsScene.h"
#include "j1Collision.h"
#include "j1Gui.h"

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
	active = conf.attribute("active").as_bool(true);
	dialog = conf.attribute("dialog").as_string("");
	rect = { conf.attribute("rect_x").as_int(0), conf.attribute("rect_y").as_int(0), conf.attribute("rect_w").as_int(0), conf.attribute("rect_h").as_int(0) };
	switch_map = conf.attribute("switch_map").as_int(0);
	return true;
}

bool Vilager::Start()
{
	direction = DOWN; //maybe in levels.xml
	state = V_IDLE;
	anim_state = V_IDLE;

	if (active)
		collision_feet = App->collision->AddCollider({ position.x, position.y, 15, 21 }, COLLIDER_VILAGER, this);

	return true;
}

bool Vilager::Update(float dt)
{
	return true;
}

void Vilager::Draw()
{
	if (active)
	{
		/*if (direction == UP)
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
		}*/
		App->render->Blit(App->entity_elements->texture_trainer, position.x, position.y, &rect);
	}
}

bool Vilager::CleanUp()
{
	return true;
}

std::string Vilager::GetDialog()
{
	return dialog;
}


//	App->gui->CreateDialogue(dialog.c_str());