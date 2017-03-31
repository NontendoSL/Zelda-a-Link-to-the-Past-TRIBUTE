#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Scene.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "j1GuiElements.h"
#include "j1Player.h"
#include "j1FileSystem.h"
#include "j1AnimationManager.h"
#include "j1Collision.h"
#include "j1InputManager.h"
#include "Soldier.h"
#include "j1Item.h"
#include "j1DynamicObjects.h"
#include "j1Creature.h"
#include "j1Weapon.h"
#include "Animation.h"

//Constructor
Player::Player() : Creature()
{
	type = CREATURE;
	name = "Link";
}

// Destructor
Player::~Player()
{}

// Called before render is available
bool Player::Awake(pugi::xml_node& conf)
{
	LOG("Loading Texture Player");
	bool ret = true;

	tex_player_file_name = conf.child("atlas").attribute("file").as_string("");
	file_hit = conf.child("hit").attribute("file").as_string("");
	texmapfile_name = conf.child("day-night").attribute("file").as_string("");
	hp = conf.child("stats").attribute("hp").as_int(0);
	attack = conf.child("stats").attribute("attack").as_int(0);
	speed = conf.child("stats").attribute("speed").as_int(0);
	position.x = conf.child("stats").attribute("pos_x").as_int(0);
	position.y = conf.child("stats").attribute("pos_y").as_int(0);


	return ret;
}

// Called before the first frame
bool Player::Start()
{
	bool ret = true;
	Camera_follow_player = true;
	changeResolution = false;
	attacker = false;
	player_texture = App->tex->Load(tex_player_file_name.c_str());
	maptex = App->tex->Load(texmapfile_name.c_str());
	hit_tex = App->tex->Load(file_hit.c_str());
	direction = UP;
	state = IDLE;
	scale = App->win->GetScale();
	width = 15;
	height = 15;
	gamestate = TIMETOPLAY;
	timetoplay = SDL_GetTicks();

	collision_player = App->collision->AddCollider({ position.x, position.y, 15, 15 }, COLLIDER_PLAYER, this);
	App->input_manager->AddListener(this);
	return ret;
}

bool Player::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdate_Player", Profiler::Color::RosyBrown)
	bool ret = true;

	return ret;
}

bool Player::Update()//TODO HIGH -> I delete dt but i thing that we need.
{
	BROFILER_CATEGORY("DoUpdate_Player", Profiler::Color::Red)
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		Camera_follow_player = !Camera_follow_player;

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
		case HOOKTHROWN:
		{
			Hooking();
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


	//CHARGE BAR --------------
	if (equiped_item == hook)
	{
		if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_REPEAT) && charge <= 34)
		{
			charge++;
		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		{
			state = HOOKTHROWN;
			ThrowHookshot(charge);
		}
		else if (charge > 0)
		{
			charge--;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_REPEAT && gems<999)
	{
		gems++;
	}
	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT && bombs<99)
	{
		bombs++;
	}
	if (App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT && arrows<99)
	{
		arrows++;
	}
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		//App->scene->dialog->PushLine(true);
	}
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) 
	{
		App->scene->switch_menu = true;
		gamestate = INMENU;
	}

	//Collision follow the player
	collision_player->SetPos(position.x, position.y);

	return ret;
}

void Player::Draw()
{
	//Draw player
	App->anim_manager->Drawing_Manager(state, direction, position, 0);  //TODO LOW-> ID magic number, need change!!
	int width, height;

	if (direction == UP || direction == DOWN)
	{
		width = 12;
		height = 8;
	}

	else if (direction == LEFT || direction == RIGHT)
	{
		width = 8;
		height = 12;
	}
	if (hook->in_use == true)
	{
		App->render->DrawQuad(SDL_Rect{ hook->position.x,hook->position.y, width, height }, 255, 255, 255);
	}
}

bool Player::CleanUp()
{
	bool ret = true;

	return ret;
}


