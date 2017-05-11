#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "j1GuiElements.h"
#include "j1Scene.h"
#include "j1SceneIntro.h"
#include "j1FileSystem.h"

j1Gui::j1Gui() : j1Module()
{
	name = "gui";
	status = NONE;
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas & entities");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");
	entities_file_name = conf.child("entities").attribute("file").as_string("");
	gui_groups_name.push_back("Main_Menu");
	gui_groups_name.push_back("Zelda_HUD");
	gui_groups_name.push_back("Zelda_Menu");
	gui_groups_name.push_back("Zelda_Menu_Options");
	gui_groups_name.push_back("PokemonCombat_HUD");
	gui_groups_name.push_back("PokemonWorld_HUD");
	gui_groups_name.push_back("PokemonWorld_Menu");
	gui_groups_name.push_back("PokemonWorld_Bag"); 
	gui_groups_name.push_back("PokemonWorld_Shop");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.c_str());
	LoadEntities();

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	ReceiveInput();
	for (uint i = 0; i < entities.size(); i++)
	{
		if (status == ZELDA_HUD || status == ZELDA_MENU || status == ZELDA_MENU_OPTION)
		{
			if (entities[i]->belong == ZELDA_HUD || entities[i]->belong == ZELDA_MENU || entities[i]->belong == ZELDA_MENU_OPTION)
			{
				if (entities[i]->visible)
					entities[i]->Update(focused);
			}
		}
		else if (status == POKEMON_HUD || status == POKEMON_MENU)
		{
			if (entities[i]->belong == POKEMON_HUD || entities[i]->belong == POKEMON_MENU)
			{
				if (entities[i]->visible)
					entities[i]->Update(focused);
			}
		}
		else
		{
			if (entities[i]->belong == status)
			{
				if (entities[i]->visible)
					entities[i]->Update(focused);
			}
		}
	}
	return true;
}

void j1Gui::ReceiveInput()
{
	switch (status)
	{
	case GuiGroup::MAIN_MENU:
		App->intro->main_menu->Input();
		break;
	case GuiGroup::ZELDA_HUD:
		App->scene->hud->Input();
		break;
	case GuiGroup::ZELDA_MENU:
		App->scene->start_menu->Input();
		break;
	case GuiGroup::ZELDA_MENU_OPTION:
		App->scene->start_menu->Input();
		break;
	case GuiGroup::POKEMON_COMBAT:
		App->scene->pokecombat->Input();
		break;
	case GuiGroup::POKEMON_HUD:
		App->scene->poke_hud->Input();
		break;
	case GuiGroup::POKEMON_MENU:
		App->scene->poke_menu->Input();
		return;
	case GuiGroup::POKEMON_BAG:
		App->scene->poke_bag->Input();
		return;
	case GuiGroup::POKEMON_SHOP:
		App->scene->poke_shop->Input();
		return;
	}
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (uint i = 0; i < entities.size(); i++)
	{
		if (status == ZELDA_HUD || status == ZELDA_MENU || status == ZELDA_MENU_OPTION)
		{
			if (entities[i]->belong == ZELDA_HUD || entities[i]->belong == ZELDA_MENU || entities[i]->belong == ZELDA_MENU_OPTION)
			{
				if (entities[i]->visible)
					entities[i]->Draw();
			}
		}
		else if (status == POKEMON_HUD || status == POKEMON_MENU)
		{
			if (entities[i]->belong == POKEMON_HUD || entities[i]->belong == POKEMON_MENU)
			{
				if (entities[i]->visible)
					entities[i]->Draw();
			}
		}
		else
		{
			if (entities[i]->belong == status)
			{
				if (entities[i]->visible)
					entities[i]->Draw();
			}
		}
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	if (entities.size() > 0)
	{
		for (uint i = entities.size() - 1; i > 0; i--) {
			if (entities[i] != nullptr) {
				delete entities[i];
			}
		}
		entities.clear();
	}
	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

void j1Gui::OnGui(j1GuiEntity * element, GuiAction event)
{
	if (element->identifier == "pk_bag:DEF PROTEIN")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			App->scene->poke_bag->selecting_poke = true;
			App->gui->SetFocus(App->scene->poke_bag->GetFirst(false));
			App->scene->poke_bag->selected_name = element->identifier;
		}
	}
	if (element->identifier == "pk_bag:HP UP")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			App->scene->poke_bag->selecting_poke = true;
			App->gui->SetFocus(App->scene->poke_bag->GetFirst(false));
			App->scene->poke_bag->selected_name = element->identifier;
		}
	}
	if (element->identifier == "pk_bag:X ATTACK")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
			App->scene->poke_bag->selecting_poke = true;
			App->gui->SetFocus(App->scene->poke_bag->GetFirst(false));
			App->scene->poke_bag->selected_name = element->identifier;
		}
	}
	if (element->identifier == "pk_bag:CITRUS BERRY")
	{
		if (event == CLICK_DOWN)
		{
			((Button*)element)->click = true;
		}
		else
		{
			((Button*)element)->click = false;
		}
	}
}

