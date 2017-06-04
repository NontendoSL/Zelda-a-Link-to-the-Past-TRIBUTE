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
#include "j1FadeToBlack.h"
#include "Geodude.h"
#include "Animation.h"
#include "Pokemon.h"
#include "Villager.h"
#include "RedMinion.h"
#include "CombatManager.h"

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
	hp = conf.child("stats").attribute("hp").as_int(0);
	attack = conf.child("stats").attribute("attack").as_int(0);
	speed = conf.child("stats").attribute("speed").as_int(0);
	position.x = conf.child("stats").attribute("pos_x").as_int(0);
	position.y = conf.child("stats").attribute("pos_y").as_int(0);
	hp_hearts = { 6,6 };

	return ret;
}

// Called before the first frame
bool Player::Start()
{
	bool ret = true;

	//changeResolution = false;
	attacker = false;

	//ANIMATION ---------------
	direction = DOWN;
	state = L_IDLE;
	anim_state = L_IDLE;
	scale = App->win->GetScale();
	offset_x = 7;
	offset_y = 10;
	//--------------------------

	/*timetoplay.Start();*/
	canSwitchMap = true;
	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 14, 14 }, COLLIDER_PLAYER, this);
	game_timer.Start();

	return ret;
}

bool Player::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdate_Player", Profiler::Color::RosyBrown)
	bool ret = true;

	return ret;
}

bool Player::Update(float dt)
{
	/*sprintf_s(buffer, 30, "%.f seconds of playtime", game_timer.ReadSec());
	std::string var = std::to_string(game_timer.ReadSec()) + "seconds of playtime";
	time->Write(var.c_str());
	var.clear();*/
	BROFILER_CATEGORY("Update_Collision", Profiler::Color::Aqua);
	bool ret = true;
	//if you die, you appear on the Link House
	if (hp_hearts.y == 0)
	{
		score = 0;
		hp_hearts = { hp_hearts.x, hp_hearts.x };
		if (before_map == 3 && App->scene->last_map == 2 || before_map == 4 && App->scene->last_map == 2)
		{
			App->scene->switch_map = 2;
			App->scene->useTP = false;
			App->scene->newPosition = App->scene->teleports[5].position;
		}
		else
		{
			App->scene->switch_map = App->scene->last_map;
			App->scene->useTP = true;
		}
		if (gems > 10)
		{
			gems = gems - (gems/ 10);
		}
		if (arrows > 10)
		{
			arrows = arrows - (arrows / 10);
		}

	}

	// STATE MACHINE ------------------
	if (App->scene->gamestate == INGAME)
	{
		//CHARGE BAR --------------
		if (equiped_item != nullptr && equiped_item == hook && hook->in_use == false)
		{
			if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_REPEAT) && charge <= 34)
			{
				charge++;
			}
			else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_UP)
			{
				state = L_HOOKTHROWN;
				anim_state = L_IDLE;
				ThrowHookshot(charge);
			}
			else if (charge > 0)
			{
				charge--;
			}
		}

		if (equiped_item != nullptr && equiped_item == bombmanager && bombs > 0)
		{
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_UP)
			{
				bombmanager->Drop(position);
				bombs--;
				App->audio->PlayFx(6);
			}
		}

		if (equiped_item != nullptr && equiped_item == bow)
		{
 			if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_REPEAT) && charge <= 34)
			{
				if (charge++ == 17)
				{
					App->audio->PlayFx(16);
				}
			}
			else if ((App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_UP) && charge >= 17)
			{
				if (arrows > 0)
				{
					state = L_BOW;
					anim_state = L_BOW;
					current_animation = App->anim_manager->GetAnimation(anim_state, direction, LINK);
					current_animation->Reset();
					bow->Shoot(position, direction, charge);
					arrows--;
				}
			}
			else if (charge > 0)
			{
				charge--;
			}
		}
		// --------------------

		switch (state)
		{
		case L_IDLE:
		{
			Idle();
			break;
		}
		case L_WALKING:
		{
			Walking(dt);
			break;
		}
		case L_ATTACKING:
		{
			Attack();
			break;
		}
		case L_HIT:
		{
			Hit();
			break;
		}
		case L_INTERACTING:
		{
			Interact();
			break;
		}
		case L_HOOKTHROWN:
		{
			Hooking();
			break;
		}
		case L_BOW:
		{
			BowAnim();
			break;
		}
		default:
		{
			break;
		}
		}
	}

	else if (App->scene->gamestate == INMENU)
	{
		if (winover != nullptr)
		{
			if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			{
				winover->visible = false;
				for (int i = 0; i < winover->elements.size(); i++)
				{
					winover->elements[i]->visible = false;
				}
			}
		}
	}

	/*else if (gamestate == TIMETOPLAY) //TODO JORDI - ELLIOT
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
	}*/
	else if (App->scene->gamestate == GAMEOVER)
	{
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_START) == EVENTSTATE::E_DOWN ||
			App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_DOWN)
		{
			if (gameover != nullptr)
				gameover->visible = false;
			if (winover != nullptr)
				winover->visible = false;
			App->scene->gamestate = INGAME;
		}
	}


	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && App->fadetoblack->IsFading())
	{

	}
	
	//Collision follow the player
	collision_feet->SetPos(position.x - offset_x, position.y - offset_y);

	return ret;
}

void Player::Draw()
{
	
	BROFILER_CATEGORY("Update_Player", Profiler::Color::AliceBlue);
	//Draw player
	App->anim_manager->Drawing_Manager(anim_state, direction, position, LINK);
}

bool Player::CleanUp()
{
	bool ret = true;

	return ret;
}

