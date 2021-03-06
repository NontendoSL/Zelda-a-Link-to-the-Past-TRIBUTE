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
#include "Villager.h"
#include "Ganon.h"
#include "GreenMinion.h"
#include "RedMinion.h"
#include "FireBat.h"


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
	text_vase_bush = App->tex->Load("textures/AnimationsAndEffects.png");
	hookshot_chain = App->tex->Load("Particles/bctrooperl.png");

	char* buf;
	int size = App->fs->Load("config.xml", &buf);
	XML.load_buffer(buf, size);
	return ret;
}

bool j1EntityElementScene::PreUpdate()
{
	BROFILER_CATEGORY("DoPreUpdate_EntityElementsScene", Profiler::Color::Silver);
	if (App->scene->combat == false && App->scene->waitVideo == false)
	{
		std::list<SceneElement*>::iterator item = elementscene.begin();
		while (item != elementscene.end())
		{
			//Comprovate if elements is not to_delete == true
			if (item._Ptr->_Myval->to_delete)
			{
				if (bct != nullptr)
				{
					if (((BCTrooper*)item._Ptr->_Myval)->GetState() == BC_DYING)
					{
						//TODO -> if animation finished, then delete.
						App->entity_elements->DeleteBCTrooper((BCTrooper*)item._Ptr->_Myval); // Delete Dynobject
						App->audio->PlayFx(17);
						item++;
						continue;
					}
				}
				if (item._Ptr->_Myval->type == DYNOBJECT)
				{
					DeleteDynObject((DynamicObjects*)item._Ptr->_Myval);
				}
				if (item._Ptr->_Myval->type == CREATURE)
				{
					DeleteCreature((Creature*)item._Ptr->_Myval);
				}
			}
			item++;
		}
	}

	return true;
}

bool j1EntityElementScene::Update(float dt)
{
	bool ret = true;
	BROFILER_CATEGORY("DoUpdate_EntityElementsScene", Profiler::Color::MediumOrchid);
	if (App->scene->combat == false && App->scene->waitVideo == false)
	{
		std::list<SceneElement*>::iterator item = elementscene.begin();
		while (item != elementscene.end())
		{
			item._Ptr->_Myval->Update(dt);
			item++;
		}
	}

	return ret;
}

bool j1EntityElementScene::PostUpdate()
{
	BROFILER_CATEGORY("Draw_Elements", Profiler::Color::Green)
	if (App->scene->combat == false && App->scene->waitVideo == false)
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
		elementscene.remove(item._Ptr->_Myval);
		delete item._Ptr->_Myval;
		item++;
	}
	texture_dynobjects = nullptr;
	texture_trainer = nullptr;
	text_vase_bush = nullptr;
	hookshot_chain = nullptr;
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
				elementscene.remove(item._Ptr->_Myval);
				delete item._Ptr->_Myval;
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
	if (element->Start())
	{
		LOG("Soldier Created");
	}
	elementscene.push_back(element);
}

