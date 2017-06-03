#include "Sudowoodo.h"

Sudowoodo::Sudowoodo()
{
	type = CREATURE;
}

Sudowoodo::~Sudowoodo()
{
}

bool Sudowoodo::Awake(pugi::xml_node &conf, uint id)
{
	name = conf.attribute("name").as_string("");
	hp = conf.attribute("hp").as_int(0);
	attack = conf.attribute("name").as_int(0);
	speed = conf.attribute("name").as_int(0);
	std::string temp = conf.attribute("dir").as_string("");
	if (temp == "up")
		direction = UP;
	else if (temp == "down")
		direction = DOWN;
	else if (temp == "left")
		direction = LEFT;
	else
		direction = RIGHT;

	position = iPoint(conf.attribute("pos_x").as_int(0), conf.attribute("pos_y").as_int(0));

	return true;
}

bool Sudowoodo::Start()
{
	state = P_IDLE;
	anim_state = P_IDLE;
	//Get the animations
	animation = *App->anim_manager->GetAnimStruct(SUDOWOODO); //id 9 = Sudowoodo
	return true;
}

bool Sudowoodo::Update(float dt)
{
	BROFILER_CATEGORY("Update_Collision", Profiler::Color::Khaki);
	return true;
}

void Sudowoodo::Draw()
{
	BROFILER_CATEGORY("Draw_Sudowoodo", Profiler::Color::Yellow)
		//App->anim_manager->Drawing_Manager(state, direction, position, 6);
		int id;
	switch (state)
	{
	case P_IDLE:
		id = 0;
		break;
	default:
		break;
	}


	if (direction == DOWN)
	{
		anim_rect = animation.anim[id].South_action.GetCurrentFrame();
		pivot = animation.anim[id].South_action.GetCurrentOffset();
	}

	//DRAW
	App->render->Blit(animation.graphics, position.x - pivot.x, position.y - pivot.y, &anim_rect);
}

bool Sudowoodo::CleanUp()
{
	return true;
}