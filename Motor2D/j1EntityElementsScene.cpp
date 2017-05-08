#include "j1EntityElementsScene.h"
#include "Soldier.h"
#include "j1Item.h"
#include "j1Player.h"
#include "j1DynamicObjects.h"
#include "j1Scene.h"
#include "j1App.h"
#include "j1Input.h"
#include "p2Log.h"
#include "Geodude.h"
#include "Sudowoodo.h"
#include "Golem.h"
#include "PokeTrainer.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Collision.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1FileSystem.h"
#include "BCTrooper.h"
#include "j1Weapon.h"
#include "Vilager.h"


j1EntityElementScene::j1EntityElementScene()
{
	name = "entityelement";
}

j1EntityElementScene::~j1EntityElementScene()
{
}

bool j1EntityElementScene::Awake(pugi::xml_node &config)
{
	/*std::list<SceneElement*>::iterator item = elementscene.begin();
	while (item != elementscene.end())
	{

		item++;
	}*/
	file_tex_dynobjects = config.child("textDynObjects").attribute("file").as_string("");
	file_tex_trainer = config.child("textBrendan").attribute("file").as_string("");
	return true;
}

bool j1EntityElementScene::Start()
{
	bool ret = true;
	std::list<SceneElement*>::iterator item = elementscene.begin();
	while (item != elementscene.end())
	{
		//item._Ptr->_Myval->Start();
		item++;
	}
	
	texture_dynobjects = App->tex->Load(file_tex_dynobjects.c_str());
	texture_trainer = App->tex->Load(file_tex_trainer.c_str());

	char* buf;
	int size = App->fs->Load("config.xml", &buf);
	XML.load_buffer(buf, size);
	return ret;
}

bool j1EntityElementScene::PreUpdate()
{
	BROFILER_CATEGORY("DoUpdate_Elements", Profiler::Color::Cyan);


	return true;
}

bool j1EntityElementScene::Update(float dt)
{
	bool ret = true;
	BROFILER_CATEGORY("DoUpdate_Elements", Profiler::Color::Cyan);
	if (App->scene->combat == false)
	{
		std::list<SceneElement*>::iterator item3 = elementscene.begin();
		while (item3 != elementscene.end())
		{
			item3._Ptr->_Myval->Update(dt);

			if (((DynamicObjects*)item3._Ptr->_Myval)->GetState() == D_DYING)
			{
				//TODO -> if animation finished, then delete.
				App->entity_elements->DeleteDynObject(((DynamicObjects*)item3._Ptr->_Myval)); // Delete Dynobject
				App->audio->PlayFx(17);
			}
			if (bct != nullptr)
			{
				if (((BCTrooper*)item3._Ptr->_Myval)->GetState() == BC_DYING)
				{
					//TODO -> if animation finished, then delete.
					App->entity_elements->DeleteBCTrooper((BCTrooper*)item3._Ptr->_Myval); // Delete Dynobject
					App->audio->PlayFx(17);
				}
			}


			item3++;
		}
	}

	return ret;
}

bool j1EntityElementScene::PostUpdate()
{
	BROFILER_CATEGORY("Draw_Elements", Profiler::Color::Green)
	if (App->scene->combat == false)
	{
		std::list<SceneElement*>::iterator item = elementscene.end();
		item--;
		while (item != elementscene.begin())
		{
			item._Ptr->_Myval->Draw();
			item--;
		}
		if (elementscene.size() > 0)
		{
			item._Ptr->_Myval->Draw();
		}
	}

	//Draw Floor 2-----------------------
	App->map->Draw(true);

	return true;
}

bool j1EntityElementScene::CleanUp()
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

bool j1EntityElementScene::DelteWeapons()
{
	std::list<SceneElement*>::iterator item = elementscene.end();
	item--;
	{
		while (item != elementscene.begin())
		{
			if (item._Ptr->_Myval->type == WEAPON)
			{
				delete item._Ptr->_Myval;
				elementscene.erase(item);
			}
			item--;
		}
	}
	return true;
}

bool j1EntityElementScene::DelteElements()
{
	App->collision->waittodelete = true;
	std::list<SceneElement*>::iterator item = elementscene.end();
	item--;
	if (elementscene.begin()._Ptr->_Myval->name != "Link")
	{
		std::list<SceneElement*>::iterator temp = elementscene.begin();
		while (temp != elementscene.end())
		{
			if (temp._Ptr->_Myval->name == "Link")
			{
				std::swap(temp._Ptr->_Myval, elementscene.begin()._Ptr->_Myval);
			}
			temp++;
		}
	}
	if (elementscene.size() > 1)
	{
		while (item != elementscene.begin())
		{
			if (item._Ptr->_Myval->type != WEAPON)
			{
				delete item._Ptr->_Myval;
				elementscene.erase(item);
			}
			item--;
		}
	}
	return true;
}

void j1EntityElementScene::CreateSoldier(uint id, pugi::xml_node& config)
{
	Soldier* element = new Soldier();
	element->Awake(config, id);
	element->Start();
	elementscene.push_back(element);
}

bool j1EntityElementScene::DeleteEnemy(Soldier* enemy)
{
	if (enemy != nullptr)
	{
		elementscene.remove(enemy);
		enemy->collision_feet->to_delete = true;
		enemy = nullptr;
		delete enemy;
	}

	return true;
}

bool j1EntityElementScene::DeleteDynObject(DynamicObjects* dynobject)
{
	elementscene.remove(dynobject);
	if (dynobject->collision != nullptr)
	{
		dynobject->collision->to_delete = true;
	}
	dynobject = nullptr;
	delete dynobject;
	return true;
}