void j1Gui::Erase(j1GuiEntity* to_delete)
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i] == to_delete)
		{
 			entities.erase(entities.begin() + i);
			delete to_delete;
			return;
		}
	}

}

int j1Gui::GetEntitiesSize()
{
	return entities.size();
}

j1GuiEntity* j1Gui::GetFocused()
{
	return focused;
}

void j1Gui::SetFocus(j1GuiEntity* to_focus)
{
	if (focused != to_focus)
	{
		if (focused != nullptr)
		{
			((Button*)focused)->click = false;
		}

		focused = to_focus;
	}
}

void j1Gui::MoveGroup(GuiGroup group, bool x_axis, float speed, bool move_all)
{
	for (int i = 0; i < App->gui->entities.size(); i++)
	{
		if (entities[i]->belong == group)
		{
			if (move_all)
			{
				if (x_axis)
					entities[i]->position.x += speed;
				else
					entities[i]->position.y += speed;
			}
			else if (entities[i]->movable)
			{
				if (x_axis)
					entities[i]->position.x += speed;
				else
					entities[i]->position.y += speed;
			}
		}
	}
}

void j1Gui::SetGui(GuiGroup guistate)
{
	status = guistate;
	switch (status)
	{
	case MAIN_MENU:
		SetFocus(App->intro->main_menu->GetElement(0));
		return;
	case ZELDA_HUD:
		SetFocus(nullptr);
		return;
	case ZELDA_MENU:
		SetFocus(App->scene->start_menu->GetFirst(false));
		return;
	case ZELDA_MENU_OPTION:
		SetFocus(App->scene->start_menu->GetFirst(true));
		return;
	case POKEMON_HUD:
		SetFocus(App->scene->poke_hud->GetFirst());
		return;
	case POKEMON_MENU:
		SetFocus(App->scene->poke_menu->GetFirst());
		return;
	case POKEMON_BAG:
		SetFocus(App->scene->poke_bag->GetFirst());
		return;
	case POKEMON_SHOP:
		SetFocus(App->scene->poke_shop->GetFirst());
		return;
	default:
		SetFocus(nullptr);
		return;
	}

}

Image* j1Gui::CreateImage(SDL_Rect rect, iPoint pos, std::string identifier, GuiGroup group, bool movable) {

	Image* element = new Image(rect, pos, identifier, group);
	element->movable = movable;
	entities.push_back(element);

	return element;
}

Text* j1Gui::CreateText(FontName search, const char* string, uint length, iPoint pos, uint size, SDL_Rect color, bool addelement, std::string identifier, GuiGroup group) {

	SDL_Color colored = { color.x,color.y,color.w,color.h };
	Text* element = new Text(search, string, colored, length, pos, size, addelement, identifier, group);
	if (addelement)
		entities.push_back(element);

	return element;
}

Button* j1Gui::CreateButton(j1Module* listener, SDL_Rect rect, iPoint pos, iPoint text2, iPoint text3, bool animated, std::string identifier, GuiGroup group, const char* textstring, uint textsize, iPoint textpos) {

	Button* element = new Button(rect, pos, text2, text3, animated, identifier, group, textstring, textsize, textpos);
	element->listener = listener;
	entities.push_back(element);

	return element;
}

Dialogue* j1Gui::CreateDialogue(const char* string)
{

	Dialogue* element = new Dialogue(string);
	element->belong = status;

	entities.push_back(element);

	return element;
}

Selector* j1Gui::CreateSelector(const char* first, const char*second, j1GuiEntity*parent)
{

	Selector* element = new Selector(first, second, parent);

	entities.push_back(element);

	return element;
}

MainMenu* j1Gui::CreateMainMenu()
{
	MainMenu* element = new MainMenu();
	entities.push_back(element);
	return element;
}

ZeldaHud* j1Gui::CreateZeldaHud()
{
	ZeldaHud* element = new ZeldaHud();
	entities.push_back(element);
	return element;
}

ZeldaMenu* j1Gui::CreateZeldaMenu()
{
	ZeldaMenu* element = new ZeldaMenu();
	entities.push_back(element);
	return element;
}

PokemonCombatHud* j1Gui::CreatePokemonCombatHud(PokemonCombat* Link, PokemonCombat* Brendan)
{
	PokemonCombatHud* element = new PokemonCombatHud(Link, Brendan);
	entities.push_back(element);
	return element;
}

