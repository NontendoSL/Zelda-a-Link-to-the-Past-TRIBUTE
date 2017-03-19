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

/////////////////////////////// IMAGE METHODS ///////////////////////////////

Image::Image(SDL_Rect rectangle, iPoint position, std::string identifier, uint id) : j1GuiEntity(rectangle, position,identifier,id) {

	type = IMAGE;
	start = true;
}


void Image::Update()
{

	if (start == true)
	{

		std::list<Image*>::iterator iterator = elements.begin();
		while (iterator != elements.end())
		{
			iterator._Ptr->_Myval->position.x += position.x;
			iterator._Ptr->_Myval->position.y += position.y;
			iterator._Ptr->_Myval->diferential.x = (iterator._Ptr->_Myval->position.x - position.x);
			iterator._Ptr->_Myval->diferential.y = (iterator._Ptr->_Myval->position.y - position.y);
			iterator++;
		}
		start = false;
	}

	std::list<Image*>::iterator iterator = elements.begin();
	while (iterator != elements.end())
	{
		iterator._Ptr->_Myval->Update();
		iterator._Ptr->_Myval->position.x = position.x + iterator._Ptr->_Myval->diferential.x;
		iterator._Ptr->_Myval->position.y = position.y + iterator._Ptr->_Myval->diferential.y;
		iterator++;
	}
}


void Image::Draw()
{

	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &Hitbox, 0);
	std::list<Image*>::iterator iterator = elements.begin();
	while (iterator != elements.end())
	{
		iterator._Ptr->_Myval->Draw();
		iterator++;
	}
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
	std::list<Image*>::iterator iterator = elements.end();
	iterator--;
	if (iterator._Ptr->_Myval != nullptr)
		while (elements.size()>0)
		{
			elements.remove(iterator._Ptr->_Myval);
			iterator--;
		}
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
Button::Button(SDL_Rect rectangle, iPoint pos, iPoint stat2, iPoint stat3, const char* textstring, uint textsize, iPoint textpos, std::string identifier, uint id) :j1GuiEntity(rectangle, pos,identifier,id)
{
	type = BUTTON;
	state = normal;
	texture2.x = stat2.x;
	texture3.x = stat3.x;
	texture2.y = stat2.y;
	texture3.y = stat3.y;
	texture2.w = texture3.w = Hitbox.w;
	texture2.h = texture3.h = Hitbox.h;
	buttontext = new Text(textstring, { textpos.x,textpos.y }, textsize);
	start = true;
}

void Button::Draw()
{

	switch (state)
	{
	case normal:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &Hitbox, 0);
		break;
	case over:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &texture2, 0);
		break;
	case clicked:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &texture3, 0);
		break;
	}
	buttontext->Draw();
}

void Button::Update()
{
	if (selected == true)
	{
		state = over;
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

Dialogue::Dialogue(iPoint pos, const char*string) :j1GuiEntity({ 0,82,190,62 }, pos)
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
// Entity Elements ---------------------------------------------------