bool Player::Save()
{
	App->entity_elements->XML.child("config").child("Link").child("stats").attribute("hp").set_value(hp);
	App->entity_elements->XML.save_file("config.xml");
	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 != nullptr && c2 != nullptr)
	{
		if (c1 == collision_attack && c2->type == COLLIDER_DYNOBJECT)
		{
			iPoint pos_dyn = App->map->WorldToMap(c2->callback->position.x, c2->callback->position.y);
			//srand(time(NULL)); 		int canDrop = rand() % 5 + 1;
			int canDrop = 1;
			if (canDrop == 1)
			{
				iPoint position;
				position.x = c2->callback->position.x + 4;
				position.y = c2->callback->position.y;
				App->scene->items.push_back(App->entity_elements->CreateItem(1, position));

			}
			if (c2->callback->name != "chest")
			{
				App->map->EditCost(pos_dyn.x, pos_dyn.y, App->map->data.tilesets[1]->firstgid);
				App->map->EditCost(pos_dyn.x + 1, pos_dyn.y, App->map->data.tilesets[1]->firstgid);
				App->map->EditCost(pos_dyn.x, pos_dyn.y + 1, App->map->data.tilesets[1]->firstgid);
				App->map->EditCost(pos_dyn.x + 1, pos_dyn.y + 1, App->map->data.tilesets[1]->firstgid);
			}

			App->entity_elements->DeleteDynObject((DynamicObjects*)c2->callback);
			//App->collision->EraseCollider(c2);
		}

		if (c1 == collision_player && c2->type == COLLIDER_ITEM)
		{
			if (c2->callback->name == "rupee")
			{
				gems++;
				App->entity_elements->DeleteItem((Item*)c2->callback);
				//App->collision->EraseCollider(c2);
			}
		}

		if (c1 == collision_attack && c2->type == COLLIDER_ENEMY)
		{
			Soldier* soldier = (Soldier*)c2->callback;
			soldier->hp--;
			if (soldier->hp == 0)
			{
				if (soldier->destructible)
				{
					c2->callback->state = DYING;
				}
			}
			else
			{
				if (soldier->destructible)
				{
					soldier->state = HIT;
					soldier->dir_hit = c1->callback->direction;
					soldier->previus_position = soldier->position;
				}
			}
		}
	}



	/*if (c1 == collision_player && c2->type == COLLIDER_ENEMY)
	{
		if (c1->rect.y <= c2->rect.y + c2->rect.h && c1->rect.y + 1 >= c2->rect.y + c2->rect.h)
		{
			App->render->camera.y -= speed * 2;
			position.y = position.y + 1;
		}
		else if (c1->rect.y + c1->rect.h >= c2->rect.y && c1->rect.y + c1->rect.h - 1 <= c2->rect.y)
		{
			App->render->camera.y += speed * 2;
			position.y = position.y - 1;
		}
		else if (c1->rect.x + c1->rect.w >= c2->rect.x && c1->rect.x + c1->rect.w - 1 <= c2->rect.x)
		{
			App->render->camera.x += speed * 2;
			position.x = position.x - 1;
		}
		else if (c1->rect.x <= c2->rect.x + c2->rect.w && c1->rect.x + 1 >= c2->rect.x + c2->rect.w)
		{
			App->render->camera.x -= speed * 2;
			position.x = position.x + 1;
		}
	}*/
}

bool Player::Camera_inside()
{
	//256x224
	iPoint temp = App->map->MapToWorld(App->map->data.width, App->map->data.height);
	if (direction == UP)
	{
		if (position.y < temp.y - (224 / 2))
		{
			if ((0 < -(App->render->camera.y + 2) && temp.y + 64 > -(App->render->camera.y + 2)) == false)
			{
				return false;
			}
		}
		else
			return false;
	}
	if (direction == DOWN)
	{
		if (position.y > 110)
		{
			if ((0 < -(App->render->camera.y - 2) && temp.y + 64 > -(App->render->camera.y - 2)) == false)
			{
				return false;
			}
		}
		else
			return false;

	}
	if (direction == LEFT)
	{
		if (position.x < temp.x - 130)
		{
			if ((0 < -(App->render->camera.x + 2) && temp.x > -(App->render->camera.x + 2)) == false)
			{
				return false;
			}
		}
		else
			return false;
	}
	if (direction == RIGHT)
	{
		if (position.x > 128)
		{
			if ((0 < -(App->render->camera.x - 2) && temp.x > -(App->render->camera.x - 2)) == false)
			{
				return false;
			}
		}
		else
			return false;
	}
	return true;
}

