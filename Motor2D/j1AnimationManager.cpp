#include "j1AnimationManager.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1FileSystem.h"
#include "j1Textures.h"
#include "j1EntityElementsScene.h"
#include "j1Creature.h"

j1AnimationManager::j1AnimationManager()
{
	name = "animation";
}

j1AnimationManager::~j1AnimationManager()
{
}

bool j1AnimationManager::Awake(pugi::xml_node& test)
{
	for (pugi::xml_node temp = test.child("file"); temp.next_sibling() != NULL; temp = temp.next_sibling())
	{
		file_names.push_back(temp.attribute("file").as_string(""));
	}

	return true;
}

bool j1AnimationManager::Start()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		state;
	std::list<std::string>::iterator file = file_names.begin();
	uint cont = 0;
	for (; file != file_names.end(); file++)
	{
		//Load config
		config = LoadConfig(config_file, file._Ptr->_Myval);

		//Create AnimationStruct temp --------------------------------
		AnimationStruct temp_animat;
		temp_animat.name = config.attribute("name").as_string("");
		temp_animat.graphics = App->tex->Load(config.attribute("imagePath").as_string(""));

		//Add all AnimDirect in animat.
		state = config.child("state");
		for (int i = 0; i < config.attribute("numberStates").as_int(0); i++)
		{
			bool stop_rearch = false;
			std::string dir;
			AnimDirect stanim;
			for (pugi::xml_node temp = state.child("sprite"); stop_rearch == false; temp = temp.next_sibling())
			{
				dir = temp.attribute("dir").as_string("");
				if (dir == "East")
				{
					stanim.East_action.PushBack({ temp.attribute("x").as_int(0), temp.attribute("y").as_int(0), temp.attribute("w").as_int(0), temp.attribute("h").as_int(0) });
				}
				else if (dir == "North")
				{
					stanim.North_action.PushBack({ temp.attribute("x").as_int(0), temp.attribute("y").as_int(0), temp.attribute("w").as_int(0), temp.attribute("h").as_int(0) });
				}
				else if (dir == "West")
				{
					stanim.West_action.PushBack({ temp.attribute("x").as_int(0), temp.attribute("y").as_int(0), temp.attribute("w").as_int(0), temp.attribute("h").as_int(0) });
				}
				else if (dir == "South")
				{
					stanim.South_action.PushBack({ temp.attribute("x").as_int(0), temp.attribute("y").as_int(0), temp.attribute("w").as_int(0), temp.attribute("h").as_int(0) });
				}
				else
				{
					stop_rearch = true;
					stanim.East_action.speed = state.attribute("speed").as_float(0);
					stanim.North_action.speed = state.attribute("speed").as_float(0);
					stanim.South_action.speed = state.attribute("speed").as_float(0);
					stanim.West_action.speed = state.attribute("speed").as_float(0);
				}
			}
			temp_animat.anim.push_back(stanim);
			state = state.next_sibling();
		}
		//Add new AnimationStruct in animat -------------------------
		animat.push_back(temp_animat);
		cont++;
	}
	range_link = 8;
	return true;
}

bool j1AnimationManager::CleanUp()
{
	return true;
}

void j1AnimationManager::Drawing_Manager(ActionState status, Direction dir, iPoint position, std::string name) 
{
	for (int i = 0; i < animat.size(); i++)
	{
		if (animat[i].name == name)
		{
			if (dir == UP)
			{
				if (status == IDLE)
				{
					App->render->Blit(animat[i].graphics, position.x, position.y - range_link, &animat[i].anim[IDLE].North_action.frames[0]);
				}
				else if (status == ATTACK)
				{

				}
				else if (status == WALKING)
				{
					SDL_Rect r = animat[i].anim[WALKING].North_action.GetCurrentFrame();
					App->render->Blit(animat[i].graphics, position.x, position.y - range_link, &r);
				}
				else
				{

				}
			}
			else if (dir == DOWN)
			{
				if (status == IDLE)
				{
					App->render->Blit(animat[i].graphics, position.x, position.y - range_link, &animat[i].anim[IDLE].South_action.frames[0]);
				}
				else if (status == ATTACK)
				{

				}
				else if (status == WALKING)
				{
					SDL_Rect r = animat[i].anim[WALKING].South_action.GetCurrentFrame();
					App->render->Blit(animat[i].graphics, position.x, position.y - range_link, &r);
				}
				else
				{

				}
			}
			else if (dir == LEFT)
			{
				if (status == IDLE)
				{
					App->render->Blit(animat[i].graphics, position.x, position.y - range_link, &animat[i].anim[IDLE].West_action.frames[0]);
				}
				else if (status == ATTACK)
				{

				}
				else if (status == WALKING)
				{
					SDL_Rect r = animat[i].anim[WALKING].West_action.GetCurrentFrame();
					App->render->Blit(animat[i].graphics, position.x, position.y - range_link, &r);
				}
				else
				{

				}
			}
			else if (dir == RIGHT)
			{
				if (status == IDLE)
				{
					App->render->Blit(animat[i].graphics, position.x, position.y - range_link, &animat[i].anim[IDLE].East_action.frames[0]);
				}
				else if (status == ATTACK)
				{

				}
				else if (status == WALKING)
				{
					SDL_Rect r = animat[i].anim[WALKING].East_action.GetCurrentFrame();
					App->render->Blit(animat[i].graphics, position.x, position.y - range_link, &r);
				}
				else
				{

				}
			}
		}
	}


}


// ---------------------------------------------
pugi::xml_node j1AnimationManager::LoadConfig(pugi::xml_document& config_file, std::string file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = App->fs->Load(file.c_str(), &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("Animation");

	return ret;
}