bool j1EntityElementScene::DeleteItem(Item* item)
{
	elementscene.remove(item);
	item->collision->to_delete = true;
	item = nullptr;
	delete item;
	return true;
}

bool j1EntityElementScene::DeletePokemon(Pokemon* pokemon)
{
	if (pokemon != nullptr)
	{
		elementscene.remove(pokemon);
		pokemon->collision_feet->to_delete = true;
		pokemon = nullptr;
		delete pokemon;
	}
	return false;
}

bool j1EntityElementScene::DeleteTrainer(PokeTrainer* trainer)
{
	if (trainer != nullptr)
	{
		elementscene.remove(trainer);
		delete App->scene->poketrainer;
		App->scene->poketrainer = nullptr;
		trainer->collision_feet->to_delete = true;
		trainer = nullptr;
		delete trainer;
	}
	return true;
}

bool j1EntityElementScene::DeleteBCTrooper(BCTrooper* bctrooper)
{
	if (bctrooper != nullptr)
	{
		elementscene.remove(bctrooper);
		bctrooper->collision_feet->to_delete = true;
		bctrooper->GetColliderMaze()->to_delete = true;
		bct = nullptr;
		delete bctrooper;
		bctrooper = nullptr;
	}
	return true;
}

bool j1EntityElementScene::DeleteElement(std::string name)
{
	std::list<SceneElement*>::iterator item = elementscene.begin();
	while (item != elementscene.end())
	{
		if (item._Ptr->_Myval->name == name)
		{
			if (item._Ptr->_Myval->type == DYNOBJECT)
			{
				DeleteDynObject((DynamicObjects*)item._Ptr->_Myval);
			}
		}
		item++;
	}


	return true;
}

bool j1EntityElementScene::DeletePlayer(Player* player)
{
	if (player != nullptr)
	{
		elementscene.remove(player);
		//delete App->scene->player;
		App->scene->player = nullptr;
		player->collision_feet->to_delete = true;
		//player = nullptr;
		//delete player;
		return true;
	}
	return false;
}

bool j1EntityElementScene::DeleteVilager(Vilager* vilager)
{
	if (vilager != nullptr)
	{
		elementscene.remove(vilager);
		delete vilager;
		vilager = nullptr;
		return true;
	}
	return false;
}




void j1EntityElementScene::CreateItem(uint id, iPoint position)
{
	Item* element = new Item();
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);
	element->Awake(config.child(element->name.c_str()), id, position);
	element->Start();
	elementscene.push_front(element);
}

Hookshot* j1EntityElementScene::CreateHookshot()
{
	Hookshot* hook = new Hookshot(true);
	hook->name = "hookshot";
	hook->Start();
	elementscene.push_back(hook);
	return hook;
}

Bow * j1EntityElementScene::CreateBow()
{
	Bow* bow = new Bow(true);
	bow->name = "bow";
	bow->Start();
	elementscene.push_back(bow);
	return bow;
}

BombContainer* j1EntityElementScene::CreateBombContainer()
{
	BombContainer* element = new BombContainer();
	element->name = "bomb";
	elementscene.push_back(element);
	return element;
}

void j1EntityElementScene::CreatePokemon(pugi::xml_node& conf, uint id, iPoint pos)
{
	if (id == 1)
	{
		Golem* temp = new Golem();
		temp->Awake(conf, id);
		temp->Start();
		elementscene.push_back(temp);
	}
	else if (id == 2)
	{
		Geodude* temp = new Geodude();
		temp->Awake(conf, id, pos);
		temp->Start();
		elementscene.push_back(temp);
	}
	else if (id == 3)
	{
		Sudowoodo* temp = new Sudowoodo();
		temp->Awake(conf, id);
		temp->Start();
		elementscene.push_back(temp);
	}
}


PokeTrainer* j1EntityElementScene::CreateTrainer(pugi::xml_node& conf, uint id)
{
	PokeTrainer* temp = new PokeTrainer();
	temp->Awake(conf);
	temp->Start();
	elementscene.push_back(temp);
	return temp;
}

void j1EntityElementScene::CreateBCTrooper(pugi::xml_node& conf)
{
	BCTrooper* temp = new BCTrooper();
	temp->Awake(conf, 1);
	temp->Start();
	bct = temp;
	elementscene.push_back(temp);
}

void j1EntityElementScene::CreateVilager(pugi::xml_node& conf)
{
	Vilager* temp = new Vilager();
	temp->Awake(conf);
	temp->Start();
	elementscene.push_back(temp);
}

void j1EntityElementScene::CreateDynObject(iPoint pos, uint id, uint id_map, bool isSign, pugi::xml_node& conf)
{
	DynamicObjects* element = new DynamicObjects();
	if (isSign)
	{
		element->Awake(conf, id, pos, isSign);
		element->Start();
		elementscene.push_back(element);
	}
	else
	{
		pugi::xml_document	config_file;
		pugi::xml_node		config;
		config = LoadConfig(config_file);
		bool stop_rearch = false;
		LOG("Create DynObjects");
		config = config.child("maps").child("map");
		for (; stop_rearch == false; config = config.next_sibling())
		{
			if (config.attribute("n").as_int(0) == id_map)
			{
				element->Awake(config, id, pos);
				element->Start();
				elementscene.push_back(element);
				LOG("Created!!");
				stop_rearch = true;
			}
		}
	}

}

Player* j1EntityElementScene::CreatePlayer()
{
	Player* element = new Player();
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	config = LoadConfig(config_file);
	element->Awake(config.child(element->name.c_str()));
	elementscene.push_back(element);
	element->Start();

	return element;
}

// ---------------------------------------------
pugi::xml_node j1EntityElementScene::LoadConfig(pugi::xml_document& config_file) const
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