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
	BROFILER_CATEGORY("DoUpdate_ComabatPokemon", Profiler::Color::Cyan)
	std::list<SceneElement*>::iterator item = elementcombat.begin();
	while (item != elementcombat.end())
	{
		Pokemon* temp = (Pokemon*)item._Ptr->_Myval;
		if (temp->active)
		{
			item._Ptr->_Myval->Update();
		}
		item++;
	}
	return true;
}

bool CombatManager::PostUpdate()
{
	BROFILER_CATEGORY("Draw_ComabatPokemon", Profiler::Color::Green)

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
	Blaziken* temp = new Blaziken();
	temp->Awake(conf);
	temp->Start();
	elementcombat.push_back(temp);
	return temp;
}

bool CombatManager::CreateTrainer(pugi::xml_node& conf, uint id)
{
	PokeTrainer* temp = new PokeTrainer();
	temp->Awake(conf);
	temp->Start();
	elementcombat.push_back(temp);
	return false;
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