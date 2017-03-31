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
	for (pugi::xml_node temp = test.child("file"); temp != NULL; temp = temp.next_sibling())
	{
		file_names.push_back(temp.attribute("file").as_string(""));
	}
	pivot = { 0, 0 };
	r = { 0, 0, 0, 0 };
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
				SDL_Rect tex_rect = { temp.attribute("x").as_int(0), temp.attribute("y").as_int(0), temp.attribute("w").as_int(0), temp.attribute("h").as_int(0) };
				
				//PIVOT POINT --------------
				float pivotX = temp.attribute("pX").as_float(0) * tex_rect.w;
				float pivotY = temp.attribute("pY").as_float(0) * tex_rect.h;
				pivotX = (pivotX > (floor(pivotX) + 0.5f)) ? ceil(pivotX) : floor(pivotX);
				pivotY = (pivotY > (floor(pivotY) + 0.5f)) ? ceil(pivotY) : floor(pivotY);
				iPoint offset(pivotX, pivotY);

				if (dir == "East")
				{
					stanim.East_action.PushBack(tex_rect, offset);
				}
				else if (dir == "North")
				{
					stanim.North_action.PushBack(tex_rect, offset);
				}
				else if (dir == "West")
				{
					stanim.West_action.PushBack(tex_rect, offset);
				}
				else if (dir == "South")
				{
					stanim.South_action.PushBack(tex_rect, offset);
				}
				else
				{
					stop_rearch = true;
					stanim.East_action.speed = state.attribute("speed").as_float(0);
					stanim.East_action.loop = state.attribute("loop").as_bool(false);
					stanim.North_action.speed = state.attribute("speed").as_float(0);
					stanim.North_action.loop = state.attribute("loop").as_bool(false);
					stanim.South_action.speed = state.attribute("speed").as_float(0);
					stanim.South_action.loop = state.attribute("loop").as_bool(false);
					stanim.West_action.speed = state.attribute("speed").as_float(0);
					stanim.West_action.loop = state.attribute("loop").as_bool(false);
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

void j1AnimationManager::Drawing_Manager(ActionState status, Direction dir, iPoint position, uint id) 
{
	BROFILER_CATEGORY("Animation", Profiler::Color::DarkMagenta)
	//INFO ID:  1-> Player // 2-> Soldier // 3-> Blaziken // 4-> unknown // 5-> unknown
	if (status <= DYING)
	{
		test_state = status;
		if (test_state == HOOKTHROWN)
		{
			test_state = IDLE; //TEST BECAUSE WE DON'T HAVE HOOKING ANIMATION YET.
		}
		if (dir == UP)
		{
			r = animat[id].anim[test_state].North_action.GetCurrentFrame();
			pivot = animat[id].anim[test_state].North_action.GetCurrentOffset();
		}

		else if (dir == DOWN)
		{
			r = animat[id].anim[test_state].South_action.GetCurrentFrame();
			pivot = animat[id].anim[test_state].South_action.GetCurrentOffset();
		}

		else if (dir == LEFT)
		{
			r = animat[id].anim[test_state].West_action.GetCurrentFrame();
			pivot = animat[id].anim[test_state].West_action.GetCurrentOffset();
		}

		else if (dir == RIGHT)
		{
			r = animat[id].anim[test_state].East_action.GetCurrentFrame();
			pivot = animat[id].anim[test_state].East_action.GetCurrentOffset();
		}
	}

	//DRAW
	App->render->Blit(animat[id].graphics, position.x - pivot.x + 8, position.y - pivot.y + 11, &r);
}

Animation* j1AnimationManager::GetAnimation(ActionState status, Direction dir, uint id)
{
	if (status <= DYING)
	{
		if (dir == UP)
		{
			current_animation = &animat[id].anim[status].North_action;
		}

		else if (dir == DOWN)
		{
			current_animation = &animat[id].anim[status].South_action;
		}

		else if (dir == LEFT)
		{
			current_animation = &animat[id].anim[status].West_action;
		}

		else if (dir == RIGHT)
		{
			current_animation = &animat[id].anim[status].East_action;
		}
	}

	return current_animation;
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