bool Player::SaveData(pugi::xml_node& check_node)
{
	pugi::xml_node hp = check_node.append_child("HP");
	hp.append_attribute("max") = checkpoint.hp_hearts.x;
	hp.append_attribute("current") = checkpoint.hp_hearts.y;

	pugi::xml_node map = check_node.append_child("MAP");
	map.append_attribute("id") = checkpoint.map_id;

	pugi::xml_node resources = check_node.append_child("RESOURCES");
	resources.append_attribute("rupees") = checkpoint.rupees;
	resources.append_attribute("arrows") = checkpoint.arrows;
	resources.append_attribute("bombs") = checkpoint.bombs;

	pugi::xml_node weapons = check_node.append_child("WEAPONS");
	weapons.append_attribute("bow") = checkpoint.bow_picked;
	weapons.append_attribute("bombmanager") = checkpoint.bombcontainer_picked;
	weapons.append_attribute("hookshot") = checkpoint.hookshot_picked;
	weapons.append_attribute("sword") = checkpoint.sword_picked;

	pugi::xml_node ui = check_node.append_child("UI");
	if (App->gui->GetGuiState() >= ZELDA_HUD && App->gui->GetGuiState() < POKEMON_COMBAT)
	{
		ui.append_attribute("world") = "Zelda";
	}
	else if (App->gui->GetGuiState() >= POKEMON_COMBAT && App->gui->GetGuiState() < MAPPING_CONTROLLER)
	{
		ui.append_attribute("world") = "Pokemon";
	}

	pugi::xml_node pokecash_node = check_node.append_child("POKECASH");
	pokecash_node.append_attribute("cash") = pokecash;

	pugi::xml_node pokemon_items = check_node.append_child("POKEMON");

	pugi::xml_node pokemon = pokemon_items.append_child("Blaziken");
	pokemon.append_attribute("hp") = checkpoint.blaz_hp;
	pokemon.append_attribute("atk") = checkpoint.blaz_atk;
	pokemon.append_attribute("def") = checkpoint.blaz_def;
	pokemon.append_attribute("pos") = checkpoint.blaz_pos;

	pokemon = pokemon_items.append_child("Sceptyle");
	pokemon.append_attribute("hp") = checkpoint.scept_hp;
	pokemon.append_attribute("atk") = checkpoint.scept_atk;
	pokemon.append_attribute("def") = checkpoint.scept_def;
	pokemon.append_attribute("pos") = checkpoint.scept_pos;

	pokemon = pokemon_items.append_child("Swampert");
	pokemon.append_attribute("hp") = checkpoint.swamp_hp;
	pokemon.append_attribute("atk") = checkpoint.swamp_atk;
	pokemon.append_attribute("def") = checkpoint.swamp_def;
	pokemon.append_attribute("pos") = checkpoint.swamp_pos;

	return true;
}

bool Player::Save()
{
	App->entity_elements->XML.child("config").child("Link").child("stats").attribute("hp").set_value(hp);
	App->entity_elements->XML.save_file("config.xml");
	return true;
}

