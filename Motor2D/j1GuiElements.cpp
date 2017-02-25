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

Image::Image(SDL_Rect rectangle, iPoint position): j1GuiEntity(rectangle, position){

	type = IMAGE;
	start = true;
}


void Image::Update() 
{

	if (start == true)
	{
		//p2List_item<j1GuiEntity*>*iterator = elements.start;
		std::list<j1GuiEntity*>::iterator iterator = elements.begin();
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

	//p2List_item<j1GuiEntity*>*iterator = elements.start;
	std::list<j1GuiEntity*>::iterator iterator = elements.begin();
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
	//p2List_item<j1GuiEntity*>*iterator = elements.start;
	std::list<j1GuiEntity*>::iterator iterator = elements.begin();
	while (iterator != elements.end())
	{
		iterator._Ptr->_Myval->Draw();
		iterator++;
	}
}

Image::~Image() 
{
	//p2List_item<j1GuiEntity*>*iterator = elements.start;
	std::list<j1GuiEntity*>::iterator iterator = elements.begin();
	if(iterator._Ptr->_Myval !=nullptr)
	while (iterator != elements.end())
	{
		delete iterator._Ptr->_Myval;
		iterator++;
	}
	elements.clear();
}

/////////////////////////////// TEXT METHODS ///////////////////////////////

Text::Text(const char* write, iPoint pos, uint size) :text(write), j1GuiEntity({ 0,0,0,0 }, pos) 
{

	type = TEXT;
	font = App->font->Load("fonts/zelda_fonts/ReturnofGanon.ttf", size);
	text_texture= App->font->Print(text, { (255),(255),(255),(255) }, font);
	App->font->CalcSize(write, Hitbox.w, Hitbox.h, font);
}


void Text::Draw() 
{

	App->render->Blit(text_texture, position.x, position.y, NULL, 0,false);

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
Button::Button(SDL_Rect rectangle, iPoint pos, iPoint stat2, iPoint stat3, const char* textstring, uint textsize, iPoint textpos):j1GuiEntity(rectangle, pos)
{
	type = BUTTON;
	state = normal;
	texture2.x = stat2.x;
	texture3.x = stat3.x;
	texture2.y = stat2.y;
	texture3.y = stat3.y;
	texture2.w = texture3.w = Hitbox.w;
	texture2.h = texture3.h = Hitbox.h;
	buttontext = new Text(textstring, {textpos.x,textpos.y}, textsize);
	start = true;
}

void Button::Draw() 
{
	
	switch (state) 
	{
		case normal:
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &Hitbox,0);
			break;
		case over:
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &texture2,0);
			break;
		case clicked:
			App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &texture3,0);
			break;
	}
	buttontext->Draw();
}

void Button::Update() 
{

	int x, y;
	App->input->GetMousePosition(x, y);
	if (start == true)
	{
		buttontext->position.x += position.x;
		buttontext->position.y += position.y;
		start = false;
	}
	if (x >= position.x && x <= (position.x + Hitbox.w) && y >= position.y  && y <= (position.y + Hitbox.h))
	{
		if (App->input->GetMouseButtonDown(1) == KEY_REPEAT)
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

Dialogue::Dialogue(iPoint pos, const char*string) :j1GuiEntity({ 0,82,190,62 }, pos)
{
	//TODO MID: Actual font needs a blue outline to match the original one, need to code that or edit the font creating the outline
	type = DIALOGUE;
	text_lines.push_back(App->gui->CreateText(string, { position.x + 10,position.y + 5 }, 20));
}

void Dialogue::Draw()
{
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &Hitbox, 0);
	std::list<Text*>::iterator iterator = text_lines.begin();
	while (iterator != text_lines.end())
	{
		iterator._Ptr->_Myval->Draw();
		iterator++;
	}
}

void Dialogue::Update()
{


}
void Dialogue::AddLine(const char* string)
{
	std::list<Text*>::iterator iterator = text_lines.end();
	iterator--;
	iPoint pos = { position.x + 10,iterator._Ptr->_Myval->Hitbox.h + position.y + 5 };
	text_lines.push_back(App->gui->CreateText(string, { pos.x,pos.y }, 20));
}

Dialogue::~Dialogue()
{
	//need to clear list;
}
// Entity Elements ---------------------------------------------------