bool Player::Idle()
{
	//TEST MOVE LINK
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
		state = ATTACKING;
		current_animation = App->anim_manager->GetAnimation(state, direction, 0);
		current_animation->Reset();
	}

	else
	{
		state = IDLE;
	}

	return true;
}

bool Player::Walking()
{
	walking = false;
	Move();

	if (walking == false)
	{
		state = IDLE;
	}

	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		state = ATTACKING;
		current_animation = App->anim_manager->GetAnimation(state, direction, 0);
		current_animation->Reset();
	}

	else
	{
		state = WALKING;
	}
	return false;
}

bool Player::Attack()
{
	if (attacker)
	{
		if (current_animation->Finished())
		{
			App->collision->EraseCollider(collision_attack);
			attacker = false;
			current_animation->Reset();
			current_animation = nullptr;
			state = IDLE;
		}
	}
	else
	{
		attacker = true;
		if (direction == UP)
		{
			collision_attack = App->collision->AddCollider({ position.x + 4, position.y - 16, 8, 20 }, COLLIDER_PLAYER, this);
		}
		else if (direction == RIGHT)
		{
			collision_attack = App->collision->AddCollider({ position.x + 9, position.y + 4, 20, 8 }, COLLIDER_PLAYER, this);
		}
		else if (direction == DOWN)
		{
			collision_attack = App->collision->AddCollider({ position.x + 4, position.y + 5, 8, 20 }, COLLIDER_PLAYER, this);
		}
		else if (direction == LEFT)
		{
			collision_attack = App->collision->AddCollider({ position.x - 13, position.y + 4, 20, 8 }, COLLIDER_PLAYER, this);
		}
	}
	return true;
}

bool Player::Equip(Weapon* to_equip)
{
	bool ret = false;
	if (equiped_item != to_equip && to_equip->equipable == true)
	{
		equiped_item = to_equip;
		equiped_item->equiped = true;
		LOG("Equiped %s", equiped_item->name.c_str());
		ret = true;
	}
	else
	{
		LOG("Can't equip %s", to_equip->name.c_str());
	}
	return ret;
}

bool Player::Unequip()
{
	bool ret = false;
	if (equiped_item == nullptr)
	{
		LOG("Nothing equiped");
	}
	else
	{
		LOG("Unequiped %s", equiped_item->name.c_str());
		equiped_item->equiped = false;
		equiped_item = nullptr;
	}
	return ret;
}

void Player::OnInputCallback(INPUTEVENT action, EVENTSTATE e_state)
{

	switch (action)
	{
		if (gamestate == INGAME)
		{
	case BUTTON_X:
	{
		if (e_state == E_DOWN)
		{
			state = ATTACKING;
			current_animation = App->anim_manager->GetAnimation(state, direction, 0);
			current_animation->Reset();
		}
		break;
	}
		}

	case BUTTON_START:
	{
		App->scene->switch_menu = true;
		gamestate = INMENU;
		break;
	}

	}
}

int Player::GetnuminputUse()
{
	int ret = 0;
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_REPEAT)
	{
		ret++;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT)
	{
		ret++;
	}
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_REPEAT)
	{
		ret++;
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_REPEAT)
	{
		ret++;
	}
	return ret;
}