void Player::OnCollision(Collider* c1, Collider* c2)
{
	if (App->scene->combat == false)
	{
		if (c1 != nullptr && c2 != nullptr)
		{
			if (c2->callback != nullptr)
			{
				// BREAKING OBJECTS -----------
				if (c1->type == COLLIDER_SWORD && c2->type == COLLIDER_DYNOBJECT && c2->callback->name != "chest" && c2->callback->name != "sign" && c2->callback->name != "bigchest") //c2->callback->destruvtible == true
				{
					//srand(time(NULL)); 		int canDrop = rand() % 5 + 1;

					((DynamicObjects*)c2->callback)->ModifyTileCost(0);

					int canDrop = 1;
					if (canDrop == 1)
					{
						iPoint position;
						position.x = c2->callback->position.x + 4;
						position.y = c2->callback->position.y;
						App->entity_elements->CreateItem(c2->callback->DropItem(), position);
					}

					//App->entity_elements->DeleteDynObject((DynamicObjects*)c2->callback);
					((DynamicObjects*)c2->callback)->SetState(D_IMPACTING);
					((DynamicObjects*)c2->callback)->start_impact = true;
				}
			}
			// ------------------------------

			// INTERACTING WITH OBJECTS ------------------
			if (c1 == collision_interact && c2->type == COLLIDER_DYNOBJECT && c2->callback != nullptr)
			{
				// SIGNS INFO TEXT
				if (c2->callback->name == "sign")
				{
					if (App->scene->gamestate == INGAME)
					{
						if (dialog == nullptr)
						{
							App->scene->gamestate = INMENU;
							interaction = false;
							DynamicObjects* dynobj = (DynamicObjects*)c2->callback;
							dialog = App->gui->CreateDialogue(dynobj->GetDialog().c_str(),1);
							collision_interact->to_delete = true;
							state = L_IDLE;
							anim_state = L_IDLE;
						}
					}
				}

				// OPENING CHESTS
				if (c2->callback->name == "chest" || c2->callback->name == "bigchest")
				{
					iPoint pos_dyn = App->map->WorldToMap(c2->callback->position.x, c2->callback->position.y);
					App->audio->PlayFx(15);
					//create item
					iPoint position;
					position.x = c2->callback->position.x + c2->rect.w*0.3;
					position.y = c2->callback->position.y + c2->rect.h;
					DynamicObjects* temp = (DynamicObjects*)c2->callback;
					App->entity_elements->CreateItem(c2->callback->DropItem(), position); //TODO LOW call Drop item() function
					score += 75;
					App->entity_elements->DeleteDynObject((DynamicObjects*)c2->callback);
					if (App->scene->last_map == 3)
					{
						App->map->EditCost(11, 50, 0);
						App->map->EditCost(12, 50, 0);
						App->entity_elements->DeleteElement("door");
					}
				}

				// PICKING OBJECTS 
				if (((DynamicObjects*)c2->callback)->pickable == true && ((DynamicObjects*)c2->callback)->GetState() == D_IDLE) //TO PICK VASES AND BUSHES
				{
					if (picked_object == nullptr) // Only carry one object.
					{
						// DROP THE ITEM INSIDE -----------------------
						iPoint position;
						position.x = c2->callback->position.x + 4;
						position.y = c2->callback->position.y;
						App->entity_elements->CreateItem(c2->callback->DropItem(), position);
						// --------------------------------------------

						((DynamicObjects*)c2->callback)->ModifyTileCost(0);
						((DynamicObjects*)c2->callback)->SetState(D_PICKED);
						((DynamicObjects*)c2->callback)->SetAnimState(D_IDLE);
						//SetAnimState(L_PICKING);
						picked_object = ((DynamicObjects*)c2->callback);
						((DynamicObjects*)c2->callback)->Follow((SceneElement*)this);

						//Set texture rect -----
						if (((DynamicObjects*)c2->callback)->name == "bush_plant")
						{
							((DynamicObjects*)c2->callback)->rect.x = 2;
							((DynamicObjects*)c2->callback)->rect.y = 17;
						}
						//----------------------

						App->scene->swap_object = true;
					}
				}
			}
			// --------------------------------------------------------

			// OBTAINING ITEMS -------------------
			if (c1 == collision_feet && c2->type == COLLIDER_ITEM && c2->callback != nullptr)
			{
				Item* temp = (Item*)c2->callback;
				if (temp->pickable == true)
				{
					if (c2->callback->name == "rupee")
					{
						App->audio->PlayFx(4);
						gems++;
					}
					if (c2->callback->name == "red_rupee")
					{
						App->audio->PlayFx(4);
						gems+=20;
					}
					if (c2->callback->name == "blue_rupee")
					{
						App->audio->PlayFx(4);
						gems += 5;
					}
					if (c2->callback->name == "bomb")
					{
						//First time picking a bomb
						if (bombmanager == nullptr)
						{
							dialog = App->gui->CreateDialogue("Bomb picked!",0);
							bombmanager = App->entity_elements->CreateBombContainer();
							App->scene->start_menu->PickItem("bomb");
							App->audio->PlayFx(20);
							score += 75;
							bombs += 4;
						}
						bombs++;
						score += 5;
					}
					if (c2->callback->name == "hookshot")
					{
						//First time picking a hookshot
						if (hook == nullptr)
						{
							dialog = App->gui->CreateDialogue("Hookshot picked!",0);
							hook = App->entity_elements->CreateHookshot();
							App->scene->start_menu->PickItem("hookshot");
							App->audio->PlayFx(20);
							score += 75;
						}
					}
					if (c2->callback->name == "bow")
					{
						//First time picking a bow
						if (bow == nullptr)
						{
							dialog = App->gui->CreateDialogue("Bow picked!",0);
							bow = App->entity_elements->CreateBow();
							App->scene->start_menu->PickItem("bow");
							App->audio->PlayFx(20);
							arrows += 5;
							score += 75;
						}
					}
					if (c2->callback->name == "heart")
					{
						AddHeartContainer();
					}

					if (c2->callback->name == "arrows")
					{
						//First time picking a bomb
						score += 75;
						arrows += 5;
					}

					if (c2->callback->name == "sword_shield")
					{
						sword_equiped = true;
						dialog = App->gui->CreateDialogue("With great power comes great responsability       -Link's Uncle",0);
						App->audio->PlayFx(20);
					}

					//Delete item when picked
					App->entity_elements->DeleteItem((Item*)c2->callback);
				}
			}
			// -----------------------
			
			// LINK HIT BY ENEMY -------------------
			if (c1 == collision_feet && (c2->type == COLLIDER_ENEMY || c2->type == COLLIDER_GMINION)) //If green soldier attacks you
			{
				if (state != L_HIT && invincible_timer.ReadSec() >= 1 && state != L_DYING)
				{
					App->audio->PlayFx(13);
					state = L_HIT;
					anim_state = L_HIT;
					hurt_timer.Start();
					invincible_timer.Start();
					hp_hearts.y--;
					dir_hit = c2->callback->direction;
					prev_position = position;

					if (picked_object != nullptr) // Destroy the picked object if an enemy attacks you.
					{
						picked_object->SetState(D_IMPACTING);
						picked_object->start_impact = true;
						picked_object = nullptr;
					}

					if (interaction == true)
					{
						collision_interact->to_delete = true;
						interaction = false;
					}
				}
			}
			// --------------------------------------

			// LINK HIT BY	RED MINION -------------------
			if (c1 == collision_feet && c2->type == COLLIDER_RMINION) //If green soldier attacks you
			{
				if (state != L_HIT && invincible_timer.ReadSec() >= 1)
				{
					App->audio->PlayFx(13);
					state = L_HIT;
					anim_state = L_HIT;
					hurt_timer.Start();
					invincible_timer.Start();
					hp_hearts.y--;
					dir_hit = c2->callback->direction;
					prev_position = position;

					if (picked_object != nullptr) // Destroy the picked object if an enemy attacks you.
					{
						picked_object->SetState(D_IMPACTING);
						picked_object->start_impact = true;
						picked_object = nullptr;
					}

					if (interaction == true)
					{
						collision_interact->to_delete = true;
						interaction = false;
					}

					//When Red Minion impacts Link, it explodes --------
					((RedMinion*)c2->callback)->start_explosion = true;
					//-------------------------------------------------
				}
			}
			// --------------------------------------

			// LINK HIT BY FIREBAT OR BOMB (same mechanic) -------------------
			if (c1 == collision_feet && (c2->type == COLLIDER_FIREBAT || c2->type == COLLIDER_BOMB))
			{
				if (state != L_HIT && invincible_timer.ReadSec() >= 1 && state != L_DYING)
				{
					App->audio->PlayFx(13);
					state = L_HIT;
					anim_state = L_HIT;
					hurt_timer.Start();
					invincible_timer.Start();
					hp_hearts.y--;

					//Knockback ---------
					SetKnockbackDir();
					// ---------------

					prev_position = position;

					if (picked_object != nullptr) // Destroy the picked object if an enemy attacks you.
					{
						picked_object->SetState(D_IMPACTING);
						picked_object->start_impact = true;
						picked_object = nullptr;
					}

					if (interaction == true)
					{
						collision_interact->to_delete = true;
						interaction = false;
					}
				}
			}
			// --------------------------------------

			// MAP TELEPORTING ---------------
			if (c1 == collision_feet && c2->type == COLLIDER_SWITCH_MAP)
			{
				if (canSwitchMap == false) // TODO LOW -> delete canSwitchMap
				{
					canSwitchMap = true;
				}
				else
				{
					for (int i = 0; i < App->scene->teleports.size(); i++)
					{
						if (App->scene->teleports[i].rect.x == c2->rect.x &&
							App->scene->teleports[i].rect.y== c2->rect.y)
						{
							canSwitchMap = false;
							before_map = App->scene->last_map;
							App->scene->switch_map = App->scene->teleports[i].id_map;
							App->scene->newPosition = App->scene->teleports[i].position;
						}
					}
				}
			}
			// --------------------------------------

			// NPC INTERACTING ----------------------
			if (c1 == collision_interact && c2->type == COLLIDER_VILAGER && c2->callback != nullptr)
			{
				if (App->scene->gamestate == INGAME)
				{
					if (dialog == nullptr)
					{
						//App->scene->gamestate = INMENU;
						if (direction == UP)
							c2->callback->direction = DOWN;
						else if (direction == DOWN)
							c2->callback->direction = UP;
						else if (direction == LEFT)
							c2->callback->direction = RIGHT;
						else
							c2->callback->direction = LEFT;
						Villager* villager = (Villager*)c2->callback;
						if (App->scene->last_map > 8 && App->scene->last_map != 10 && App->scene->last_map != 16)
						{
							if (App->scene->notrepeatCombat)
							{
								App->scene->cutscene_id = villager->cutscene_id;
								dialog = App->gui->CreateDialogue(villager->GetDialog().c_str(),2);
							}
							else
							{
								App->scene->cutscene_id = -1;
								dialog = App->gui->CreateDialogue("Congratulations! You defeated me. Go ahead and proceed with your run!",2);
							}
						}
						else
						{
							if (App->scene->last_map == 3)
							{
								dialog = App->gui->CreateDialogue(villager->GetDialog().c_str(),2);
							}
							if (App->scene->last_map == 5)
							{
								App->scene->playVideo = true;
								App->scene->combat_map_id = villager->switch_map;
								App->scene->useTP = true;
								dialog = App->gui->CreateDialogue(villager->GetDialog().c_str(),0);
								App->gui->GetEntity("pendant_poke")->visible = true;
							}
							else if (App->scene->last_map == 16)
							{
								dialog = App->gui->CreateDialogue(villager->GetDialog().c_str(),2);
								App->entity_elements->DeleteElement("door");
								App->gui->GetEntity("pendant_link")->visible = true;
								App->map->EditCost(32, 53, 0);
								App->map->EditCost(33, 53, 0);
								App->map->EditCost(32, 54, 0);
								App->map->EditCost(33, 54, 0);
							}
							else if (App->scene->last_map == 10)
							{
								if (App->scene->cash_swapped)
								{
									dialog = App->gui->CreateDialogue("You are in the Pokemon League! Here you will face the greatest trainers from Hoenn!",2);
								}
								else
								{
									dialog = App->gui->CreateDialogue(villager->GetDialog().c_str(),2);
									App->scene->joy_talk = true;
								}
							}
						}

						collision_interact->to_delete = true;
						interaction = false;
						state = L_IDLE;
						anim_state = L_IDLE;
					}
				}
			}
			// ----------------------

			// TRAINER INTERACTING -------------
			if (c1 == collision_interact && c2->type == COLLIDER_TRAINER && c2->callback != nullptr)
			{
				if (App->scene->gamestate == INGAME)
				{
					if (dialog == nullptr)
					{
						App->scene->gamestate = INMENU;
						if (direction == UP)
							c2->callback->direction = DOWN;
						else if (direction == DOWN)
							c2->callback->direction = UP;
						else if (direction == LEFT)
							c2->callback->direction = RIGHT;
						else
							c2->callback->direction = LEFT;
						dialog = App->gui->CreateDialogue("Hey, what are you doing in my world? Here we fight with creatures called pokemon, not with weapons, let's try it",2);
						collision_interact->to_delete = true;
						interaction = false;
						state = L_IDLE;
						anim_state = L_IDLE;
					}
				}
			}
			// ------------

			// GANON HIT ----------------
			if (c1 == collision_feet && c2->type == COLLIDER_GANON_ATTACK) //If green soldier attacks you
			{
				if (state != L_HIT && invincible_timer.ReadSec() >= 1 && state != L_DYING)
				{
					App->audio->PlayFx(13);
					state = L_HIT;
					anim_state = L_HIT;
					hurt_timer.Start();
					invincible_timer.Start();
					hp_hearts.y--;
					dir_hit = c2->callback->direction;
					prev_position = position;

					if (picked_object != nullptr) // Destroy the picked object if an enemy attacks you.
					{
						picked_object->SetState(D_IMPACTING);
						picked_object->start_impact = true;
						picked_object = nullptr;
					}

					if (interaction == true) // Delete interaction collider
					{
						collision_interact->to_delete = true;
						interaction = false;
					}
				}
			}
			// -----------------------			
			if (c1 == collision_interact && c2->type == COLLIDER_PC && c2->callback != nullptr)
			{
				if (App->scene->gamestate == INGAME)
				{
					App->scene->gamestate = INMENU;
					collision_interact->to_delete = true;
					interaction = false;
					state = L_IDLE;
					anim_state = L_IDLE;
					App->gui->SetGui(POKEMON_SHOP);
				}
			}
		}
	}
}

