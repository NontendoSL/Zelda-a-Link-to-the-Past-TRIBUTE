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
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.c_str());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	ReceiveInput();
	for (uint i = 0; i < entities.size(); i++)
	{
		if (status == ZELDA_HUD || status == ZELDA_MENU)
		{
			if (entities[i]->belong == ZELDA_HUD || entities[i]->belong == ZELDA_MENU)
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
	case GuiGroup::POKEMON_COMBAT:
		App->scene->pokecombat->Input();
		break;
	}
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (uint i = 0; i < entities.size(); i++)
	{
		if (status == ZELDA_HUD || status == ZELDA_MENU)
		{
			if (entities[i]->belong == ZELDA_HUD || entities[i]->belong == ZELDA_MENU)
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

void j1Gui::Erase(j1GuiEntity* to_delete)
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i] == to_delete)
		{
			entities.erase(entities.begin() + i);
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
	if (focused != nullptr)
	{
		Button* temp = (Button*)focused;
		temp->click = false;
	}

	focused = to_focus;
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
		SetFocus(App->scene->start_menu->GetFirst());
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

Text* j1Gui::CreateText(FontName search, const char* string, uint length, iPoint pos, uint size, SDL_Color color, bool addelement, std::string identifier, GuiGroup group) {

	Text* element = new Text(search, string, color, length, pos, size, addelement, identifier, group);
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

PokemonCombatHud* j1Gui::CreatePokemonCombatHud(Pokemon* Link, Pokemon* Brendan)
{
	PokemonCombatHud* element = new PokemonCombatHud(Link, Brendan);
	entities.push_back(element);
	return element;
}



// class Gui ---------------------------------------------------