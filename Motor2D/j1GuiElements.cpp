#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "Animation.h"
#include "j1Scene.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"
#include "j1GuiElements.h"
#include "j1Player.h"
#include "j1Weapon.h"
#include <assert.h>

/////////////////////////////// IMAGE METHODS ///////////////////////////////

Image::Image(SDL_Rect rectangle, iPoint position, std::string identifier, uint id) : j1GuiEntity(rectangle, position, identifier, id) {

	type = IMAGE;
	start = true;
	resize = true;
}


void Image::Update()
{

	if (start == true)
	{

		for (int i = 0; i<elements.size(); i++)
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

Text::Text(FontName search, const char* write, SDL_Color color, uint length, iPoint pos, uint size, bool draw, std::string identifier, uint id) :size(size), draw(draw), length(length), j1GuiEntity({ 0,0,0,0 }, pos, identifier, id)
{
	type = TEXT;
	text = write;
	this->color = color;
	font_name = search;
	switch (search)
	{
	case GANONF:
		font = App->font->Load("fonts/zelda_fonts/ReturnofGanon.ttf", size);
		break;
	case PIXEL:
		font = App->font->Load("fonts/zelda_fonts/Pixeled.ttf", size);
		break;
	case PIXELMORE:
		font = App->font->Load("fonts/zelda_fonts/PixeledMore.ttf", size);
		break;
	case POKE1:
		font = App->font->Load("fonts/poke_fonts/poke1.ttf", size);
		break;
	default:
		font = App->font->Load("fonts/zelda_fonts/ReturnofGanon.ttf", size);
		break;
	}

	CheckString(std::string(write));
	text_texture = App->font->Print(text.c_str(), color, font);
	Hitbox.w /= 2; //TODO MID Adapt functions to resolution scale
	Hitbox.h /= 2;
}

void Text::Visible(bool yes)
{
	Text* item = next_line;
	if (yes) {
		visible = true;
		while (item != nullptr)
		{
			item->visible = true;
			item = item->next_line;
		}
	}
	else {
		visible = false;
		while (item != nullptr)
		{
			item->visible = false;
			item = item->next_line;
		}
	}
}

void Text::CheckString(std::string string)
{
	iPoint volum;
	/* int pixels;
	App->font->CalcSize("l", pixels, volum.x, font);*/
	App->font->CalcSize(string.c_str(), volum.x, volum.y, font);
	if (string.size() > length)
	{
		//int characters = length / pixels;
		text = string.substr(0, length + 1);
		if (text[length] != ' ') {
			text += "-";
		}
		next_line = App->gui->CreateText(font_name, string.substr(length + 1, 100).c_str(), length, { position.x,position.y + volum.y / 2 }, size, color,draw);
	}
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
	//font = App->font->Load("fonts/zelda_fonts/ReturnofGanon.ttf", 23);
	text_texture = App->font->Print(text.c_str(), color, font);

}

Text::~Text() {

}

/////////////////////////////// BUTTON METHODS ///////////////////////////////
Button::Button(SDL_Rect rectangle, iPoint pos, iPoint stat2, iPoint stat3, bool animated, std::string identifier, uint id, const char* textstring, uint textsize, iPoint textpos) :j1GuiEntity(rectangle, pos, identifier, id)
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
		buttontext = new Text(GANONF, textstring, { 255,255,255,255 }, 20, { textpos.x,textpos.y }, textsize, true, "undefined", 0);
	}

	start = true;
	if (animated == true) {
		anim = new Animation();
	}

}

void Button::Draw()
{
	switch (state)
	{
	case normal:
		App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &Hitbox, 0, resize);
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

Dialogue::Dialogue(const char*string) :j1GuiEntity({ 0,82,190,62 }, { 40,150 })
{
	//TODO MID: Actual font needs a blue outline to match the original one, need to code that or edit the font creating the outline
	type = DIALOGUE;
	lines = App->gui->CreateText(GANONF, string, 29, { position.x + 10, 0 }, 30, { 255,255,255,255 }, false);

}

void Dialogue::Draw()
{

	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &Hitbox, 0);
	SDL_Rect viewport = { 0,312,Hitbox.w * 3,Hitbox.h*1.5 + 5 };//TODO LOW REMOVE MAGIC NUMBERS
	SDL_RenderSetViewport(App->render->renderer, &viewport);
	lines->Draw();
	Text* item = lines;
	while (item != nullptr) {
		item->Draw();
		item = item->next_line;
	}
	/*std::list<Text*>::iterator iterator = text_lines.begin();
	while (iterator != text_lines.end())
	{
	if (iterator._Ptr->_Myval->visible == true) {
	iterator._Ptr->_Myval->Draw();
	}
	iterator++;
	}*/
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
	/*
	std::list<Text*>::iterator iterator = text_lines.end();
	iterator--;
	iPoint pos = { position.x + 10,iterator._Ptr->_Myval->position.y + (iterator._Ptr->_Myval->Hitbox.h) }; // Hitbox/2 is for the resolution scale
	text_lines.push_back(App->gui->CreateText(GANONF,string,50, { pos.x,pos.y }, 30, false));
	*/
}