bool j1EntityElementScene::DeleteEnemy(Soldier* enemy)
{
	if (enemy != nullptr)
	{
		elementscene.remove(enemy);
		enemy->collision_feet->to_delete = true;
		enemy->collision_feet = nullptr;
		delete enemy;
		enemy = nullptr;
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
	dynobject->collision = nullptr;
	delete dynobject;
	dynobject = nullptr;
	return true;
}

bool j1EntityElementScene::DeleteItem(Item* item)
{
	elementscene.remove(item);
	item->collision->to_delete = true;
	item->collision = nullptr;
	delete item;
	item = nullptr;
	return true;
}

bool j1EntityElementScene::DeletePokemon(Pokemon* pokemon)
{
	if (pokemon != nullptr)
	{
		elementscene.remove(pokemon);
		pokemon->collision_feet->to_delete = true;
		pokemon->collision_feet = nullptr;
		delete pokemon;
		pokemon = nullptr;
	}
	return false;
}

bool j1EntityElementScene::DeleteBCTrooper(BCTrooper* bctrooper)
{
	if (bctrooper != nullptr)
	{
		elementscene.remove(bctrooper);
		bctrooper->collision_feet->to_delete = true;
		for (uint i = 0; i < bctrooper->GetMazeSize(); i++)
		{
			if (bctrooper->GetColliderMaze(i) != nullptr)
			{
				bctrooper->GetColliderMaze(i)->to_delete = true;
			}
		}

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

bool j1EntityElementScene::DeleteVilager(Villager* vilager)
{
	if (vilager != nullptr)
	{
		elementscene.remove(vilager);
		vilager->collision_feet->to_delete = true;
		vilager->collision_feet = nullptr;
		delete vilager;
		vilager = nullptr;
		return true;
	}
	return false;
}

bool j1EntityElementScene::DeleteCreature(Creature* creature)
{
	if (creature != nullptr)
	{
		elementscene.remove(creature);
		if (creature->collision_feet != nullptr)
		{
			creature->collision_feet->to_delete = true;
			creature->collision_feet = nullptr;
		}
		delete creature;
		creature = nullptr;
	}
	return true;
}

void j1EntityElementScene::SwapObject(SceneElement* obj)
{
	if (obj != nullptr)
	{
		if (obj != elementscene.begin()._Ptr->_Myval)
		{
			std::list<SceneElement*>::iterator temp = elementscene.begin();
			for (;temp != elementscene.end();temp++)
			{
				if (temp._Ptr->_Myval == obj)
				{
					std::swap(temp._Ptr->_Myval, elementscene.begin()._Ptr->_Myval);
					break;
				}
			}
			if (elementscene.begin()._Ptr->_Myval != App->scene->player)
			{
				for (temp = elementscene.begin(); temp != elementscene.end(); temp++)
				{
					if (temp._Ptr->_Myval == App->scene->player)
					{
						std::swap(temp._Ptr->_Myval, elementscene.begin()._Ptr->_Next->_Myval);
						break;
					}
				}
			}
		}
	}
}

void j1EntityElementScene::SwapGanon()
{
	if (App->scene->player != nullptr && App->entity_elements->ganon != nullptr)
	{
		if (App->entity_elements->ganon != elementscene.begin()._Ptr->_Myval)
		{
			std::list<SceneElement*>::iterator temp = elementscene.begin();
			for (; temp != elementscene.end(); temp++)
			{
				if (temp._Ptr->_Myval == App->entity_elements->ganon)
				{
					std::swap(temp._Ptr->_Myval, elementscene.begin()._Ptr->_Myval);
					break;
				}
			}
		}
	}
}

void j1EntityElementScene::SwapPlayer()
{
	if (App->scene->player != nullptr && App->entity_elements->ganon != nullptr)
	{
		if (App->scene->player != elementscene.begin()._Ptr->_Myval)
		{
			std::list<SceneElement*>::iterator temp = elementscene.begin();
			for (; temp != elementscene.end(); temp++)
			{
				if (temp._Ptr->_Myval == App->scene->player)
				{
					std::swap(temp._Ptr->_Myval, elementscene.begin()._Ptr->_Myval);
					break;
				}
			}
		}
	}
}



void j1EntityElementScene::CreateItem(uint id, iPoint position)
{
	if (id != 0)
	{
		Item* element = new Item();
		pugi::xml_document	config_file;
		pugi::xml_node		config;
		config = LoadConfig(config_file);
		element->Awake(config.child(element->name.c_str()), id, position);
		element->Start();
		elementscene.push_front(element);
	}
}

Hookshot* j1EntityElementScene::CreateHookshot()
{
	Hookshot* hook = new Hookshot(true);
	hook->name = "hookshot";
	hook->Start();
	elementscene.push_back(hook);
	return hook;
}

Bow* j1EntityElementScene::CreateBow()
{
	Bow* bow = new Bow(true);
	bow->name = "bow";
	bow->Start();
	elementscene.push_back(bow);
	return bow;
}

void j1EntityElementScene::DeleteArrows()
{
	std::list<SceneElement*>::iterator temp = elementscene.begin();
	while (temp != elementscene.end())
	{
		if (temp._Ptr->_Myval->name == "bow")
		{
			Bow* bow = (Bow*)temp._Ptr->_Myval;
			bow->DestroyArrows();
			break;
		}
		temp++;
	}
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

void j1EntityElementScene::CreateBCTrooper(pugi::xml_node& conf)
{
	BCTrooper* temp = new BCTrooper();
	temp->Awake(conf, 1);
	temp->Start();
	elementscene.push_back(temp);
	bct = (BCTrooper*)elementscene.back();
}

void j1EntityElementScene::CreateGanon(pugi::xml_node& conf)
{
	Ganon* temp = new Ganon();
	temp->Awake(conf, 1);
	temp->Start();
	elementscene.push_back(temp);
	ganon = (Ganon*)elementscene.back();
}

void j1EntityElementScene::CreateVillager(pugi::xml_node& conf)
{
	Villager* temp = new Villager();
	temp->Awake(conf);
	temp->Start();
	elementscene.push_back(temp);
}

void j1EntityElementScene::CreateGMinion(iPoint pos)
{
	GreenMinion* temp = new GreenMinion();
	temp->Start(pos);
	elementscene.push_back(temp);
}

void j1EntityElementScene::CreateRMinion(iPoint pos)
{
	RedMinion* temp = new RedMinion();
	temp->Start(pos);
	elementscene.push_back(temp);
}

void j1EntityElementScene::CreateFireBat()
{
	FireBat* temp = new FireBat();
	temp->Start();
	elementscene.push_back(temp);
}

void j1EntityElementScene::CreateDynObject(iPoint pos, uint id, uint id_map, bool isSign, pugi::xml_node& special_config)
{
	DynamicObjects* element = new DynamicObjects();
	if (isSign)
	{
		element->Awake(special_config, id, pos, isSign);
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