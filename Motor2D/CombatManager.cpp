#include "CombatManager.h"
#include "j1Item.h"
#include "j1Player.h"
#include "j1DynamicObjects.h"
#include "j1Scene.h"
#include "j1App.h"
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
#include "PokeTrainer.h"


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

	return ret;
}

bool CombatManager::PreUpdate()
{

	return true;
}

bool CombatManager::Update(float dt)
{
	BROFILER_CATEGORY("DoUpdate_ComabatPokemon", Profiler::Color::Cyan);

	std::list<SceneElement*>::iterator item = elementcombat.begin();
	if (App->scene->combat)
	{
		/*if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			Pokemon* temp = (Pokemon*)item._Ptr->_Myval;
			Pokemon* temp_2 = (Pokemon*)item._Ptr->_Next->_Myval;
			Pokemon* temp_3 = (Pokemon*)item._Ptr->_Next->_Next->_Myval;
			temp->active = true;
			temp_2->active = false;
			temp_3->active = false;
		}
		if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
		{
			Pokemon* temp = (Pokemon*)item._Ptr->_Myval;
			Pokemon* temp_2 = (Pokemon*)item._Ptr->_Next->_Myval;
			Pokemon* temp_3 = (Pokemon*)item._Ptr->_Next->_Next->_Myval;
			temp->active = false;
			temp_2->active = true;
			temp_3->active = false;
		}
		if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
		{
			Pokemon* temp = (Pokemon*)item._Ptr->_Myval;
			Pokemon* temp_2 = (Pokemon*)item._Ptr->_Next->_Myval;
			Pokemon* temp_3 = (Pokemon*)item._Ptr->_Next->_Next->_Myval;
			temp->active = false;
			temp_2->active = false;
			temp_3->active = true;
		}*/
		while (item != elementcombat.end())
		{
			Pokemon* temp = (Pokemon*)item._Ptr->_Myval;
			if (temp->active)
			{
				item._Ptr->_Myval->Update(dt);
			}
			item++;
		}
	}


	return true;
}

bool CombatManager::PostUpdate()
{
	BROFILER_CATEGORY("Draw_ComabatPokemon", Profiler::Color::Green);

	std::list<SceneElement*>::iterator item = elementcombat.begin();
	while (item != elementcombat.end())
	{
		Pokemon* temp = (Pokemon*)item._Ptr->_Myval;
		if (temp->active)
		{
			item._Ptr->_Myval->Draw();
		}
		item++;
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
	/*std::list<SceneElement*>::iterator item = elementcombat.begin();
	while (item != elementcombat.end())
	{
		Pokemon* temp = (Pokemon*)item._Ptr->_Myval;
		if (temp->active)
		{
			item._Ptr->_Myval->Draw();
		}
		item++;
	}*/
}

Pokemon* CombatManager::CreatePokemon(pugi::xml_node& conf, uint id)
{
	if (id == 1)
	{
		Blaziken* temp = new Blaziken();
		temp->Awake(conf);
		temp->Start();
		elementcombat.push_back(temp);
		return temp;
	}
	if (id == 2)
	{
		Sceptyle* temp = new Sceptyle();
		temp->Awake(conf);
		temp->Start();
		elementcombat.push_back(temp);
		return temp;
	}
	if (id == 3)
	{
		Swampert* temp = new Swampert();
		temp->Awake(conf);
		temp->Start();
		elementcombat.push_back(temp);
		return temp;
	}
}

PokeTrainer* CombatManager::CreateTrainer(pugi::xml_node& conf, uint id)
{
	PokeTrainer* temp = new PokeTrainer();
	temp->Awake(conf);
	temp->Start();
	elementcombat.push_back(temp);
	return temp;
}

bool CombatManager::DeleteElements_combat()
{
	std::list<SceneElement*>::iterator item = elementcombat.begin();
	while (item != elementcombat.end())
	{
		delete item._Ptr->_Myval;
		elementcombat.erase(item);
		item++;
	}
	return true;
}

Pokemon* CombatManager::change_pokemon(bool trainer)//true Link - false Brendan
{
	if (trainer) //Link
	{
		std::list<Pokemon*>::iterator item = App->scene->player->pokedex.begin();
		while (item != App->scene->player->pokedex.end())
		{
			if (item._Ptr->_Myval->active == true)
			{
				item._Ptr->_Myval->active = false;
				item++;
				if (item == App->scene->player->pokedex.end())
				{
					return nullptr;
				}
				else
				{
					item._Ptr->_Myval->active = true;
					return item._Ptr->_Myval;
				}
			}
			item++;
		}
		return nullptr;
	}
	else //Brendan
	{
		std::list<Pokemon*>::iterator item = App->scene->poketrainer.begin()._Ptr->_Myval->pokedex.begin();
		while (item != App->scene->poketrainer.begin()._Ptr->_Myval->pokedex.end())
		{
			if (item._Ptr->_Myval->active == true)
			{
				item._Ptr->_Myval->active = false;
				item++;
				if (item == App->scene->poketrainer.begin()._Ptr->_Myval->pokedex.end())
				{
					return nullptr;
				}
				else if(item._Ptr->_Myval!=nullptr)
				{
					item._Ptr->_Myval->active = true;
					return item._Ptr->_Myval;
				}
			}
			item++;
		}
		return nullptr;
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