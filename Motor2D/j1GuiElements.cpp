#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "Animation.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "j1GuiElements.h"
#include <assert.h>

/////////////////////////////// IMAGE METHODS ///////////////////////////////

Image::Image(SDL_Rect rectangle, iPoint position, std::string identifier, uint id) : j1GuiEntity(rectangle, position,identifier,id) {

	type = IMAGE;
	start = true;
	resize = true;
}


void Image::Update()
{

	if (start == true)
	{

		for(int i=0;i<elements.size();i++)
		{
			elements[i]->position.x += position.x;
			elements[i]->position.y += position.y;
			elements[i]->diferential.x = (elements[i]->position.x - position.x);
			elements[i]->diferential.y = (elements[i]->position.y - position.y);
		}
		start = false;
	}

	for (int i = 0; i<elements.size(); i++)
	{
		elements[i]->position.x = position.x + elements[i]->diferential.x;
		elements[i]->position.y = position.y + elements[i]->diferential.y;
	}
}


void Image::Draw()
{

	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &Hitbox, 0);

}

void Image::AssignNumber(uint n)
{
	switch (n)
	{
	case 0:
		Hitbox.x = 259;
		Hitbox.y = 13;
		break;
	case 1:
		Hitbox.x = 267;
		Hitbox.y = 13;
		break;
	case 2:
		Hitbox.x = 275;
		Hitbox.y = 13;
		break;
	case 3:
		Hitbox.x = 283;
		Hitbox.y = 13;
		break;
	case 4:
		Hitbox.x = 291;
		Hitbox.y = 13;
		break;
	case 5:
		Hitbox.x = 259;
		Hitbox.y = 21;
		break;
	case 6:
		Hitbox.x = 267;
		Hitbox.y = 21;
		break;
	case 7:
		Hitbox.x = 275;
		Hitbox.y = 21;
		break;
	case 8:
		Hitbox.x = 283;
		Hitbox.y = 21;
		break;
	case 9:
		Hitbox.x = 291;
		Hitbox.y = 21;
		break;
	}

}

Image::~Image()
{
	elements.clear();
}

/////////////////////////////// TEXT METHODS ///////////////////////////////

Text::Text(const char* write, iPoint pos, uint size, std::string identifier, uint id) :text(write), j1GuiEntity({ 0,0,0,0 }, pos, identifier, id)
{

	type = TEXT;
	font = App->font->Load("fonts/zelda_fonts/ReturnofGanon.ttf", size);
	text_texture = App->font->Print(text, { (255),(255),(255),(255) }, font);
	App->font->CalcSize(write, Hitbox.w, Hitbox.h, font);
	Hitbox.w /= 2; //TODO MID Adapt functions to resolution scale
	Hitbox.h /= 2;
}


void Text::Draw()
{

	App->render->Blit(text_texture, position.x, position.y, NULL, 0, false);

}

void Text::Update()
{


}

void Text::Write(const char* string)
{

	text = string;
	font = App->font->Load("fonts/zelda_fonts/ReturnofGanon.ttf", 23);
	text_texture = App->font->Print(text, { (255),(255),(255),(255) }, font);

}

Text::~Text() {

}

/////////////////////////////// BUTTON METHODS ///////////////////////////////
Button::Button(SDL_Rect rectangle, iPoint pos, iPoint stat2, iPoint stat3, bool animated, const char* textstring, uint textsize, iPoint textpos, std::string identifier, uint id) :j1GuiEntity(rectangle, pos,identifier,id)
{
	type = BUTTON;
	state = normal;
	texture2.x = stat2.x;
	texture3.x = stat3.x;
	texture2.y = stat2.y;
	texture3.y = stat3.y;
	texture2.w = texture3.w = Hitbox.w;
	texture2.h = texture3.h = Hitbox.h;
	if (textstring != nullptr) {
		buttontext = new Text(textstring, { textpos.x,textpos.y }, textsize);
	}

	start = true;
	if (animated==true) {
		anim = new Animation();
	}

}

void Button::Draw()
{

	switch (state)
	{
	case normal:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &Hitbox, 0,resize);
		break;
	case over:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &texture2, 0, resize);
		break;
	case clicked:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &texture3, 0, resize);
		break;
	}
	if (buttontext != nullptr) {
		buttontext->Draw();	
	}
}

void Button::Update()
{
	if (anim != nullptr) {
		texture2 = anim->GetCurrentFrame();
	}
	if (selected == true)
	{
		if (click == true) 
		{
			state = clicked;
		}
		else 
		{
			state = over;
		}
	}
	else
	{
		state = normal;
	}
}

Button::~Button()
{
	delete buttontext;

}

/////////////////////////////// DIALOGUE METHODS ///////////////////////////////

