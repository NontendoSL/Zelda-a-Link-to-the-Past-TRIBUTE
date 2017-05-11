#include "CombatManager.h"
#include "j1Player.h"
#include "j1DynamicObjects.h"
#include "j1Scene.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Creature.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1FileSystem.h"
#include "Pokemon.h"
#include "Swampert.h"
#include "Sceptyle.h"
#include "Blaziken.h"
#include "Salamance.h"
#include "Walrein.h"
#include "Dusclops.h"
#include "Groudon.h"
#include "Shiftry.h"
#include "PokeTrainer.h"
#include "j1Gui.h"
#include "j1GuiElements.h"
#include "j1GuiEntity.h"


CombatManager::CombatManager()
{
	name = "combatmanager";
}

CombatManager::~CombatManager()
{
}

bool CombatManager::Awake(pugi::xml_node &config)
{

	return true;
}

bool CombatManager::Start()
{
	bool ret = true;
	for (int i = 0; i < 3; i++)
	{
		Item_pokeCombat id;
		bag_items.push_back(id);
	}
	return ret;
}

bool CombatManager::PreUpdate()
{

	return true;
}

bool CombatManager::Update(float dt)
{
	BROFILER_CATEGORY("DoUpdate_ComabatPokemon", Profiler::Color::Cyan);

	//Pokemon of Player is dead

	std::list<SceneElement*>::iterator item = elementcombat.begin();
	if (App->scene->combat)
	{
		while (item != elementcombat.end())
		{
			if (item._Ptr->_Myval->type == CREATURE)
			{
				PokemonCombat* poke = (PokemonCombat*)item._Ptr->_Myval;
				if (poke->active)
				{
					if (poke->hp <= 0)
					{
						if (pokemon_active_link == poke)
						{
							pokemon_active_link->active = false;
							poke->collision_feet->to_delete = true;
							if (poke->collision_attack != nullptr)
								poke->collision_attack->to_delete = true;
							if (poke->sp_attack != nullptr)
								poke->sp_attack->to_delete = true;
							elementcombat.erase(item);
							item++;
							pokemon_order++;
							change_pokemon();
						}
						else //pokemon_active_trainer == poke
						{
							App->scene->combat = false;
							App->scene->switch_map = App->scene->last_map;
							App->scene->useTP = true;
							App->scene->player->state_complet = true;
							item++;
						}
					}
					else
					{
						item._Ptr->_Myval->Update(dt);
						item++;
					}
				}
			}
			else
			{
				item._Ptr->_Myval->Update(dt);
				item++;
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_UP)
		{
			App->scene->pokecombat->cooldown = false;
		}
		if (App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		{
			pokemon_active_trainer->hp -= 1000;
		}
		if (App->input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
		{
			pokemon_active_link->hp -= 1000;
			App->scene->pokecombat->cooldown = false;
		}
	}
	return true;
}

bool CombatManager::PostUpdate()
{
	BROFILER_CATEGORY("Draw_ComabatPokemon", Profiler::Color::Green);
	if (App->scene->combat)
	{
		std::list<SceneElement*>::iterator item = elementcombat.begin();
		while (item != elementcombat.end())
		{
			if (item._Ptr->_Myval->type == DYNOBJECT)
			{
				item._Ptr->_Myval->Draw();
			}
			else
			{
				PokemonCombat* temp = (PokemonCombat*)item._Ptr->_Myval;
				if (temp->active)
				{
					item._Ptr->_Myval->Draw();
				}
			}
			item++;
		}
	}
	return true;
}

bool CombatManager::CleanUp()
{
	bool ret = true;
	std::list<SceneElement*>::iterator item = elementcombat.begin();
	while (item != elementcombat.end())
	{
		item._Ptr->_Myval->CleanUp();
		item++;
	}
	return ret;
}

void CombatManager::CreateTargets()
{
}

void CombatManager::CreateDynObject(iPoint pos, uint id, uint id_map)
{
	DynamicObjects* element = new DynamicObjects();
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);
	bool stop_rearch = false;
	LOG("Create DynObjects");
	config = config.child("map_combat").child("map");
	for (; stop_rearch == false; config = config.next_sibling())
	{
		if (config.attribute("n").as_int(0) == id_map)
		{
			element->Awake(config, id, pos);
			element->Start();
			elementcombat.push_front(element);
			LOG("Created!!");
			stop_rearch = true;
		}
	}
}

int CombatManager::Getsize_elements()
{
	return elementcombat.size();
}

bool CombatManager::GiveItem(int id_pokemon, const char* item_name)
{
	if (bag_items[id_pokemon].items_equiped < 3)
	{
		if (item_name == "pk_bag:HP UP")
		{
			if (bag_items[id_pokemon].hp_up >= 3)
			{
				return false;
			}
			else
			{
				LOG("Item Equiped!");
				bag_items[id_pokemon].hp_up++;
				bag_items[id_pokemon].items_equiped++;
				return true;
			}
		}
		else if (item_name == "pk_bag:DEF PROTEIN")
		{
			if (bag_items[id_pokemon].def_protein >= 3)
			{
				return false;
			}
			else
			{
				LOG("Item Equiped!");
				bag_items[id_pokemon].def_protein++;
				bag_items[id_pokemon].items_equiped++;
				return true;
			}
		}
		else if (item_name == "pk_bag:X ATTACK")
		{
			if (bag_items[id_pokemon].x_attack >= 3)
			{
				return false;
			}
			else
			{
				LOG("Item Equiped!");
				bag_items[id_pokemon].x_attack++;
				bag_items[id_pokemon].items_equiped++;
				return true;
			}
		}
		else
		{
			LOG("Error name!");
			return false;
		}
	}
	else
	{
		LOG("Can't equip more items!");
		return false;
	}
}

PokemonCombat* CombatManager::CreatePokemon(pugi::xml_node& conf, uint id)
{
	if (id == 1)
	{
		Blaziken* temp = new Blaziken();
		temp->Awake(conf);
		//temp->Start();
		//elementcombat.push_back(temp);
		return temp;
	}
	else if (id == 2)
	{
		Sceptyle* temp = new Sceptyle();
		temp->Awake(conf);
		//temp->Start();
		//elementcombat.push_back(temp);
		return temp;
	}
	else if (id == 3)
	{
		Swampert* temp = new Swampert();
		temp->Awake(conf);
		//temp->Start();
		//elementcombat.push_back(temp);
		return temp;
	}
	else if (id == 4)
	{
		Walrein* temp = new Walrein();
		temp->Awake(conf);
		temp->Start();
		elementcombat.push_back(temp);
		pokemon_active_trainer = (PokemonCombat*)elementcombat.back();
		return temp;
	}
	else if (id == 5)
	{
		Salamance* temp = new Salamance();
		temp->Awake(conf);
		temp->Start();
		elementcombat.push_back(temp);
		pokemon_active_trainer = (PokemonCombat*)elementcombat.back();
		return temp;
	}
	else if (id == 6)
	{
		Shiftry* temp = new Shiftry();
		temp->Awake(conf);
		temp->Start();
		elementcombat.push_back(temp);
		pokemon_active_trainer = (PokemonCombat*)elementcombat.back();
		return temp;
	}
	else if (id == 7)
	{
		Dusclops* temp = new Dusclops();
		temp->Awake(conf);
		temp->Start();
		elementcombat.push_back(temp);
		pokemon_active_trainer = (PokemonCombat*)elementcombat.back();
		return temp;
	}
	else if (id == 8)
	{
		Groudon* temp = new Groudon();
		temp->Awake(conf);
		temp->Start();
		elementcombat.push_back(temp);
		pokemon_active_trainer = (PokemonCombat*)elementcombat.back();
		return temp;
	}
	else
	{
		LOG("ERROR id");
	}
	return nullptr;
}

void CombatManager::CreateTrainer(pugi::xml_node& conf, uint id)
{
	/*PokeTrainer* temp = new PokeTrainer();
	temp->Awake(conf);
	temp->Start();
	trainer = temp;*/
}

bool CombatManager::DeleteElements_combat()
{
	// First nullptr the targets
	pokemon_active_trainer = nullptr;
	pokemon_active_link = nullptr;
	// Delete from List
	std::list<SceneElement*>::iterator item = elementcombat.begin();
	while (item != elementcombat.end())
	{
		if (item._Ptr->_Myval->name != "BLAZIKEN")
		{
			delete item._Ptr->_Myval;
		}

		elementcombat.erase(item);
		item++;
	}
	std::list<PokemonCombat*>::iterator Link_pokedex = App->scene->player->pokedex.begin();
	while (Link_pokedex != App->scene->player->pokedex.end())
	{
		Link_pokedex._Ptr->_Myval->active = false;
		Link_pokedex++;
	}

	elementcombat.clear();
	id_map_combat = 0;
	return true;
}

void CombatManager::PrepareToCombat(PokemonCombat* pokemon, uint idMap, int id_pokemon)
{
	if (pokemon->active == false)
	{
		pugi::xml_document	config_file;
		pugi::xml_node		config;
		config = LoadConfig(config_file);
		config = config.child("map_combat").child("map");
		for (; config != NULL; config = config.next_sibling())
		{
			if (config.attribute("n").as_int(0) == idMap)
			{
				pokemon->active = true;
				pokemon->Start();
				ModifyStats(config, pokemon, bag_items[id_pokemon - 1]);
				pokemon->target = pokemon_active_trainer;
				pokemon_active_trainer->target = pokemon;
				elementcombat.push_back(pokemon);
				pokemon_active_link = (PokemonCombat*)elementcombat.back();
			}
		}
	}
}

void CombatManager::ModifyStats(pugi::xml_node& conf, PokemonCombat* pokemon, Item_pokeCombat bag)
{
	pokemon->position.x = conf.child("poke_Link").attribute("pos_x").as_int(0);
	pokemon->position.y = conf.child("poke_Link").attribute("pos_y").as_int(0);
	pokemon->ModifyStats(&bag);
}

PokemonCombat* CombatManager::change_pokemon()//true Link - false Brendan
{
	//Pokemon Link
	if (App->scene->poke_hud->GetPokeOrder(pokemon_order) == "pk_bar_blaziken")
	{
		PrepareToCombat(App->scene->player->pokedex.begin()._Ptr->_Myval, id_map_combat, 1);
		return App->scene->player->pokedex.begin()._Ptr->_Myval;
	}
	else if (App->scene->poke_hud->GetPokeOrder(pokemon_order) == "pk_bar_sceptile")
	{
		PrepareToCombat(App->scene->player->pokedex.begin()._Ptr->_Next->_Myval, id_map_combat, 2);
		return App->scene->player->pokedex.begin()._Ptr->_Next->_Myval;
	}
	else
	{
		PrepareToCombat(App->scene->player->pokedex.begin()._Ptr->_Next->_Next->_Myval, id_map_combat, 3);
		return App->scene->player->pokedex.begin()._Ptr->_Next->_Next->_Myval;
	}
	return nullptr;
}


// ---------------------------------------------
pugi::xml_node CombatManager::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = App->fs->Load("Levels.xml", &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("levels");

	return ret;
}