void Player::ThrowHookshot(uint charge)
{
	hook->in_use = true;
	//CHECK DIRECTION
	if (direction == UP)
	{
		iPoint pos(position.x + 2, position.y - 16);
		hook->collision = App->collision->AddCollider({ pos.x, pos.y, 12, 8 }, COLLIDER_HOOKSHOT, hook);
		hook->direction = UP;
		hook->SetPos(pos);
	}
	else if (direction == RIGHT)
	{
		iPoint pos(position.x + 9, position.y + 2);
		hook->collision = App->collision->AddCollider({ position.x + 9, position.y + 4, 8, 12 }, COLLIDER_HOOKSHOT, hook);
		hook->direction = RIGHT;
		hook->SetPos(pos);
	}
	else if (direction == DOWN)
	{
		iPoint pos(position.x + 2, position.y + 5);
		hook->collision = App->collision->AddCollider({ position.x + 4, position.y + 5, 12, 8 }, COLLIDER_HOOKSHOT, hook);
		hook->direction = DOWN;
		hook->SetPos(pos);
	}
	else if (direction == LEFT)
	{
		iPoint pos(position.x - 9, position.y + 2);
		hook->collision = App->collision->AddCollider({ position.x - 13, position.y + 4, 8, 12 }, COLLIDER_HOOKSHOT, hook);
		hook->direction = LEFT;
		hook->SetPos(pos);
	}

	//SET MAX RANGE
	hook->SetRange(charge);
}

bool Player::Hooking()
{
	//collider follows the hookshot
	hook->collision->SetPos(hook->position.x, hook->position.y);
	HookState stat = hook->GetState();

	if (hook->actual_range_pos < hook->range)
	{
		if (stat == MISS)
		{
			HookState stat = hook->ReachObjective();
			KeepGoing();
			hook->actual_range_pos++;
		}
		else if (hook->GetState() == TARGET)
		{
			MoveTo(hook->position);
		}
		else if (hook->GetState() == OBSTACLE)
		{
			PickUpHook();
		}
	}

	else
	{
		PickUpHook();
	}

	return true;
}

void Player::KeepGoing()
{
	switch (direction)
	{
	case UP:
		hook->position.y-= hook->speed;
		break;
	case DOWN:
		hook->position.y += hook->speed;
		break;
	case LEFT:
		hook->position.x -= hook->speed;
		break;
	case RIGHT:
		hook->position.x += hook->speed;
		break;
	default:
		break;
	}
}

void Player::PickUpHook()
{
	switch (direction)
	{
	case UP:
		hook->position.y += hook->speed;
		if (hook->position.y + hook->height >= collision_player->rect.y)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	case DOWN:
		hook->position.y -= hook->speed;
		if (hook->position.y <= collision_player->rect.y + collision_player->rect.h)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	case LEFT:
		hook->position.x += hook->speed;
		if (hook->position.x + hook->width >= collision_player->rect.x)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	case RIGHT:
		hook->position.x -= hook->speed;
		if (hook->position.x <= collision_player->rect.x + collision_player->rect.w)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	default:
		break;
	}
}

void Player::MoveTo(const iPoint& pos)
{
	switch (direction)
	{
	case UP:
	{
		int temp = App->map->MovementCost(position.x, position.y - hook->speed, UP);
		if (temp == 0)
		{
			if (Camera_inside())
				App->render->camera.y += hook->speed * scale;
			position.y -= hook->speed;
		}
		else if (hook->position.y + hook->height >= collision_player->rect.y || temp !=0)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	}

	case DOWN:
	{
		int temp = App->map->MovementCost(position.x, position.y + (hook->speed + height), DOWN);
		if (temp == 0)
		{
			if (Camera_inside())
				App->render->camera.y -= hook->speed * scale;
			position.y += hook->speed;
		}
		else if (hook->position.y <= collision_player->rect.y || temp != 0)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	}

	case LEFT:
	{
		int temp = App->map->MovementCost(position.x - hook->speed, position.y, LEFT);
		if (temp == 0)
		{
			if (Camera_inside())
				App->render->camera.x = hook->speed * scale;
			position.x -= hook->speed;
		}
		else if (hook->position.x + hook->width >= collision_player->rect.x || temp != 0)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	}

	case RIGHT:
	{
		int temp = App->map->MovementCost(position.x + (hook->speed + width), position.y, RIGHT);
		if (temp == 0)
		{
			if (Camera_inside())
				App->render->camera.x -= hook->speed * scale;
			position.x += hook->speed;
		}
		else if (hook->position.x <= collision_player->rect.x || temp != 0)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	}

	default:
		break;
	}
}