bool Player::Camera_inside(Direction direction) //TODO LOW -> Elliot
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
			if (camera_pos.x <= 0)
			{
				camera_pos.x = 0;
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

//STATE MACHINE ---------------------------------------------------------------------
bool Player::Idle()
{
	//TEST MOVE LINK
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_REPEAT ||
		App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_REPEAT)
	{
		state = L_WALKING;
		anim_state = L_WALKING;
	}

	else if ((App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_X) == EVENTSTATE::E_DOWN) && sword_equiped == true && picked_object == nullptr)
	{
		state = L_ATTACKING;
		anim_state = L_ATTACKING;
		current_animation = App->anim_manager->GetAnimation(anim_state, direction, LINK);
		current_animation->Reset();
	}

	else if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_DOWN)
	{
		if (picked_object == nullptr)
		{
			state = L_INTERACTING;
			anim_state = L_IDLE;
			//current_animation = App->anim_manager->GetAnimation(state, direction, 0);
			//current_animation->Reset();
		}
		else
		{
			ThrowObject();
		}
	}

	else
	{
		state = L_IDLE;
		anim_state = L_IDLE;
	}

	return true;
}

bool Player::Walking(float dt)
{
	walking = false;
	Move(dt);

	if (walking == false)
	{
		state = L_IDLE;
		anim_state = L_IDLE;
	}

	else if ((App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_X) == EVENTSTATE::E_DOWN) && sword_equiped == true && picked_object == nullptr)
	{
		state = L_ATTACKING;
		anim_state = L_ATTACKING;
		current_animation = App->anim_manager->GetAnimation(anim_state, direction, LINK);
		current_animation->Reset();
	}

	else if ((App->input->GetKey(SDL_SCANCODE_Q) || App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_DOWN) == KEY_DOWN)
	{
		if (picked_object == nullptr)
		{
			state = L_INTERACTING;
			anim_state = L_IDLE;
			//current_animation = App->anim_manager->GetAnimation(state, direction, 0);
			//current_animation->Reset();
		}
		else
		{
			ThrowObject();
		}
	}

	else
	{
		state = L_WALKING;
		anim_state = L_WALKING;
	}
	return true;
}

