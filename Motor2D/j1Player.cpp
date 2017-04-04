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
#include "Golem.h"
#include "j1Collision.h"
#include "j1InputManager.h"
#include "Soldier.h"
#include "j1Item.h"
#include "j1DynamicObjects.h"
#include "j1Creature.h"
#include "j1Weapon.h"
#include "Geodude.h"
#include "Animation.h"
#include "Pokemon.h"

//Constructor
Player::Player() : Creature()
{
	type = CREATURE;
	name = "Link";
	hp_hearts = { 6,6 };
}

// Destructor
Player::~Player()
{}

// Called before render is available
bool Player::Awake(pugi::xml_node& conf)
{
	LOG("Loading Texture Player");
	bool ret = true;
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
	changeResolution = false;
	attacker = false;
	direction = DOWN;
	state = IDLE;
	scale = App->win->GetScale();
	offset_x = 7;
	offset_y = 10;
	gamestate = TIMETOPLAY;
	timetoplay.Start();
	canSwitchMap = true;
	black = 0;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 14, 14 }, COLLIDER_PLAYER, this);
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
	//if you dead, you appear on the Link House
	if (hp_hearts.y == 0)
	{
		hp_hearts = { 6,6 };
		if (App->map->CleanUp())
		{
			App->collision->EreseAllColiderPlayer();
			App->entity_elements->DelteElements();
			if (App->scene->enemy.size() > 0)
			{
				App->scene->enemy.clear();
			}
			if (App->scene->items.size() > 0)
			{
				App->scene->items.clear();
			}
			if (App->scene->dynobjects.size() > 0)
			{
				App->scene->dynobjects.clear();
			}
			if (App->scene->pokemons.size() > 0)
			{
				App->scene->pokemons.clear();
			}
			
			gems = 0;
			arrows = 0;
			bombs = 0;
			App->scene->Load_new_map(1);
		}
	}

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
		case HIT:
		{
			Hit();
			break;
		}
		case INTERACTING:
		{
			Interact();
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

		if (timetoplay.ReadSec() > 1 || dialog == nullptr)
		{
			gamestate = INGAME;
		}
		else
		{
			if (dialog->end && timetoplay.ReadSec() > 0.2f)
			{
				gamestate = INGAME;
				delete dialog;
				dialog = nullptr;
			}
		}
	}


	//CHARGE BAR --------------
	if (equiped_item != nullptr && equiped_item == hook && hook->in_use == false)
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
	if (equiped_item != nullptr && equiped_item == bombmanager && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		bombmanager->Drop(position);
		App->audio->PlayFx(6);
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
		if (App->scene->inventory) //TODO LOW -> If pres to fast you can lisen 2.
		{
			App->audio->PlayFx(3);
		}
		else
		{
			App->audio->PlayFx(2);
		}

		App->scene->switch_menu = true;
		gamestate = INMENU;
	}

	//Collision follow the player
	collision_feet->SetPos(position.x - offset_x, position.y - offset_y);


	return ret;
}