PokemonWorldHud* j1Gui::CreatePokemonWorldHud()
{
	PokemonWorldHud* element = new PokemonWorldHud();
	entities.push_back(element);
	return element;
}

PokemonWorldMenu* j1Gui::CreatePokemonWorldMenu()
{
	PokemonWorldMenu* element = new PokemonWorldMenu();
	entities.push_back(element);
	return element;
}

PokemonWorldBag * j1Gui::CreatePokemonWorldBag()
{
	PokemonWorldBag* element = new PokemonWorldBag();
	entities.push_back(element);
	return element;
}

PokemonWorldShop * j1Gui::CreatePokemonWorldShop()
{
	PokemonWorldShop* element = new PokemonWorldShop();
	entities.push_back(element);
	return element;
}

// --------------------------------------------- Entities Loading
void j1Gui::LoadEntities()
{
	pugi::xml_document	entities_file;
	pugi::xml_node		entities_node;
	pugi::xml_node		group_node;

	entities_node = LoadConfig(entities_file, entities_file_name);
	GuiGroup actual = GuiGroup::MAIN_MENU;
	group_node = entities_node.child(gui_groups_name[actual - 1].c_str());

	Button* anim_picker = nullptr;
	Image* inside_picker = nullptr;
	Image* inside_picker_push = nullptr;
	std::string type;

	for (pugi::xml_node temp = group_node; temp != NULL; temp = group_node)
	{
		for (pugi::xml_node temp_inside = temp.child("entity"); temp_inside != NULL; temp_inside = temp_inside.next_sibling())
		{
			type = temp_inside.attribute("type").as_string("");
			if (type == "image")
			{
				inside_picker = App->gui->CreateImage({ temp_inside.attribute("rect.x").as_int(0), temp_inside.attribute("rect.y").as_int(0), temp_inside.attribute("rect.w").as_int(0), temp_inside.attribute("rect.h").as_int(0) }, { temp_inside.attribute("pos.x").as_int(0),temp_inside.attribute("pos.y").as_int(0) }, temp_inside.attribute("identifier").as_string(""), actual, temp_inside.attribute("movable").as_bool(true));
				if (temp_inside.attribute("inside").as_bool(false))
				{
					if (std::string(temp_inside.attribute("identifier").as_string("")) == "pokemon menu bg")
					{
						int a = 0;
					}
					for (pugi::xml_node push = temp_inside.child("entity"); push != NULL; push = push.next_sibling())
					{
						if (std::string(push.attribute("type").as_string("")) == "image")
						{
							inside_picker_push = App->gui->CreateImage({ push.attribute("rect.x").as_int(0), push.attribute("rect.y").as_int(0), push.attribute("rect.w").as_int(0), push.attribute("rect.h").as_int(0) }, { push.attribute("pos.x").as_int(0), push.attribute("pos.y").as_int(0) }, push.attribute("identifier").as_string(""), actual, push.attribute("movable").as_bool(true));
							inside_picker->elements.push_back(inside_picker_push);
							if (push.attribute("inside").as_bool(false))
							{
								for (pugi::xml_node push2 = push.child("entity"); push2 != NULL; push2 = push2.next_sibling())
								{
									inside_picker_push->elements.push_back(App->gui->CreateImage({ push2.attribute("rect.x").as_int(0), push2.attribute("rect.y").as_int(0), push2.attribute("rect.w").as_int(0), push2.attribute("rect.h").as_int(0) }, { push2.attribute("pos.x").as_int(0), push2.attribute("pos.y").as_int(0) }, push2.attribute("identifier").as_string(""), actual, push2.attribute("movable").as_bool(true)));
								}
							}
						}
						if (std::string(push.attribute("type").as_string("")) == "button")
						{
							Button* temp = App->gui->CreateButton(App->Search(push.attribute("listener").as_string("")), { push.attribute("rect.x").as_int(0), push.attribute("rect.y").as_int(0), push.attribute("rect.w").as_int(0), push.attribute("rect.h").as_int(0) }, { push.attribute("pos.x").as_int(0), push.attribute("pos.y").as_int(0) }, { push.attribute("state2.x").as_int(0) ,push.attribute("state2.y").as_int(0) }, { push.attribute("state3.x").as_int(0) ,push.attribute("state3.y").as_int(0) }, push.attribute("animated").as_bool(false), push.attribute("identifier").as_string(""), actual);
							inside_picker->elements.push_back(temp);
							if (push.attribute("inside").as_bool(false))
							{
								for (pugi::xml_node push3 = push.child("entity"); push3 != NULL; push3 = push3.next_sibling())
								{
									//image inside button
									if (std::string(push3.attribute("type").as_string("")) == "image")
									{
										Image* prov = App->gui->CreateImage({ push3.attribute("rect.x").as_int(0), push3.attribute("rect.y").as_int(0), push3.attribute("rect.w").as_int(0), push3.attribute("rect.h").as_int(0) }, { push3.attribute("pos.x").as_int(0), push3.attribute("pos.y").as_int(0) }, push3.attribute("identifier").as_string(""), actual, push3.attribute("movable").as_bool(true));
										temp->elements.push_back(prov);
									}
								}
							}
						}
						if (std::string(push.attribute("type").as_string("")) == "text")
						{
							Text* ins = App->gui->CreateText((FontName)push.attribute("font").as_int(0), push.attribute("string").as_string(""), push.attribute("length").as_int(0), { push.attribute("pos.x").as_int(0), push.attribute("pos.y").as_int(0) }, push.attribute("size").as_int(0), { push.attribute("rgb.x").as_int(255), push.attribute("rgb.y").as_int(255), push.attribute("rgb.w").as_int(255), push.attribute("rgb.h").as_int(255) }, true, push.attribute("identifier").as_string(""), actual);
							inside_picker->elements.push_back(ins);
						}
					}
				}
			}
			if (type == "button")
			{
				anim_picker = App->gui->CreateButton(App->Search(temp_inside.attribute("listener").as_string("")), { temp_inside.attribute("rect.x").as_int(0), temp_inside.attribute("rect.y").as_int(0), temp_inside.attribute("rect.w").as_int(0), temp_inside.attribute("rect.h").as_int(0) }, { temp_inside.attribute("pos.x").as_int(0), temp_inside.attribute("pos.y").as_int(0) }, { temp_inside.attribute("state2.x").as_int(0) ,temp_inside.attribute("state2.y").as_int(0) }, { temp_inside.attribute("state3.x").as_int(0) ,temp_inside.attribute("state3.y").as_int(0) }, temp_inside.attribute("animated").as_bool(false), temp_inside.attribute("identifier").as_string(""), actual);
				if (temp_inside.attribute("animated").as_bool(false))
				{
					for (pugi::xml_node anim = temp_inside.child("animation"); anim != NULL; anim = anim.next_sibling())
					{
						anim_picker->anim->PushBack({ anim.attribute("x").as_int(0), anim.attribute("y").as_int(0), anim.attribute("w").as_int(0), anim.attribute("h").as_int(0) });
					}
					anim_picker->anim->speed = temp_inside.attribute("anim_speed").as_double();
				}
				if (temp_inside.attribute("inside").as_bool(false))
				{
					for (pugi::xml_node anim = temp_inside.child("entity"); anim != NULL; anim = anim.next_sibling())
					{
						inside_picker = App->gui->CreateImage({ anim.attribute("rect.x").as_int(0), anim.attribute("rect.y").as_int(0), anim.attribute("rect.w").as_int(0), anim.attribute("rect.h").as_int(0) }, { anim.attribute("pos.x").as_int(0), anim.attribute("pos.y").as_int(0) }, anim.attribute("identifier").as_string(""), actual, anim.attribute("movable").as_bool(true));
						anim_picker->elements.push_back(inside_picker);
					}
				}
			}
			if (type == "text")
			{
				App->gui->CreateText((FontName)temp_inside.attribute("font").as_int(0), temp_inside.attribute("string").as_string(""), temp_inside.attribute("length").as_int(0), { temp_inside.attribute("pos.x").as_int(0), temp_inside.attribute("pos.y").as_int(0) }, temp_inside.attribute("size").as_int(0), { temp_inside.attribute("rgb.x").as_int(255), temp_inside.attribute("rgb.y").as_int(255), temp_inside.attribute("rgb.w").as_int(255), temp_inside.attribute("rgb.h").as_int(255) }, true, temp_inside.attribute("identifier").as_string(""), actual);
			}
		}
		actual = (GuiGroup)(actual + 1);
		if (actual > gui_groups_name.size())
		{
			LOG("GUI ENTITIES LOADED=======================================");
			return;
		}
		else
		{
			group_node = entities_node.child(gui_groups_name[actual - 1].c_str());
		}
	}


}

pugi::xml_node j1Gui::LoadConfig(pugi::xml_document& config_file, std::string file) const
{
	pugi::xml_node ret;

	char* buf;
	int size = App->fs->Load(file.c_str(), &buf);
	pugi::xml_parse_result result = config_file.load_buffer(buf, size);
	RELEASE(buf);

	if (result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("elements");

	return ret;
}

j1GuiEntity* j1Gui::GetEntity(const char* identifier)
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i]->identifier == identifier)
		{
			return entities[i];
		}
	}

}


// class Gui ---------------------------------------------------