#include "Blaziken.h"


Blaziken::Blaziken()
{
	type = CREATURE;
	name = "Link";
}

Blaziken::~Blaziken()
{
}

bool Blaziken::Awake(pugi::xml_node &conf)
{
	hp = conf.child("stats").attribute("hp").as_int(0);
	attack = conf.child("stats").attribute("attack").as_int(0);
	speed = conf.child("stats").attribute("speed").as_int(0);
	position.x = conf.child("stats").attribute("pos_x").as_int(0);
	position.y = conf.child("stats").attribute("pos_y").as_int(0);
	return true;
}

bool Blaziken::Start()
{
	direction = LEFT;
	state = IDLE;
	scale = App->win->GetScale();
	width = 15;
	height = 15;
	gamestate = TIMETOPLAY;
	timetoplay = SDL_GetTicks();

	collision_enemy = App->collision->AddCollider({ position.x, position.y, 15, 15 }, COLLIDER_PLAYER, this);

	return true;
}

bool Blaziken::Update(float dt)
{
	// STATE MACHINE ------------------
	if (gamestate == INGAME)
	{
		switch (state)
		{
		case IDLE:
		{
			Idle();
			break;
		}
		case WALKING:
		{
			Walking();
			break;
		}
		case ATTACKING:
		{
			Attack();
			break;
		}
		default:
		{
			break;
		}

		}
	}
	else if (gamestate == INMENU)
	{

	}
	else if (gamestate == TIMETOPLAY)
	{
		if (SDL_GetTicks() - timetoplay > 1000)
		{
			gamestate = INGAME;
		}
	}

	//Collision follow the player
	collision_enemy->SetPos(position.x, position.y);
	return true;
}

void Blaziken::Draw()
{
	App->anim_manager->Drawing_Manager(state, direction, position, 0);  //TODO LOW-> ID magic number, need change!!

}

bool Blaziken::CleanUp()
{
	return true;
}

void Blaziken::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		if (c1 == collision_enemy && c2->type == COLLIDER_ENEMY)
		{

		}
	}
}

bool Blaziken::Idle()
{
	//IDLE BLAZIKEN
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_REPEAT)
	{
		state = WALKING;
		CheckOrientation();
	}

	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		/*state = ATTACKING;
		current_animation = App->anim_manager->GetAnimation(state, direction, 0);
		current_animation->Reset();*/
	}

	else
	{
		state = IDLE;
	}
	return true;
}

bool Blaziken::Walking()
{
	walking = false;
	Move();

	if (walking == false)
	{
		state = IDLE;
	}

	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		/*state = ATTACKING;
		current_animation = App->anim_manager->GetAnimation(state, direction, 0);
		current_animation->Reset();*/
	}

	else
	{
		state = WALKING;
	}
	return false;
}

bool Blaziken::Move()
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_REPEAT)
	{
		direction = LEFT;
		int temp = App->map->MovementCost(position.x - speed, position.y, LEFT);
		if (temp == 0)
		{
			position.x -= speed;
		}

		walking = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT)
	{
		direction = DOWN;
		int temp = App->map->MovementCost(position.x, position.y + (speed + height), DOWN);
		if (temp == 0)
		{
			position.y += speed;
		}

		walking = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_REPEAT)
	{
		direction = RIGHT;
		int temp = App->map->MovementCost(position.x + (speed + width), position.y, RIGHT);
		if (temp == 0)
		{
			position.x += speed;
		}
		walking = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_REPEAT)
	{
		direction = UP;
		int temp = App->map->MovementCost(position.x, position.y - speed, UP);
		if (temp == 0)
		{
			position.y -= speed;
		}
		walking = true;
	}
	return walking;
}

bool Blaziken::Attack()
{
	return true;
}

bool Blaziken::CheckOrientation()
{
	return true;
}

void Blaziken::OnInputCallback(INPUTEVENT, EVENTSTATE)
{
}