void Player::Draw()
{
	test_state = state;
	if (test_state == HIT)
	{
		test_state = IDLE;
	}
	//Draw player
	App->anim_manager->Drawing_Manager(test_state, direction, position, 0);  //TODO LOW-> ID magic number, need change!!
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
		if (c1 == collision_attack && c2->type == COLLIDER_DYNOBJECT && c2->callback->name != "chest" && c2->callback->name != "bigchest")
		{
			iPoint pos_dyn = App->map->WorldToMap(c2->callback->position.x, c2->callback->position.y);
			//srand(time(NULL)); 		int canDrop = rand() % 5 + 1;
			int canDrop = 1;
			if (canDrop == 1)
			{
				iPoint position;
				position.x = c2->callback->position.x + 4;
				position.y = c2->callback->position.y;
				DynamicObjects* temp = (DynamicObjects*)c2->callback;
				App->scene->items.push_back(App->entity_elements->CreateItem(temp->item_id, position));

			}

			App->map->EditCost(pos_dyn.x, pos_dyn.y, 0);
			App->map->EditCost(pos_dyn.x + 1, pos_dyn.y, 0);
			App->map->EditCost(pos_dyn.x, pos_dyn.y + 1, 0);
			App->map->EditCost(pos_dyn.x + 1, pos_dyn.y + 1, 0);


			App->entity_elements->DeleteDynObject((DynamicObjects*)c2->callback);
			//App->collision->EraseCollider(c2);
		}

		if (c1 == collision_interact && c2->type == COLLIDER_DYNOBJECT)
		{
			if (c2->callback->name == "chest" || c2->callback->name == "bigchest")
			{
				iPoint pos_dyn = App->map->WorldToMap(c2->callback->position.x, c2->callback->position.y);
				//srand(time(NULL)); 		int canDrop = rand() % 5 + 1;
				int canDrop = 1;
				if (canDrop == 1)
				{
					iPoint position;
					position.x = c2->callback->position.x + c2->rect.w*0.5;
					position.y = c2->callback->position.y + c2->rect.h;
					DynamicObjects* temp = (DynamicObjects*)c2->callback;
					App->scene->items.push_back(App->entity_elements->CreateItem(temp->item_id, position)); //TODO LOW call Drop item() function
				}

				App->entity_elements->DeleteDynObject((DynamicObjects*)c2->callback);
				//App->collision->EraseCollider(c2);
			}
		}


		if (c1 == collision_feet && c2->type == COLLIDER_ITEM)
		{
			Item* temp = (Item*)c2->callback;
			if (temp->pickable == true)
			{
				if (c2->callback->name == "rupee")
				{
					App->audio->PlayFx(4);
					gems++;
					App->entity_elements->DeleteItem((Item*)c2->callback);
					//App->collision->EraseCollider(c2);
				}
				if (c2->callback->name == "bomb")
				{
					if (bombmanager == nullptr)
					{
						bombmanager = App->entity_elements->CreateBombContainer();
						App->scene->start_menu->AddElement(App->gui->CreateButton({ 271,336,32,32 }, { 72,21 - 224 }, { 304,336 }, { 337,336 }, false, "bomb"));
					}
					App->entity_elements->DeleteItem((Item*)c2->callback);
					bombs++;
					//App->collision->EraseCollider(c2);
				}
				if (c2->callback->name == "hookshot")
				{
					if (hook == nullptr)
					{
						hook = App->entity_elements->CreateHookshot();
						App->scene->start_menu->AddElement(App->gui->CreateButton({ 271,301,32,32 }, { 48,21 - 224 }, { 304,301 }, { 337,301 }, false, "hookshot"));
					}
					App->entity_elements->DeleteItem((Item*)c2->callback);
					//App->collision->EraseCollider(c2);
				}
				if (c2->callback->name == "heart")
				{
					AddHeartContainer();
					App->entity_elements->DeleteItem((Item*)c2->callback);
					//App->collision->EraseCollider(c2);
				}
			}
		}
		if (c1 == collision_feet && c2->type == COLLIDER_ENEMY)
		{
			if (state != HIT && invincible_timer.ReadSec() >= 1) //TODO MED -> change this to a player state
			{
				state = HIT;
				hurt_timer.Start();
				invincible_timer.Start();
				hp_hearts.y--;
				App->audio->PlayFx(13);

				//KNOCKBACK--------------

				if (direction == UP)
				{
					if (App->map->MovementCost(position.x, position.y + 15, offset_x, offset_y, DOWN) == 0) //magic numbers 20 -> this is the distance you will move
					{
						position.y += 15;
						if (Camera_inside(iPoint(0, 15)))
							App->render->camera.y -= 15;
					}
				}
				if (direction == DOWN)
				{
					if (App->map->MovementCost(position.x, position.y - 15, offset_x, offset_y, UP) == 0) //magic numbers 20 -> this is the distance you will move
					{
						position.y -= 15;
						if (Camera_inside(iPoint(0, 15)))
							App->render->camera.y += 15;
					}
				}
				if (direction == LEFT)
				{
					if (App->map->MovementCost(position.x + 15, position.y, offset_x, offset_y, RIGHT) == 0) //magic numbers 20 -> this is the distance you will move
					{
						position.x += 15;
						if (Camera_inside(iPoint(15, 0)))
							App->render->camera.x -= 15;
					}
				}
				if (direction == RIGHT)
				{
					if (App->map->MovementCost(position.x - 15, position.y, offset_x, offset_y, LEFT) == 0) //magic numbers 20 -> this is the distance you will move
					{
						position.x -= 15;
						if (Camera_inside(iPoint(15, 0)))
							App->render->camera.x += 15;
					}
				}
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
		if (c1 == collision_feet && c2->type == COLLIDER_SWITCH_MAP)
		{
			if (canSwitchMap == false)
			{
				canSwitchMap = true;
			}
			else
			{
				iPoint temp_meta = App->map->WorldToMap(position.x, position.y); //central position
				MapLayer* meta_ = App->map->data.layers[1];
				int id_meta = meta_->Get(temp_meta.x, temp_meta.y);
				for (int i = 0; i < App->map->directMap.size(); i++)
				{
					if (App->map->directMap[i].id_tile == id_meta)
					{
						canSwitchMap = false;
						App->scene->switch_map = App->map->directMap[i].id_map;
						App->scene->newPosition = App->map->directMap[i].position;
					}
				}
			}
		}

		if (c1 == collision_feet && c2->type == COLLIDER_POKEMON) //TODO MED -> change this (we will have golbats int the future)
		{
			if (state != HOOKTHROWN)
			{
				if (c2->callback->name == "Golem" && c2->callback->state == HIT)
				{
					//Not dammage
				}
				else
				{
					if (hurt == false)
					{
						timer.Start();
						hurt = true;
						hp_hearts.y--;

						if (direction == UP)
						{
							if (App->map->MovementCost(position.x, position.y + 15, offset_x, offset_y, DOWN) == 0)
							{
								position.y += 15;
								if (Camera_inside(iPoint(0, 15)))
									App->render->camera.y -= 15;
							}
						}
						if (direction == DOWN)
						{
							if (App->map->MovementCost(position.x, position.y - 15, offset_x, offset_y, UP) == 0)
							{
								position.y -= 15;
								if (Camera_inside(iPoint(0, 15)))
									App->render->camera.y += 15;
							}
						}
						if (direction == LEFT)
						{
							if (App->map->MovementCost(position.x + 15, position.y, offset_x, offset_y, RIGHT) == 0)
							{
								position.x += 15;
								if (Camera_inside(iPoint(15, 0)))
									App->render->camera.x -= 15;
							}
						}
						if (direction == RIGHT)
						{
							if (App->map->MovementCost(position.x - 15, position.y, offset_x, offset_y, LEFT) == 0)
							{
								position.x -= 15;
								if (Camera_inside(iPoint(15, 0)))
									App->render->camera.x += 15;
							}
						}
					}
					else
					{
						if (timer.ReadSec() >= 1)
						{
							hurt = false;
						}
					}
				}
			}
		}

		if (c1 == collision_attack && c2->type == COLLIDER_POKEMON)
		{
			if (c2->callback->name == "Golem" && c2->callback->state == HIT)
			{
				//Not dammage
			}
			else
			{
				if (c2->callback->name == "Geodude")
				{
					Geodude* pokemon = (Geodude*)c2->callback;
					if (hurt == false)
					{
						hurt = true;
						timer.Start();
						pokemon->hp--;
					}
					if (timer.ReadSec() > 0.5f)
					{
						hurt = false;
					}


					if (pokemon->hp == 0)
					{
						c2->callback->state = DYING;
					}
					else
					{
						c2->callback->state = HIT;
						pokemon->dir_hit = c1->callback->direction;
						pokemon->previus_position = pokemon->position;
					}
				}
				if (c2->callback->name == "Golem")
				{
					Golem* pokemon = (Golem*)c2->callback;
					pokemon->hp--;
					if (pokemon->hp == 0)
					{
						c2->callback->state = DYING;
					}
					else
					{
						c2->callback->state = HIT;
						pokemon->dir_hit = c1->callback->direction;
						pokemon->previus_position = pokemon->position;
					}
				}
			}
		}

		if (c1 == collision_interact && c2->type == COLLIDER_TRAINER)
		{
			if (gamestate == INGAME)
			{
				if (dialog == nullptr)
				{
					gamestate = INMENU;
					if (direction == UP)
						c2->callback->direction = DOWN;
					else if (direction == DOWN)
						c2->callback->direction = UP;
					else if (direction == LEFT)
						c2->callback->direction = RIGHT;
					else
						c2->callback->direction = LEFT;
					dialog = App->gui->CreateDialogue("> Allahuakbar LOREM IPSUM,main nemim i spotato nintendo switch nontendoo SL maoeraoern ayylmao olaefc bruh. THE END");
					App->collision->EraseCollider(collision_interact);
					interaction = false;
					state = IDLE;
				}
			}
		}

		if (c1 == collision_feet && c2->type == COLLIDER_BOMB)
		{
			if (hurt == false) 
			{
				GetDamage();
				hurt = true;
			}
		}
	}
}

bool Player::Camera_inside()
{
	//256x224
	if (camera_follow == true)
	{
		iPoint camera_pos(-App->render->camera.x / 2, -App->render->camera.y / 2);
		iPoint size_map = App->map->MapToWorld(App->map->data.width, App->map->data.height);
		if (direction == UP)
		{
			if (camera_pos.y == 0)
			{
				return false;
			}
			else
			{
				if (position.y > size_map.y - (App->win->GetHeight() / scale) / 2)
				{
					return false;
				}
			}
		}
		if (direction == DOWN)
		{
			if (camera_pos.y + (App->win->GetHeight() / scale) >= size_map.y)
			{
				return false;
			}
			else
			{
				if (position.y < (App->win->GetHeight() / scale) / 2)
				{
					return false;
				}
			}
		}
		if (direction == LEFT)
		{
			if (camera_pos.x == 0)
			{
				return false;
			}
			else
			{
				if (position.x > size_map.x - (App->win->GetWidth() / scale) / 2)
				{
					return false;
				}
			}
		}
		if (direction == RIGHT)
		{
			if (camera_pos.x + (App->win->GetWidth() / scale) >= size_map.x)
			{
				return false;
			}
			else
			{
				if (position.x < (App->win->GetWidth() / scale) / 2)
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}
	return true;
}

bool Player::Camera_inside(iPoint pos)
{
	//256x224
	if (camera_follow == true)
	{
		iPoint camera_pos(-App->render->camera.x / 2, -App->render->camera.y / 2);
		iPoint size_map = App->map->MapToWorld(App->map->data.width, App->map->data.height);
		if (direction == UP)
		{
			if (camera_pos.y - pos.y <= 0)
			{
				return false;
			}
			else
			{
				if (position.y > size_map.y - (App->win->GetHeight() / scale) / 2)
				{
					return false;
				}
			}
		}
		if (direction == DOWN)
		{
			if (camera_pos.y + pos.y + (App->win->GetHeight() / scale) >= size_map.y)
			{
				return false;
			}
			else
			{
				if (position.y < (App->win->GetHeight() / scale) / 2)
				{
					return false;
				}
			}
		}
		if (direction == LEFT)
		{
			if (camera_pos.x - pos.x <= 0)
			{
				return false;
			}
			else
			{
				if (position.x > size_map.x - (App->win->GetWidth() / scale) / 2)
				{
					return false;
				}
			}
		}
		if (direction == RIGHT)
		{
			if (camera_pos.x + pos.x + (App->win->GetWidth() / scale) >= size_map.x)
			{
				return false;
			}
			else
			{
				if (position.x < (App->win->GetWidth() / scale) / 2)
				{
					return false;
				}
			}
		}
	}
	else
	{
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

	else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		state = INTERACTING;
		//current_animation = App->anim_manager->GetAnimation(state, direction, 0);
		//current_animation->Reset();
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

	else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		state = INTERACTING;
		//current_animation = App->anim_manager->GetAnimation(state, direction, 0);
		//current_animation->Reset();
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
			App->audio->PlayFx(5);
			collision_attack = App->collision->AddCollider({ position.x - 4, position.y - offset_y - 16, 8, 20 }, COLLIDER_PLAYER, this);
		}
		else if (direction == RIGHT)
		{
			App->audio->PlayFx(5);
			collision_attack = App->collision->AddCollider({ position.x + 3, position.y - 8, 20, 8 }, COLLIDER_PLAYER, this);
		}
		else if (direction == DOWN)
		{
			App->audio->PlayFx(5);
			collision_attack = App->collision->AddCollider({ position.x - 4, position.y - 2, 8, 20}, COLLIDER_PLAYER, this);
		}
		else if (direction == LEFT)
		{
			App->audio->PlayFx(5);
			collision_attack = App->collision->AddCollider({ position.x - 22, position.y - 7, 20, 8 }, COLLIDER_PLAYER, this);
		}
	}
	return true;
}

bool Player::Interact()
{
	if (interaction)
	{
		if (timer.ReadSec() >= 0.3) // change to animation.finished
		{
			//if (current_animation->Finished())
			//{
			App->collision->EraseCollider(collision_interact);
			interaction = false;
			//current_animation->Reset();
			//current_animation = nullptr;
			state = IDLE;
			//}
		}
	}
	else
	{
		timer.Start();
		interaction = true;
		if (direction == UP)
		{
			collision_interact = App->collision->AddCollider({ position.x - 8, position.y - 14, 16, 5 }, COLLIDER_PLAYER, this);
		}
		else if (direction == RIGHT)
		{
			collision_interact = App->collision->AddCollider({ position.x + offset_x - 1, position.y - offset_y - 1, 5, 16 }, COLLIDER_PLAYER, this);
		}
		else if (direction == DOWN)
		{
			collision_interact = App->collision->AddCollider({ position.x - 8, position.y + 3, 16, 5 }, COLLIDER_PLAYER, this);
		}
		else if (direction == LEFT)
		{
			collision_interact = App->collision->AddCollider({ position.x - offset_x - 4, position.y - offset_y - 1, 5, 16 }, COLLIDER_PLAYER, this);
		}
	}
	return true;
}

bool Player::Equip(Weapon* to_equip)
{
	if (to_equip != nullptr)
	{
		if (equiped_item != to_equip && to_equip->equipable == true)
		{
			equiped_item = to_equip;
			equiped_item->equiped = true;
			LOG("Equiped %s", equiped_item->name.c_str());
			return true;
		}
	}

	LOG("Can't equip item");
	return false;
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
	case BUTTON_A:
	{
		if (e_state == E_DOWN)
		{
			if (dialog == nullptr)
			{
				state = INTERACTING;
				//current_animation = App->anim_manager->GetAnimation(state, direction, 0);
				//current_animation->Reset();
			}
			else if (dialog->end == false)
			{
				dialog->PushLine(true);
			}
		}
		break;
	}

	case BUTTON_B:
		if (hook != nullptr && equiped_item == hook)
		{
			if (e_state == E_UP)
			{
				state = HOOKTHROWN;
				ThrowHookshot(charge);
			}
		}
		else if (bombmanager != nullptr && equiped_item == bombmanager)
		{
			if (e_state == E_UP)
			{
				bombmanager->Drop(position);
			}
		}
		break;
		}

	case BUTTON_START:
	{
		if (App->scene->inventory)
		{
			App->audio->PlayFx(2);
		}
		else
		{
			App->audio->PlayFx(3);
		}

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
		iPoint pos(position.x, position.y - 3);
		hook->SetPos(pos);
		hook->offset_x = 6;
		hook->offset_y = 4;
		hook->collision = App->collision->AddCollider({ pos.x - hook->offset_x, pos.y - hook->offset_y, 12, 8 }, COLLIDER_HOOKSHOT, hook);
		hook->direction = UP;
	
	}
	else if (direction == RIGHT)
	{
		iPoint pos(position.x, position.y - 3);
		hook->SetPos(pos);
		hook->offset_x = 4;
		hook->offset_y = 6;
		hook->collision = App->collision->AddCollider({ pos.x + offset_x, pos.y - hook->offset_y, 8, 12 }, COLLIDER_HOOKSHOT, hook);
		hook->direction = RIGHT;
	}
	else if (direction == DOWN)
	{
		iPoint pos(position.x, position.y);
		hook->SetPos(pos);
		hook->offset_x = 6;
		hook->offset_y = 4;
		hook->collision = App->collision->AddCollider({ pos.x - hook->offset_x, pos.y + hook->offset_y, 12, 8 }, COLLIDER_HOOKSHOT, hook);
		hook->direction = DOWN;
	}
	else if (direction == LEFT)
	{
		iPoint pos(position.x, position.y - 3);
		hook->SetPos(pos);
		hook->offset_x = 4;
		hook->offset_y = 6;
		hook->collision = App->collision->AddCollider({ pos.x - hook->offset_x, pos.y - hook->offset_y, 8, 12 }, COLLIDER_HOOKSHOT, hook);
		hook->direction = LEFT;
	}

	//SET MAX RANGE
	hook->SetRange((float)charge);
}

bool Player::Hooking()
{
	//collider follows the hookshot
	hook->collision->SetPos(hook->position.x - hook->offset_x, hook->position.y - hook->offset_y);
	HookState stat = hook->GetState();

	if (hook->actual_range_pos < hook->range)
	{
		if (stat == MISS)
		{
			HookState stat = hook->ReachObjective(actual_floor);
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
		if (hook->position.y + hook->offset_y >= collision_feet->rect.y)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	case DOWN:
		hook->position.y -= hook->speed;
		if (hook->position.y <= collision_feet->rect.y + collision_feet->rect.h)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	case LEFT:
		hook->position.x += hook->speed;
		if (hook->position.x + hook->offset_x >= collision_feet->rect.x)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	case RIGHT:
		hook->position.x -= hook->speed;
		if (hook->position.x <= collision_feet->rect.x + collision_feet->rect.w)
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
		//int temp = App->map->MovementCost(position.x, position.y - hook->speed, UP);
		if (Camera_inside(pos))
			App->render->camera.y += hook->speed * scale;

		position.y -= hook->speed;

		if (hook->position.y >= position.y)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	}

	case DOWN:
	{
		//int temp = App->map->MovementCost(position.x, position.y + (hook->speed + height), DOWN
		if (Camera_inside(pos))
			App->render->camera.y -= hook->speed * scale;
		position.y += hook->speed;

		if (hook->position.y <= position.y)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	}

	case LEFT:
	{
		//int temp = App->map->MovementCost(position.x - hook->speed, position.y, LEFT);

		if (Camera_inside(pos))
			App->render->camera.x += hook->speed * scale;
		position.x -= hook->speed;

		if (hook->position.x >= position.x)
		{
			hook->Reset();
			state = IDLE;
		}
		break;
	}

	case RIGHT:
	{
		//int temp = App->map->MovementCost(position.x + (hook->speed + width), position.y, RIGHT

		if (Camera_inside(pos))
			App->render->camera.x -= hook->speed * scale;
		position.x += hook->speed;

		if (hook->position.x <= position.x)
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
		//int temp = App->map->MovementCost(position.x - speed, position.y, LEFT
		int temp = App->map->MovementCost(collision_feet->rect.x - speed, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT);
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

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_REPEAT)
	{
		direction = RIGHT;
		//int temp = App->map->MovementCost(position.x + (speed + width), position.y, RIGHT);
		int temp = App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + speed, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT);
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
		//int temp = App->map->MovementCost(position.x, position.y - speed, UP);
		int temp = App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - speed, collision_feet->rect.w, collision_feet->rect.h, UP);
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

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT)
	{
		direction = DOWN;
		//int temp = App->map->MovementCost(position.x, position.y + (speed + height), DOWN);
		int temp = App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + speed, collision_feet->rect.w, collision_feet->rect.h, DOWN);
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

	//Set the actual floor of the player
	if (App->map->data.layers.size() >= 3) // Only Victory Road has 3 layers
	{
		GetfloorLvl(position);
	}

	return walking;
}

