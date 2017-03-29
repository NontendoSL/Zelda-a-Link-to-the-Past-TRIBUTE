#include "CombatManager.h"
#include "Soldier.h"
#include "j1Item.h"
#include "j1Player.h"
#include "j1DynamicObjects.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Input.h"
#include "p2Log.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1FileSystem.h"


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
	std::list<SceneElement*>::iterator item = elementscene.begin();
	while (item != elementscene.end())
	{
		//item._Ptr->_Myval->Start();
		item++;
	}
	char* buf;
	int size = App->fs->Load("config.xml", &buf);
	XML.load_buffer(buf, size);
	return ret;
}

bool CombatManager::PreUpdate()
{
	BROFILER_CATEGORY("DoUpdate_Elements", Profiler::Color::Cyan)
		std::list<SceneElement*>::iterator item3 = elementscene.begin();
	while (item3 != elementscene.end())
	{
		item3._Ptr->_Myval->Update();
		item3++;
	}
	return true;
}

bool CombatManager::Update(float dt)
{
	BROFILER_CATEGORY("Draw_Elements", Profiler::Color::Green)
		bool ret = true;

	std::list<SceneElement*>::iterator item = elementscene.end();
	item--;
	while (item != elementscene.begin()) //TODO HIGH -> need inverse_iterator
	{
		item._Ptr->_Myval->Draw();
		item--;
	}
	if (elementscene.size() > 0)
	{
		item._Ptr->_Myval->Draw();
	}

	return ret;
}

bool CombatManager::PostUpdate()
{

	return true;
}

bool CombatManager::CleanUp()
{
	bool ret = true;
	std::list<SceneElement*>::iterator item = elementscene.begin();
	while (item != elementscene.end())
	{
		item._Ptr->_Myval->CleanUp();
		item++;
	}
	return ret;
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