bool Player::Move(float dt)
{
	int keysuse = GetnuminputUse();
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_REPEAT)
	{
		direction = LEFT;
		int temp = App->map->MovementCost(collision_feet->rect.x - ceil(speed*dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT);
		if (temp == T_CONTINUE)
		{
			if (Camera_inside(direction))
				App->render->camera.x += ceil(speed*dt) * scale;
			position.x -= ceil(speed*dt);
		}
		if (keysuse == 1)
		{
			if (temp == T_UP)
			{
				direction = UP;
				if (Camera_inside(direction))
					App->render->camera.y += ceil(speed*dt) * scale;
				position.y -= ceil(speed*dt);
				direction = LEFT;
			}
			if (temp == T_DOWN)
			{
				direction = DOWN;
				if (Camera_inside(direction))
					App->render->camera.y -= ceil(speed*dt) * scale;
				position.y += ceil(speed*dt);
				direction = LEFT;
			}
		}
		walking = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_REPEAT)
	{
		direction = RIGHT;
		int temp = App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + ceil(speed*dt), collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT);
		if (temp == T_CONTINUE)
		{
			if (Camera_inside(direction))
				App->render->camera.x -= ceil(speed*dt) * scale;
			position.x += ceil(speed*dt);
		}
		if (keysuse == 1)
		{
			if (temp == T_UP)
			{
				direction = UP;
				if (Camera_inside(direction))
					App->render->camera.y += ceil(speed*dt) * scale;
				position.y -= ceil(speed*dt);
				direction = RIGHT;
			}
			if (temp == T_DOWN)
			{
				direction = DOWN;
				if (Camera_inside(direction))
					App->render->camera.y -= ceil(speed*dt) * scale;
				position.y += ceil(speed*dt);
				direction = RIGHT;
			}
		}

		walking = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_REPEAT)
	{
		direction = UP;
		int temp = App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - ceil(speed*dt), collision_feet->rect.w, collision_feet->rect.h, UP);
		if (temp == T_CONTINUE)
		{
			if (Camera_inside(direction))
				App->render->camera.y += ceil(speed*dt) * scale;
			position.y -= ceil(speed*dt);
		}
		if (keysuse == 1)
		{
			if (temp == T_LEFT)
			{
				direction = LEFT;
				if (Camera_inside(direction))
					App->render->camera.x += ceil(speed*dt) * scale;
				position.x -= ceil(speed*dt);
				direction = UP;
			}
			if (temp == T_RIGHT)
			{
				direction = RIGHT;
				if (Camera_inside(direction))
					App->render->camera.x -= ceil(speed*dt) * scale;
				position.x += ceil(speed*dt);
				direction = UP;
			}
		}
		walking = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_REPEAT)
	{
		direction = DOWN;
		int temp = App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + ceil(speed*dt), collision_feet->rect.w, collision_feet->rect.h, DOWN);
		if (temp == T_CONTINUE)
		{
			if (Camera_inside(direction))
				App->render->camera.y -= ceil(speed*dt) * scale;
			position.y += ceil(speed*dt);
		}
		if (keysuse == 1)
		{
			if (temp == T_LEFT)
			{
				direction = LEFT;
				if (Camera_inside(direction))
					App->render->camera.x += ceil(speed*dt) * scale;
				position.x -= ceil(speed*dt);
				direction = DOWN;
			}
			if (temp == T_RIGHT)
			{
				direction = RIGHT;
				if (Camera_inside(direction))
					App->render->camera.x -= ceil(speed*dt) * scale;
				position.x += ceil(speed*dt);
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

bool Player::Attack()
{
	if (attacker)
	{
		if (current_animation->Finished())
		{
			collision_attack->to_delete = true;
			attacker = false;
			current_animation->Reset();
			current_animation = nullptr;
			state = L_IDLE;
			anim_state = L_IDLE;
		}
	}
	else
	{
		attacker = true;
		if (direction == UP)
		{
			App->audio->PlayFx(5);
			collision_attack = App->collision->AddCollider({ position.x - 4, position.y - offset_y - 16, 8, 20 }, COLLIDER_SWORD, this);
		}
		else if (direction == RIGHT)
		{
			App->audio->PlayFx(5);
			collision_attack = App->collision->AddCollider({ position.x + 3, position.y - 8, 20, 8 }, COLLIDER_SWORD, this);
		}
		else if (direction == DOWN)
		{
			App->audio->PlayFx(5);
			collision_attack = App->collision->AddCollider({ position.x - 4, position.y - 2, 8, 20}, COLLIDER_SWORD, this);
		}
		else if (direction == LEFT)
		{
			App->audio->PlayFx(5);
			collision_attack = App->collision->AddCollider({ position.x - 22, position.y - 7, 20, 8 }, COLLIDER_SWORD, this);
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
			collision_interact->to_delete = true;
			collision_interact = nullptr;
			interaction = false;
			//current_animation->Reset();
			//current_animation = nullptr;
			state = L_IDLE;
			anim_state = L_IDLE;
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

void Player::ThrowObject()
{
	picked_object->Throw(direction);
	picked_object = nullptr;
}

bool Player::Hooking()
{
	if (hook != nullptr)
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
	}

	return true;
}

bool Player::Hit()
{
	// Delete collision attack
	if (collision_attack != nullptr)
	{
		collision_attack->to_delete = true;
	}

	// Check if hit is finished
	if (hurt_timer.ReadSec() >= 0.2)
	{
		state = L_IDLE;
		anim_state = L_IDLE;
		return true;
	}
	if (hp <= 0)
	{
		state = L_DYING;
		anim_state = L_IDLE;
		return true;
	}

	if (dir_hit == UP)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y - 3, collision_feet->rect.w, collision_feet->rect.h, DOWN) == 0)
		{
			if (Camera_inside(dir_hit))
				App->render->camera.y += 3 * scale;

			position.y -= 3;
		}
	}

	else if (dir_hit == DOWN)
	{
		if (App->map->MovementCost(collision_feet->rect.x, collision_feet->rect.y + collision_feet->rect.h + 2.5, collision_feet->rect.w, collision_feet->rect.h, UP) == 0)
		{
			if (Camera_inside(dir_hit))
				App->render->camera.y -= 3 * scale;

			position.y += 3;
		}
	}
	else if (dir_hit == LEFT)
	{
		if (App->map->MovementCost(collision_feet->rect.x - 3, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, RIGHT) == 0)
		{
			if (Camera_inside(dir_hit))
				App->render->camera.x += 3 * scale;

			position.x -= 3;
		}
	}
	else if (dir_hit == RIGHT)
	{
		if (App->map->MovementCost(collision_feet->rect.x + collision_feet->rect.w + 3, collision_feet->rect.y, collision_feet->rect.w, collision_feet->rect.h, LEFT) == 0)
		{
			if (Camera_inside(dir_hit))
				App->render->camera.x -= 3 * scale;

			position.x += 3;
		}
	}


	return true;
}

//----------------------------------------------------------------------------------

//EQUIP, UNEQUIP AND USE ITEMS
bool Player::Equip(Weapon* to_equip)
{
	if (to_equip != nullptr)
	{
		if (equiped_item == nullptr && to_equip->equipable == true)
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
		if (equiped_item->in_use == false)
		{
			LOG("Unequiped %s", equiped_item->name.c_str());
			equiped_item->equiped = false;
			equiped_item = nullptr;
			ret = true;
		}
	}
	return ret;
}

//HOOKSHOT  TODO JORDI ----------------------------------
void Player::KeepGoing()
{
	if (hook != nullptr)
	{
		switch (direction)
		{
		case UP:
			hook->position.y -= hook->speed;
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
}

void Player::PickUpHook()
{
	if (hook != nullptr)
	{
		switch (direction)
		{
		case UP:
			hook->position.y += hook->speed;
			if (hook->position.y + hook->offset_y >= collision_feet->rect.y)
			{
				hook->Reset();
				state = L_IDLE;
				anim_state = L_IDLE;
			}
			break;
		case DOWN:
			hook->position.y -= hook->speed;
			if (hook->position.y <= collision_feet->rect.y + collision_feet->rect.h)
			{
				hook->Reset();
				state = L_IDLE;
				anim_state = L_IDLE;
			}
			break;
		case LEFT:
			hook->position.x += hook->speed;
			if (hook->position.x + hook->offset_x >= collision_feet->rect.x)
			{
				hook->Reset();
				state = L_IDLE;
				anim_state = L_IDLE;
			}
			break;
		case RIGHT:
			hook->position.x -= hook->speed;
			if (hook->position.x <= collision_feet->rect.x + collision_feet->rect.w)
			{
				hook->Reset();
				state = L_IDLE;
				anim_state = L_IDLE;
			}
			break;
		default:
			break;
		}
	}

}

void Player::MoveTo(const iPoint& pos)
{
	switch (direction)
	{
	case UP:
	{
		//int temp = App->map->MovementCost(position.x, position.y - hook->speed, UP);
		if (Camera_inside(direction))
			App->render->camera.y += hook->speed * scale;

		position.y -= hook->speed;

		if (hook->position.y >= position.y - offset_y)
		{
			hook->Reset();
			state = L_IDLE;
			anim_state = L_IDLE;
		}
		break;
	}

	case DOWN:
	{
		//int temp = App->map->MovementCost(position.x, position.y + (hook->speed + height), DOWN
		if (Camera_inside(direction))
			App->render->camera.y -= hook->speed * scale;
		position.y += hook->speed;

		if (hook->position.y <= position.y + offset_y)
		{
			hook->Reset();
			state = L_IDLE;
			anim_state = L_IDLE;
		}
		break;
	}

	case LEFT:
	{
		//int temp = App->map->MovementCost(position.x - hook->speed, position.y, LEFT);

		if (Camera_inside(direction))
			App->render->camera.x += hook->speed * scale;
		position.x -= hook->speed;

		if (hook->position.x >= position.x - offset_x)
		{
			hook->Reset();
			state = L_IDLE;
			anim_state = L_IDLE;
		}
		break;
	}

	case RIGHT:
	{
		//int temp = App->map->MovementCost(position.x + (hook->speed + width), position.y, RIGHT

		if (Camera_inside(direction))
			App->render->camera.x -= hook->speed * scale;
		position.x += hook->speed;

		if (hook->position.x <= position.x + offset_x)
		{
			hook->Reset();
			state = L_IDLE;
			anim_state = L_IDLE;
		}
		break;
	}

	default:
		break;
	}
}

void Player::BowAnim()
{
	if (current_animation->Finished())
	{
		current_animation->Reset();
		current_animation = nullptr;
		state = L_IDLE;
		anim_state = L_IDLE;
	}
}

void Player::ThrowHookshot(uint charge)
{
	if (hook != nullptr)
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

}

//-----------------

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

void Player::GetfloorLvl(iPoint pos)
{
	const MapLayer* meta_layer = App->map->data.layers[App->map->data.layers.size() - 1]; //WEAPONS LAYER
	iPoint map_pos = App->map->WorldToMap(pos.x, pos.y);
	int player_lvl = meta_layer->Get(map_pos.x, map_pos.y);

	const TileSet* tileset = App->map->data.tilesets[0]; //META TILESET
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
}

int Player::GetFloor() const
{
	return actual_floor;
}

void Player::AddHeartContainer()
{
	iPoint last_heart_pos = App->scene->hud->hp->elements.back()->position - App->scene->hud->hp->position;
	Image* newhp = App->gui->CreateImage({ 177,24,7,7 }, { last_heart_pos.x + 9,last_heart_pos.y }, "hp_add", ZELDA_HUD);
	App->scene->hud->hp->elements.push_back(newhp);
	newhp->parent = App->scene->hud->hp;
	newhp->CalculateDiferential();
	hp_hearts.x += 2;
	hp_hearts.y = hp_hearts.x;
}

void Player::ShowHearts()
{
	int addheart = hp_hearts.y;
	for (int i = 0; i < App->scene->hud->hp->elements.size(); i++)
	{
		if (addheart - 2 >= 0)
		{
			App->scene->hud->hp->elements[i]->Hitbox.x = 161;
			addheart -= 2;
		}
		else if (addheart - 1 >= 0)
		{
			App->scene->hud->hp->elements[i]->Hitbox.x = 169;
			addheart--;
		}
		else if (addheart == 0)
		{
			App->scene->hud->hp->elements[i]->Hitbox.x = 177;
		}
	}
}

void Player::GetDamage()
{
	if (hp_hearts.y>0)
		hp_hearts.y--;
}

void Player::SetKnockbackDir()
{
	if (direction == UP)
	{
		dir_hit = DOWN;
	}
	else if (direction == DOWN)
	{
		dir_hit = UP;
	}
	else if (direction == LEFT)
	{
		dir_hit = RIGHT;
	}
	else
	{
		dir_hit = LEFT;
	}
}


LinkState Player::GetState() const
{
	return state;
}

void Player::SetState(LinkState s_state)
{
	state = s_state;
}

void Player::SetAnimState(LinkState a_state)
{
	anim_state = a_state;
}

bool Player::CameraisIn() //Comprovate if the camera is inside the Map
{
	bool ret = false;
	iPoint camera_pos(-App->render->camera.x / 2, -App->render->camera.y / 2);
	iPoint size_map = App->map->MapToWorld(App->map->data.width, App->map->data.height);
	if (camera_pos.x < 0)
	{
		camera_pos.x = 0;
		ret = true;
	}
	if (camera_pos.x > size_map.x - (App->win->GetWidth() / scale) / 2)
	{
		camera_pos.x = size_map.x - (App->win->GetWidth() / scale) / 2;
		ret = true;
	}
	if (camera_pos.y < 0)
	{
		camera_pos.y = 0;
		ret = true;
	}
	if (camera_pos.y > size_map.y - (App->win->GetHeight() / scale) / 2)
	{
		camera_pos.y = size_map.y - (App->win->GetHeight() / scale) / 2;
		ret = true;
	}
	return ret;
}

void Player::SaveCheckPoint(int map_id)
{
	checkpoint.pos = position;
	checkpoint.map_id = map_id;
	checkpoint.hp_hearts = hp_hearts;

	if (App->gui->GetGuiState() >= ZELDA_HUD && App->gui->GetGuiState() < POKEMON_COMBAT)
	{
		checkpoint.world = "Zelda";
	}
	else if (App->gui->GetGuiState() >= POKEMON_COMBAT && App->gui->GetGuiState() < MAPPING_CONTROLLER)
	{
		checkpoint.world = "Pokemon";
	}

	checkpoint.arrows = arrows;
	checkpoint.bombs = bombs;
	checkpoint.rupees = gems;

	if (bow != nullptr)
	{
		checkpoint.bow_picked = true;
	}
	if (bombmanager != nullptr)
	{
		checkpoint.bombcontainer_picked = true;
	}
	if (hook != nullptr)
	{
		checkpoint.hookshot_picked = true;
	}
	if (sword_equiped == true)
	{
		checkpoint.sword_picked = true;
	}

	checkpoint.cash = pokecash;

	//POKEMON ITEMS & CREATURES ----------
	App->combatmanager->SaveItemsCheckPoint();
	checkpoint.blaz_pos = blaz_pos;
	checkpoint.scept_pos = scept_pos;
	checkpoint.swamp_pos = swamp_pos;
	// -----------------------

	LOG("CHECKPOINT SAVED ---");
	LOG("Map: %i", checkpoint.map_id);
	LOG("Position: (%i,%i)", checkpoint.pos);
	LOG("Max HP: %i, Current HP: %i", checkpoint.hp_hearts.x, checkpoint.hp_hearts.y);
	LOG("Rupees: %i, Arrows: %i, Bombs: %i", checkpoint.rupees, checkpoint.arrows, checkpoint.bombs);
	if (checkpoint.bow_picked == true)
	{
		LOG("Bow PICKED", checkpoint.pos);
	}
	if (checkpoint.bombcontainer_picked == true)
	{
		LOG("Bomb Container PICKED", checkpoint.pos);
	}
	if (checkpoint.hookshot_picked == true)
	{
		LOG("Hookshot PICKED", checkpoint.pos);
	}
	
	LOG("WORLD: %s", checkpoint.world.c_str());
	LOG("PokeCash: %i", checkpoint.cash);
	LOG("BLAZIKEN = hp: %i, attack: %i, defense: %i pos: %i", checkpoint.blaz_hp, checkpoint.blaz_atk, checkpoint.blaz_def, checkpoint.blaz_pos);
	LOG("SCEPTYLE = hp: %i, attack: %i, defense: %i pos: %i", checkpoint.scept_hp, checkpoint.scept_atk, checkpoint.scept_def, checkpoint.scept_pos);
	LOG("SWAMPERT = hp: %i, attack: %i, defense: %i pos: %i", checkpoint.swamp_hp, checkpoint.swamp_atk, checkpoint.swamp_def, checkpoint.swamp_pos);
	LOG("--------------------");

}

void Player::LoadStats()
{
	hp_hearts.x = App->scene->Check.hp_hearts.x;
	hp_hearts.y = App->scene->Check.hp_hearts.y;

	gems = App->scene->Check.rupees;
	arrows = App->scene->Check.arrows;
	bombs = App->scene->Check.bombs;
	pokecash = App->scene->Check.cash;

	// WEAPONS -----------------------------------------------------
	if (App->scene->Check.bow_picked == true)
	{
		bow = App->entity_elements->CreateBow();
	}
	if (App->scene->Check.bombcontainer_picked == true)
	{
		bombmanager = App->entity_elements->CreateBombContainer();
	}
	if (App->scene->Check.hookshot_picked == true)
	{
		hook = App->entity_elements->CreateHookshot();
	}
	if (App->scene->Check.sword_picked == true)
	{
		sword_equiped = true;
	}
	// ---------------------------------------------------------------

	// POKEMON ITEMS & CREATURES ----------------------------------
	App->combatmanager->LoadPokemonItems();
	LoadPokemonPositions();
	// ------------------------------------------------

	// UI -----------------------------------------------
	if (App->scene->Check.world == "Zelda")
	{
		App->gui->SetGui(ZELDA_HUD);
	}
	if (App->scene->Check.world == "Pokemon")
	{
		App->gui->SetGui(POKEMON_HUD);		
		App->scene->poke_hud->SetPokeOrder(blaz_pos, scept_pos, swamp_pos);
	}
	// -----------------------------------------------------

	SaveCheckPoint(App->scene->Check.map_id);
}

void Player::SavePokemonPositions()
{
	//GET THE POSITION OF THE POKEMON FROM UI
}

void Player::LoadPokemonPositions()
{
	blaz_pos = App->scene->Check.blaz_pos;
	scept_pos = App->scene->Check.scept_pos;
	swamp_pos = App->scene->Check.swamp_pos;
}