Dialogue::Dialogue(const char*string) :j1GuiEntity({ 0,82,190,62 }, {40,150})
{
	//TODO MID: Actual font needs a blue outline to match the original one, need to code that or edit the font creating the outline
	type = DIALOGUE;
	text_lines.push_back(App->gui->CreateText(string, { position.x + 10, 0 }, 30, false));

}

void Dialogue::Draw()
{

	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &Hitbox, 0);
	SDL_Rect viewport = { 0,312,Hitbox.w * 2,Hitbox.h*1.5 + 5};//TODO LOW REMOVE MAGIC NUMBERS
	SDL_RenderSetViewport(App->render->renderer, &viewport);
	std::list<Text*>::iterator iterator = text_lines.begin();
	while (iterator != text_lines.end())
	{
		if (iterator._Ptr->_Myval->visible == true) {
			iterator._Ptr->_Myval->Draw();
		}
		iterator++;
	}
	SDL_RenderSetViewport(App->render->renderer, NULL);
}

void Dialogue::Update()
{
	if (push == true && SDL_GetTicks() - timer > 200)
	{
		push = false;
		PushLine(false);
	}

}

void Dialogue::AddLine(const char* string)
{
	std::list<Text*>::iterator iterator = text_lines.end();
	iterator--;
	iPoint pos = { position.x + 10,iterator._Ptr->_Myval->position.y + (iterator._Ptr->_Myval->Hitbox.h) }; // Hitbox/2 is for the resolution scale
	text_lines.push_back(App->gui->CreateText(string, { pos.x,pos.y }, 30, false));
}

void Dialogue::PushLine(bool push)
{
	std::list<Text*>::iterator iterator = text_lines.begin();
	while (iterator != text_lines.end())
	{
		iterator._Ptr->_Myval->position.y -= (iterator._Ptr->_Myval->Hitbox.h / 2) + 0.5;
		iterator++;
	}
	timer = SDL_GetTicks();
	this->push = push;
}

Dialogue::~Dialogue()
{
	//need to clear list;
}

/////////////////////////////// MENU METHODS ///////////////////////////////

Menu::Menu()
{
	id_selected = 0;
	visible = false;
}

void Menu::AddElement(j1GuiEntity* element)
{
	if (element->type == BUTTON) {
		element->id = menu_buttons.size() + 1;
		menu_buttons.push_back((Button*)element);
	}
	else 
	{
		menu_images.push_back((Image*)element);
	}
	
}

void Menu::Select(int value)
{
	//assert(id_selected + value < menu_elements.size() + 1 || id_selected + value >0);
	if (id_selected + value < menu_buttons.size() && id_selected + value >=0)
	{
			menu_buttons[id_selected]->selected = false;
			id_selected += value;
			menu_buttons[id_selected]->selected = true;
	}
}

void Menu::Click()
{
	menu_buttons[id_selected]->click = true;
}

void Menu::UnClick()
{
	menu_buttons[id_selected]->click = false;
}

void Menu::Open()
{
	for (uint i = 0; i < menu_buttons.size(); i++) {
		menu_buttons[i]->visible = true;
	}
	for (uint i = 0; i < menu_images.size(); i++) {
		menu_images[i]->visible = true;
		if (menu_images[i]->elements.size() > 0)
		{
			for (int j = 0; j < menu_images[i]->elements.size(); j++)
			{
				menu_images[i]->elements[j]->visible = true;
			}
		}
	}
	
	visible = true;
}

void Menu::Close()
{
	for (uint i = 0; i < menu_buttons.size(); i++) {
		menu_buttons[i]->visible = false;
	}
	for (uint i = 0; i < menu_images.size(); i++) {
		menu_images[i]->visible = false;
		if (menu_images[i]->elements.size() > 0)
		{
			for (int j = 0; j < menu_images[i]->elements.size(); j++)
			{
				menu_images[i]->elements[j]->visible = false;
			}
		}
	}
	visible = false;
}

void Menu::Move(bool x_axis, float speed) //bool x_axis is to know in wich axis do we move (x=true/y=false)
{
	if (x_axis)
	{
		for (int i = 0; i < menu_buttons.size(); i++)
		{
			menu_buttons[i]->position.x += speed;
		}
		for (int i = 0; i < menu_images.size(); i++)
		{
			menu_images[i]->position.x += speed;
		}
		position.x += speed;
	}
	else 
	{
		for (int i = 0; i < menu_buttons.size(); i++)
		{
			menu_buttons[i]->position.y += speed;
		}
		for (int i = 0; i < menu_images.size(); i++)
		{
			menu_images[i]->position.y += speed;
		}
		position.y += speed;
	}
}

Menu::~Menu()
{
	//need to clear vector;
}

// Entity Elements ---------------------------------------------------