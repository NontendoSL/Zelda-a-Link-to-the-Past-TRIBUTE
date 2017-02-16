#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "j1GuiElements.h"
#include "j1Scene.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");

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
	atlas = App->tex->Load(atlas_file_name.GetString());

	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	for (uint i = 0; i < entities.Count(); i++) {
		entities[i]->Update();
		entities[i]->Handle_Input();
	}
	return true;
}



// Called after all Updates
bool j1Gui::PostUpdate()
{
	for (uint i = 0; i < entities.Count(); i++) {
		entities[i]->Draw();
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");
	for (uint i = entities.Count()-1; i > 0; i--) {
		delete entities[i];
	}
	entities.Clear();
	return true;
}

// const getter for atlas
const SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}


Image* j1Gui::CreateImage(SDL_Rect rect, iPoint pos) {

	Image* element = new Image(rect, pos);

	entities.PushBack(element);

	return element;
}

Text* j1Gui::CreateText(const char* string, iPoint pos, uint size ) {

	Text* element = new Text(string,pos, size);

	entities.PushBack(element);

	return element;
}

Button* j1Gui::CreateButton(SDL_Rect rect, iPoint pos, iPoint text2, iPoint text3, const char* textstring, uint textsize, iPoint textpos) {

	Button* element = new Button(rect, pos, text2, text3, textstring, textsize, textpos);

	entities.PushBack(element);

	return element;
}


// class Gui ---------------------------------------------------