bool Player::Hit()
{
	if (hurt_timer.ReadSec() >= 0.2)
	{
		state = IDLE;
	}

	return true;
}

void Player::GetfloorLvl(iPoint pos)
{
	const MapLayer* meta_layer = App->map->data.layers[2];
	iPoint map_pos = App->map->WorldToMap(pos.x, pos.y);
	int player_lvl = meta_layer->Get(map_pos.x, map_pos.y);

	const TileSet* tileset = App->map->data.tilesets[1];
	int first_floor = tileset->firstgid + 1; // RED TILE
	int second_floor = tileset->firstgid + 2; // YELLOW TILE
	int third_floor = tileset->firstgid ; // GREEN TILE

	if (first_floor == player_lvl)
	{
		actual_floor = 0;
	}
	else if (second_floor == player_lvl)
	{
		actual_floor = 1;
	}
	else if (third_floor == player_lvl)
	{
		actual_floor = 2;
	}

	LOG("Link is in floor %i", actual_floor);
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

void Player::AddHeartContainer()
{
	iPoint last_heart_pos = App->scene->hp->elements.back()->position - App->scene->hp->position;
	Image* newhp = App->gui->CreateImage({ 177,24,7,7 }, { last_heart_pos.x + 9,last_heart_pos.y }, "hp_add");
	App->scene->hp->elements.push_back(newhp);
	newhp->parent = App->scene->hp;
	newhp->CalculateDiferential();
	hp_hearts.x += 2;
	hp_hearts.y = hp_hearts.x;
}

void Player::ShowHearts()
{
	int addheart = hp_hearts.y;
	for (int i = 0; i < App->scene->hp->elements.size(); i++)
	{
		if (addheart - 2 >= 0)
		{
			App->scene->hp->elements[i]->Hitbox.x = 161;
			addheart -= 2;
		}
		else if (addheart - 1 >= 0)
		{
			App->scene->hp->elements[i]->Hitbox.x = 169;
			addheart--;
		}
		else if (addheart == 0)
		{
			App->scene->hp->elements[i]->Hitbox.x = 177;
		}
	}
}

void Player::GetDamage()
{
	if (hp_hearts.y>0)
		hp_hearts.y--;
}