bool Player::Move()
{
	int keysuse = GetnuminputUse();
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_REPEAT)
	{
		direction = LEFT;
		int temp = App->map->MovementCost(position.x - speed, position.y, LEFT);
		if (temp == T_CONTINUE)
		{
			if (Camera_inside())
				App->render->camera.x += speed * scale;
			position.x -= speed;
		}
		if (keysuse == 1) //if you pres a key left and up this if will do that dont move more fast
		{
			if (temp == T_UP)//up
			{
				direction = UP;
				if (Camera_inside())
					App->render->camera.y += speed * scale;
				position.y -= speed;
				direction = LEFT;
			}
			if (temp == T_DOWN)//down
			{
				direction = DOWN;
				if (Camera_inside())
					App->render->camera.y -= speed * scale;
				position.y += speed;
				direction = LEFT;
			}
		}

		walking = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT)
	{
		direction = DOWN;
		int temp = App->map->MovementCost(position.x, position.y + (speed + height), DOWN);
		if (temp == T_CONTINUE)
		{
			if (Camera_inside())
				App->render->camera.y -= speed * scale;
			position.y += speed;
		}
		if (keysuse == 1)
		{
			if (temp == T_LEFT)//left
			{
				direction = LEFT;
				if (Camera_inside())
					App->render->camera.x += speed * scale;
				position.x -= speed;
				direction = DOWN;
			}
			if (temp == T_RIGHT)//right
			{
				direction = RIGHT;
				if (Camera_inside())
					App->render->camera.x -= speed * scale;
				position.x += speed;
				direction = DOWN;
			}
		}

		walking = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_REPEAT)
	{
		direction = RIGHT;
		int temp = App->map->MovementCost(position.x + (speed + width), position.y, RIGHT);
		if (temp == T_CONTINUE)
		{
			if (Camera_inside())
				App->render->camera.x -= speed * scale;
			position.x += speed;
		}
		if (keysuse == 1)
		{
			if (temp == T_UP)//up
			{
				direction = UP;
				if (Camera_inside())
					App->render->camera.y += speed * scale;
				position.y -= speed;
				direction = RIGHT;
			}
			if (temp == T_DOWN)//down
			{
				direction = DOWN;
				if (Camera_inside())
					App->render->camera.y -= speed * scale;
				position.y += speed;
				direction = RIGHT;
			}
		}

		walking = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_REPEAT)
	{
		direction = UP;
		int temp = App->map->MovementCost(position.x, position.y - speed, UP);
		if (temp == T_CONTINUE)
		{
			if (Camera_inside())
				App->render->camera.y += speed * scale;
			position.y -= speed;
		}
		if (keysuse == 1)
		{
			if (temp == T_LEFT)//left
			{
				direction = LEFT;
				if (Camera_inside())
					App->render->camera.x += speed * scale;
				position.x -= speed;
				direction = UP;
			}
			if (temp == T_RIGHT)//right
			{
				direction = RIGHT;
				if (Camera_inside())
					App->render->camera.x -= speed * scale;
				position.x += speed;
				direction = UP;
			}
		}
		walking = true;
	}
	return walking;
}

bool Player::CheckOrientation()
{
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_REPEAT)
	{
		direction = LEFT;
	}
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_REPEAT)
	{
		direction = RIGHT;
	}
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_REPEAT)
	{
		direction = UP;
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT)
	{
		direction = DOWN;
	}
	return true;
}
