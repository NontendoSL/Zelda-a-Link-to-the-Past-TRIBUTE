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

j1AnimationManager::j1AnimationManager()
{
	name = "animation";
}

j1AnimationManager::~j1AnimationManager()
{
}

bool j1AnimationManager::Awake(pugi::xml_node& test)
{
	file_texture = test.child("sprite").attribute("file").as_string();

	for (pugi::xml_node temp = test.child("file"); temp.next_sibling() != NULL; temp = temp.next_sibling())
	{
		file_names.push_back(temp.attribute("file").as_string(""));
	}

	return true;
}

bool j1AnimationManager::Start()
{

	graphics = App->tex->Load(file_texture.c_str());
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	std::list<std::string>::iterator file = file_names.begin();

	for (; file != file_names.end(); file++)
	{
		/*Animation link_walk_east;
		Animation link_walk_north;
		Animation link_walk_west;
		Animation link_walk_south;*/
		config = LoadConfig(config_file, file._Ptr->_Myval);

		//config.child("")
		bool stop_rearch = false;
		std::string dir;
		for (pugi::xml_node temp = config.child("sprite"); stop_rearch == false; temp = temp.next_sibling())
		{
			dir = temp.attribute("dir").as_string("");
			if (dir == "East")
			{
				link_walk_east.PushBack({ temp.attribute("x").as_int(0), temp.attribute("y").as_int(0), temp.attribute("w").as_int(0), temp.attribute("h").as_int(0) });
			}
			else if (dir == "North")
			{
				link_walk_north.PushBack({ temp.attribute("x").as_int(0), temp.attribute("y").as_int(0), temp.attribute("w").as_int(0), temp.attribute("h").as_int(0) });
			}
			else if (dir == "West")
			{
				link_walk_west.PushBack({ temp.attribute("x").as_int(0), temp.attribute("y").as_int(0), temp.attribute("w").as_int(0), temp.attribute("h").as_int(0) });
			}
			else if (dir == "South")
			{
				link_walk_south.PushBack({ temp.attribute("x").as_int(0), temp.attribute("y").as_int(0), temp.attribute("w").as_int(0), temp.attribute("h").as_int(0) });
			}
			else
			{
				stop_rearch = true;
				//graphics = App->tex->Load(config.attribute("imagePath").as_string(""));
				link_walk_east.speed = 0.3f;
				link_walk_north.speed = 0.3f;
				link_walk_west.speed = 0.3f;
				link_walk_south.speed = 0.3f;
			}

		}
	}
	range_link = 8;

	/*link.speed = 0.3f;*/


	return true;
}

bool j1AnimationManager::CleanUp()
{
	return true;
}

void j1AnimationManager::PlayerSelector(AnimationState status, Direction dir, iPoint position) 
{
	if (dir == UP)
	{
		if (status == IDLE)
		{
			App->render->Blit(App->scene->player->player_texture, position.x, position.y - range_link, &link_walk_north.frames[3]);
		}
		else if (status == ATTACK)
		{

		}
		else if (status == WALKING)
		{
			current_animation = &link_walk_north;
			SDL_Rect r = current_animation->GetCurrentFrame();
			App->render->Blit(App->scene->player->player_texture, position.x, position.y - range_link, &r);
		}
		else
		{

		}
	}
	else if (dir == DOWN)
	{
		if (status == IDLE)
		{
			App->render->Blit(graphics, position.x, position.y - range_link, &link_walk_south.frames[3]);
		}
		else if (status == ATTACK)
		{

		}
		else if (status == WALKING)
		{
			current_animation = &link_walk_south;
			SDL_Rect r = current_animation->GetCurrentFrame();
			App->render->Blit(graphics, position.x, position.y - range_link, &r);
		}
		else
		{

		}
	}
	else if (dir == LEFT)
	{
		if (status == IDLE)
		{
			App->render->Blit(graphics, position.x, position.y - range_link, &link_walk_west.frames[3]);
		}
		else if (status == ATTACK)
		{

		}
		else if (status == WALKING)
		{
			current_animation = &link_walk_west;
			SDL_Rect r = current_animation->GetCurrentFrame();
			App->render->Blit(graphics, position.x, position.y - range_link, &r);
		}
		else
		{

		}
	}
	else if (dir == RIGHT)
	{
		if (status == IDLE)
		{
			App->render->Blit(graphics, position.x, position.y - range_link, &link_walk_east.frames[3]);
		}
		else if (status == ATTACK)
		{

		}
		else if (status == WALKING)
		{
			current_animation = &link_walk_east;
			SDL_Rect r = current_animation->GetCurrentFrame();
			App->render->Blit(graphics, position.x, position.y - range_link, &r);
		}
		else
		{

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
		ret = config_file.child("TextureAtlas");

	return ret;
}