void Dialogue::PushLine(bool push)
{
	/*
	std::list<Text*>::iterator iterator = text_lines.begin();
	while (iterator != text_lines.end())
	{
	iterator._Ptr->_Myval->position.y -= (iterator._Ptr->_Myval->Hitbox.h / 2) + 0.5;
	iterator++;
	}*/
	Text* item = lines;
	while (item != nullptr)
	{
		item->position.y -= (Hitbox.h / 2) + 0.5;
		item = item->next_line;
	}

	timer = SDL_GetTicks();
	this->push = push;
}

Dialogue::~Dialogue()
{
	//need to clear list;
}

/////////////////////////////// MENU METHODS ///////////////////////////////

ZeldaMenu::ZeldaMenu()
{
	id_selected = 0;
	visible = false;
	type = MENU;
	App->input_manager->AddListener(this);
}

void ZeldaMenu::AddElement(j1GuiEntity* element)
{
	if (element->type == BUTTON) {
		element->id = menu_buttons.size() + 1;
		menu_buttons.push_back((Button*)element);
	}
	if(element->type == IMAGE)
	{
		menu_images.push_back((Image*)element);
	}
	if (element->type == TEXT)
	{
		menu_texts.push_back((Text*)element);
	}
	
}

void ZeldaMenu::Select(int value)
{
	menu_buttons[id_selected]->click = false;
	//assert(id_selected + value < menu_elements.size() + 1 || id_selected + value >0);
	if (id_selected + value < menu_buttons.size() && id_selected + value >=0)
	{
			menu_buttons[id_selected]->selected = false;
			id_selected += value;
			menu_buttons[id_selected]->selected = true;
			if (App->scene->inventory == true && App->scene->ingame == true && this->identifier == "start_menu")
			{
				ShowItemInfo();
			}

	}
}

void ZeldaMenu::Click()
{
	menu_buttons[id_selected]->click = true;
	Do();
}

void ZeldaMenu:: Do()
{
	std::string i_name = App->scene->start_menu->GetSelected()->identifier;
	Image* item = App->scene->hud->GetImage(1);
	Image* item_menu = App->scene->start_menu->GetImage(2);
	if (i_name == "bow")
	{
		item->elements[0]->Hitbox.y = 276;
		item_menu->Hitbox.y = 276;
		menu_texts[1]->Write("BOW ARROWS");
		App->scene->player->Unequip();
		//App->scene->player->Equip(bow);
	}
	else if (i_name == "hookshot")
	{
		item->elements[0]->Hitbox.y = 309;
		item_menu->Hitbox.y = 309;
		menu_texts[1]->Write(" HOOKSHOT");
		App->scene->player->Unequip();
		App->scene->player->Equip((Weapon*)App->scene->player->hook);
	}
	else if (i_name == "bomb")
	{
		item->elements[0]->Hitbox.y = 344;
		item_menu->Hitbox.y = 344;
		menu_texts[1]->Write("      BOMBS");
		App->scene->player->Unequip();
		//App->scene->player->Equip(bombs);
	}
}

Image* ZeldaMenu::GetImage(uint id)
{
	return menu_images[id];
}

Button* ZeldaMenu::GetSelected()
{
	return menu_buttons[id_selected];
}

void ZeldaMenu::UnClick()
{
	menu_buttons[id_selected]->click = false;
}

void ZeldaMenu::Open()
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
void ZeldaMenu::Update()
{

	

}

void ZeldaMenu::Handle_Input()
{
	if (App->scene->inventory == true && App->scene->ingame == true && this->identifier=="start_menu")
	{
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_DOWN)
		{
			Select(1);
		}
		else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_DOWN)
		{
			Select(-1);
		}
		else if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_DOWN)
		{
			Click();
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_UP || App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_UP)//TODO MED-> Check E_UP botton state
		{
			UnClick(); 
		}
	}
}

/*void ZeldaMenu::OnInputCallback(INPUTEVENT, EVENTSTATE)
{
	if (App->scene->inventory == true && App->scene->ingame == true && this->identifier == "start_menu")
	{
		if (App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_DOWN)
		{
			Select(1);
		}
		else if (App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_DOWN)
		{
			Select(-1);
		}
		else if (App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_DOWN)
		{
			Click();
		}
		if (App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_UP)
		{
			UnClick();
		}
	}
}*/

void ZeldaMenu::Close()
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

