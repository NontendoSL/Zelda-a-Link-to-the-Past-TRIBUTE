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

j1Gui::j1Gui() : j1Module()
{
	name = "gui";

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
	for (uint i = 0; i < entities.size(); i++) {
		entities[i]->Handle_Input();
		entities[i]->Update();
	}
	return true;
}



// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (uint i = 0; i < entities.size(); i++) {
		if (entities[i]->visible == true) {
			entities[i]->Draw();
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

void j1Gui::Erase(int id)
{
	entities.erase(entities.begin() + id + 1);
}

int j1Gui::GetEntitiesSize()
{
	return entities.size();
}

Image* j1Gui::CreateImage(SDL_Rect rect, iPoint pos, std::string identifier, uint id) {

	Image* element = new Image(rect, pos, identifier, id);

	entities.push_back(element);

	return element;
}

Text* j1Gui::CreateText(FontName search, const char* string, uint length, iPoint pos, uint size, SDL_Color color, bool addelement, std::string identifier, uint id) {

	Text* element = new Text(search, string, color, length, pos, size, addelement, identifier, id);
	if (addelement)
		entities.push_back(element);

	return element;
}

Button* j1Gui::CreateButton(SDL_Rect rect, iPoint pos, iPoint text2, iPoint text3, bool animated, std::string identifier, uint id, const char* textstring, uint textsize, iPoint textpos) {

	Button* element = new Button(rect, pos, text2, text3, animated, identifier, id, textstring, textsize, textpos);

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

ZeldaMenu* j1Gui::CreateZeldaMenu() 
{
	ZeldaMenu* element = new ZeldaMenu();
	entities.push_back(element);
	return element;
}

PokemonCombatHud* j1Gui::CreatePokemonCombatHud(uint cd_time, uint hpbar1, uint hpbar2)
{
	PokemonCombatHud* element = new PokemonCombatHud(cd_time, hpbar1, hpbar2);
	entities.push_back(element);
	return element;
}



// class Gui ---------------------------------------------------