void ZeldaMenu::Move(bool x_axis, float speed) //bool x_axis is to know in wich axis do we move (x=true/y=false)
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
			if (menu_images[i]->elements.size() > 0)
			{
				for (int j = 0; j < menu_images[i]->elements.size(); j++)
				{
					menu_images[i]->elements[j]->position.x += speed;
				}
			}
		}
		for (int i = 0; i < menu_texts.size(); i++)
		{
			menu_texts[i]->position.x += speed;
			if (menu_texts[i]->next_line != nullptr)
			{
				Text*line = menu_texts[i]->next_line;
				while (line != nullptr)
				{
					line->position.x += speed;
					line = line->next_line;
				}
			}
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
			if (menu_images[i]->elements.size() > 0)
			{
				for (int j = 0; j < menu_images[i]->elements.size(); j++)
				{
					menu_images[i]->elements[j]->position.y += speed;
				}
			}
		}
		for (int i = 0; i < menu_texts.size(); i++)
		{
			menu_texts[i]->position.y += speed;
			if (menu_texts[i]->next_line != nullptr)
			{
				Text*line = menu_texts[i]->next_line;
				while (line != nullptr)
				{
					line->position.y += speed;
					line = line->next_line;
				}
			}
		}
		position.y += speed;
	}
}

void ZeldaMenu::ShowItemInfo()
{
	switch (id_selected)
	{
	case 0:
		menu_images[1]->Hitbox.y = 268;
		menu_texts[0]->Write("BOW ARROWS");
		menu_texts[2]->Visible(true);
		menu_texts[3]->Visible(false);
		menu_texts[4]->Visible(false);
		break;
	case 1:
		menu_images[1]->Hitbox.y = 301;
		menu_texts[0]->Write("HOOKSHOT");
		menu_texts[2]->Visible(false);
		menu_texts[3]->Visible(true);
		menu_texts[4]->Visible(false);
		break;
	case 2:
		menu_images[1]->Hitbox.y = 336;
		menu_texts[0]->Write("BOMBS");
		menu_texts[2]->Visible(false);
		menu_texts[3]->Visible(false);
		menu_texts[4]->Visible(true);
		break;
	}

}

ZeldaMenu::~ZeldaMenu()
{
	//need to clear vector;
}


/////////////////////////////////////POKEMON COMBAT HUD//////////////////////////

PokemonCombatHud::PokemonCombatHud(uint cd_time, uint hpbar1, uint hpbar2)
{
	hud_images.push_back(App->gui->CreateImage({ 335,19,254,51 }, { 0,0 },"top hud"));
	hud_images.push_back(App->gui->CreateImage({ 597,18,254,33 }, { 0,224-33 }, "bottom hud"));
	ability = App->gui->CreateImage({ 561,85,30,30 }, { 3,0 }, "left ability");
	hud_images[1]->elements.push_back(ability);
	ability->elements.push_back(App->gui->CreateImage({ 525,85,30,0 }, { 0,0 }, "left cd"));
	hud_images[1]->elements.push_back(App->gui->CreateImage({ 426,84,90,23 }, { 35,7 }, "left box"));
	hp1= App->gui->CreateImage({ 464,110,48,2 }, { 38,16 }, "left hp");
	hud_images[1]->elements[1]->elements.push_back(hp1);
	hud_images[1]->elements[1]->elements.push_back(App->gui->CreateText(POKE1, "SCEPTILE", 50, { 6,4 }, 15, { 0,0,0,255 }));
	//
	sprintf_s(buffer, 25, "%i/%i", hpbar1, hpbar1);
	hp_print = App->gui->CreateText(POKE1, buffer, 50, { 4,15 }, 12, { 0,0,0,255 });
	hud_images[1]->elements[1]->elements.push_back(hp_print);
	hud_images[1]->elements.push_back(App->gui->CreateImage({ 525,85,30,30 }, { 221,0 }, "right ability"));
	hud_images[1]->elements.push_back(App->gui->CreateImage({ 426,84,90,23 }, { 129,7 }, "right box"));
	incd = false;
	cdtime.x = cd_time;
	this->hpbar1.x = hpbar1;
	this->hpbar1.y = hpbar1;
}


void PokemonCombatHud::Handle_Input()
{
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN && incd==false) 
	{
		incd = true;
		cdtime.y=cdtime.x;
	}
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT&&hpbar1.y>0)
	{
		hpbar1.y--;
	}
}

void PokemonCombatHud::Update()
{
	if (incd) //player cd update 
	{
		if (cdtime.y>0)
		{
			cdtime.y--;
			ability->elements[0]->Hitbox.h = (cdtime.y*ability->elements[0]->Hitbox.w)/ cdtime.x;
			LOG("setting cd");
		}
		else {
			incd = false;
		}
	}
	sprintf_s(buffer, 25, "%i/%i", hpbar1.y, hpbar1.x);
	hp_print->Write(buffer);
	hp1->Hitbox.w = (hpbar1.y * 48) / hpbar1.x;//being 48 the max pixels hp can have (atlas)
}

void PokemonCombatHud::SetCd(uint newcd)
{
	cdtime.x = newcd;
}

PokemonCombatHud::~PokemonCombatHud()
{

}


// Entity Elements ---------------------------------------------------