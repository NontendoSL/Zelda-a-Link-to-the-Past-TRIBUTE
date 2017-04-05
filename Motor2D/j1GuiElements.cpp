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
#include "Pokemon.h"
#include "CombatManager.h"
#include "j1Weapon.h"
#include "j1Window.h"
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
		next_line = App->gui->CreateText(font_name, string.substr(length + 1, 100).c_str(), length, { position.x,position.y + volum.y / 2 }, size, color, draw);
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
	int win_marge = (App->win->GetWidth() - App->scene->start_menu->Hitbox.w*App->win->GetScale()) / 4;
	position = { win_marge + 40,150 };
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

void Dialogue::PushLine(bool push)
{
	if (end == false)
	{
		Text* item = lines;
		//TODO LOW: Fix this sh done with magic numbers
		while (item != nullptr)
		{
			item->position.y -= (Hitbox.h / 8) + diferential; //+ 0.5
			item = item->next_line;
		}
		item = lines;
		while (item->next_line != nullptr)
		{
			item = item->next_line;
		}
		if (item->position.y + position.y + 8 < position.y)
		{
			options = App->gui->CreateSelector("Yes", "No", this);
			options->parent = this;
			end = true;
		}
		diferential += 0.2;
		timer = SDL_GetTicks();
		this->push = push;
	}
}

//TODO WORK on clear dialogue lines when u delete dialog
/*void Dialogue::Clear(int more_erased)
{
	Text* item = lines;
	Text* next_item = lines->next_line;
	int erased = more_erased;
	while (item != nullptr)
	{
		App->gui->Erase(item->vector_pos-erased);
		item = next_item;
		if (next_item != nullptr)
		{
			next_item = next_item->next_line;
		}
		erased++;
	}

}*/

Dialogue::~Dialogue()
{
	//need to clear text;
}

Selector::Selector(const char* first_option, const char* second_option, j1GuiEntity* parent)
{
	this->parent = parent;
	this->first = App->gui->CreateText(GANONF, first_option, 50, { parent->position.x + 20,parent->position.y + 5 }, 30);
	this->second = App->gui->CreateText(GANONF, second_option, 50, { parent->position.x + 20,parent->position.y + 20 }, 30);
	selector = App->gui->CreateText(GANONF, ">", 50, { first->position.x - 10,first->position.y }, 30);
	position = true;
}

void Selector::Handle_Input()
{
	if (App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_DOWN)
	{
		if (position)
		{
			selector->position.y = second->position.y;
			position = false;
		}
	}
	if (App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_DOWN)
	{
		if (position == false)
		{
			selector->position.y = first->position.y;
			position = true;
		}
	}
	if (App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_DOWN)
	{

		App->gui->Erase(selector->vector_pos);
		App->gui->Erase(second->vector_pos);
		App->gui->Erase(first->vector_pos);
		Dialogue*item = (Dialogue*)parent;
		App->gui->Erase(item->options->vector_pos);
		item->options = nullptr;
		//item->Clear(3);
		App->gui->Erase(item->vector_pos);
		if (selector->position.y == first->position.y)
		{
			App->scene->switch_map = 7;
			App->scene->player->dialog = nullptr;
		}
		else 
		{
			App->scene->player->gamestate = TIMETOPLAY;
			App->scene->player->state = IDLE;
			App->scene->player->timetoplay.Start();
		}
	}
}

Selector::~Selector() {}

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
	if (menu_buttons.size() == 0)
	{
		selected = (Button*)element;
		selected->selected = true;
		ShowItemInfo();
	}
	if (element->type == BUTTON) {
		element->id = menu_buttons.size() + 1;
		menu_buttons.push_back((Button*)element);
	}
	if (element->type == IMAGE)
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
	
	//assert(id_selected + value < menu_elements.size() + 1 || id_selected + value >0);
	if (id_selected + value < menu_buttons.size() && id_selected + value >= 0)
	{
		menu_buttons[id_selected]->click = false;
		menu_buttons[id_selected]->selected = false;
		id_selected += value;
		menu_buttons[id_selected]->selected = true;
		//if (App->scene->inventory == true && App->scene->ingame == true && this->identifier == "start_menu")
		//{
	
		//}
		selected = menu_buttons[id_selected];
		ShowItemInfo();
	}
	
}

void ZeldaMenu::Click()
{
	if (menu_buttons.size() > 0)
	{
	menu_buttons[id_selected]->click = true;
	Do();
	}

}

void ZeldaMenu::Do()
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
		App->scene->player->Equip((Weapon*)App->scene->player->bombmanager);
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
	if(menu_buttons.size()>0)
		menu_buttons[id_selected]->click = false;
}

void ZeldaMenu::Update()
{



}

void ZeldaMenu::Handle_Input()
{
	if (App->scene->inventory == true && App->scene->ingame == true && this->identifier == "start_menu")
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

void ZeldaMenu::OpenClose(bool open)
{
	for (uint i = 0; i < menu_buttons.size(); i++) {
		menu_buttons[i]->visible = open;
	}
	for (uint i = 0; i < menu_images.size(); i++) {
		menu_images[i]->visible = open;
		if (menu_images[i]->elements.size() > 0)
		{
			for (int j = 0; j < menu_images[i]->elements.size(); j++)
			{
				menu_images[i]->elements[j]->visible = open;
			}
		}
	}
	for (uint i = 0; i < menu_texts.size(); i++) {
		if (open == false)
		{
			menu_texts[i]->visible = open;
			if (menu_texts[i]->next_line != nullptr)
			{
				Text*line = menu_texts[i]->next_line;
				while (line != nullptr)
				{
					line->visible=open;
					line = line->next_line;
				}
			}
		}
		else if (i<2){
			menu_texts[i]->visible = open;
			if (menu_texts[i]->next_line != nullptr)
			{
				Text*line = menu_texts[i]->next_line;
				while (line != nullptr)
				{
					line->visible = open;
					line = line->next_line;
				}
			}
		}
	}
	visible = open;
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
				if (x_axis == false)
				{
					for (int j = 0; j < menu_images[i]->elements.size(); j++)
					{
						menu_images[i]->elements[j]->position.x += speed;
					}
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
	if (selected->identifier == "bow") 
	{
		menu_images[1]->Hitbox.y = 268;
		menu_texts[0]->Write("BOW ARROWS");
		menu_texts[2]->Visible(true);
		menu_texts[3]->Visible(false);
		menu_texts[4]->Visible(false);
	}
	if (selected->identifier == "hookshot")
	{
		menu_images[1]->Hitbox.y = 301;
		menu_texts[0]->Write("HOOKSHOT");
		menu_texts[2]->Visible(false);
		menu_texts[3]->Visible(true);
		menu_texts[4]->Visible(false);
	}
	if(selected->identifier =="bomb")
	{
		menu_images[1]->Hitbox.y = 336;
		menu_texts[0]->Write("BOMBS");
		menu_texts[2]->Visible(false);
		menu_texts[3]->Visible(false);
		menu_texts[4]->Visible(true);
	}

}

ZeldaMenu::~ZeldaMenu()
{
	//need to clear vector;
}


/////////////////////////////////////POKEMON COMBAT HUD//////////////////////////

PokemonCombatHud::PokemonCombatHud(Pokemon* Link, Pokemon* Brendan)
{
	//Hud_images 0-> up // Hud_imatge 1-> down
	//elements 0-> left circle // elements 1-> left box // elements 2-> right circle // elements 3-> right box
	//Big sprites
	hud_images.push_back(App->gui->CreateImage({ 335,19,254,51 }, { 0,0 }, "top hud"));
	int hotfix = App->win->GetHeight()/App->win->GetScale();
	hud_images.push_back(App->gui->CreateImage({ 597,18,254,33 }, { 0,hotfix-33  }, "bottom hud"));
	//UP pokeballs
	//Link
	hud_images[0]->elements.push_back(App->gui->CreateImage({ 344,72,7,7 }, { 42,11 }, "Link_Pokeball_1"));
	hud_images[0]->elements.push_back(App->gui->CreateImage({ 344,72,7,7 }, { 52,13 }, "Link_Pokeball_2"));
	hud_images[0]->elements.push_back(App->gui->CreateImage({ 344,72,7,7 }, { 62,14 }, "Link_Pokeball_3"));
	//Brendan
	hud_images[0]->elements.push_back(App->gui->CreateImage({ 344,72,7,7 }, { 204,11 }, "Bren_Pokeball_1"));
	hud_images[0]->elements.push_back(App->gui->CreateImage({ 344,72,7,7 }, { 194,13 }, "Bren_Pokeball_2"));
	hud_images[0]->elements.push_back(App->gui->CreateImage({ 344,72,7,7 }, { 184,14 }, "Bren_Pokeball_3"));
	//Left sprites
	ability = App->gui->CreateImage({ 561,155,30,30 }, { 3,0 }, "left ability");
	hud_images[1]->elements.push_back(ability);
	ability->elements.push_back(App->gui->CreateImage({ 525,155,30,0 }, { 0,0 }, "left cd"));
	hud_images[1]->elements.push_back(App->gui->CreateImage({ 426,84,90,23 }, { 35,7 }, "left box"));
	//hp bar left
	hp1 = App->gui->CreateImage({ 464,110,48,2 }, { 38,16 }, "left hp");
	hud_images[1]->elements[1]->elements.push_back(hp1);
	//Right sprites
	hud_images[1]->elements.push_back(App->gui->CreateImage({ 525,85,30,30 }, { 221,0 }, "right ability"));
	hud_images[1]->elements.push_back(App->gui->CreateImage({ 426,84,90,23 }, { 129,7 }, "right box"));
	//hp bar right
	hp2 = App->gui->CreateImage({ 464,110,48,2 }, { 38,16 }, "right hp");
	hud_images[1]->elements[3]->elements.push_back(hp2);
	//Names
	hud_images[1]->elements[1]->elements.push_back(App->gui->CreateText(POKE1, Link->name.c_str(), 50, { 6,4 }, 15, { 0,0,0,255 }));
	hud_images[1]->elements[3]->elements.push_back(App->gui->CreateText(POKE1, Brendan->name.c_str(), 50, { 6,4 }, 15, { 0,0,0,255 }));
	// HP Poke Link
	hpbar_pLink = iPoint(Link->hp, Link->hp);
	sprintf_s(buffer, 25, "%i/%i", hpbar_pLink.y, hpbar_pLink.x);
	poke_hp_Link = App->gui->CreateText(POKE1, buffer, 50, { 4,15 }, 12, { 0,0,0,255 });
	hud_images[1]->elements[1]->elements.push_back(poke_hp_Link);
	// HP Poke Brendan
	hpbar_pBrendan = iPoint(Brendan->hp, Brendan->hp);
	sprintf_s(buffer, 25, "%i/%i", hpbar_pBrendan.y, hpbar_pBrendan.x);
	poke_hp_Brendan = App->gui->CreateText(POKE1, buffer, 50, { 4,15 }, 12, { 0,0,0,255 });
	hud_images[1]->elements[3]->elements.push_back(poke_hp_Brendan);

	//x-> Link - y->Brendan
	num_pokemons = { 3, 3 };
	cooldown = false;
	cdtime = iPoint(Link->cooldown, Link->cooldown);

}

void PokemonCombatHud::Move(bool x_axis, float speed)
{
	hud_images[0]->position.x += speed;
	hud_images[1]->position.x += speed;
}

void PokemonCombatHud::LoadNewPokemon(Pokemon* pokemon, bool trainer) //true Link - false Brendan
{
	if (pokemon != nullptr)
	{
		if (trainer) //Link
		{
			if (num_pokemons.x == 2)
			{
				hud_images[0]->elements[2]->Hitbox.x = 334;
				ability->Hitbox.y = 120;
				ability->elements[0]->Hitbox.y = 120;
			}
			if (num_pokemons.x == 1)
			{
				hud_images[0]->elements[1]->Hitbox.x = 334;
				ability->Hitbox.y = 190;
				ability->elements[0]->Hitbox.y = 190;
			}

			Text*item = (Text*)hud_images[1]->elements[1]->elements[1];
			item->Write(pokemon->name.c_str());
			hpbar_pLink = iPoint(pokemon->hp, pokemon->hp);
			cdtime = iPoint(pokemon->cooldown, pokemon->cooldown);
			hp1->Hitbox.w = (hpbar_pLink.y * 48) / hpbar_pLink.x;
			ability->elements[0]->Hitbox.h = 0;
			cooldown = false;
			sprintf_s(buffer, 25, "%i/%i", hpbar_pLink.y, hpbar_pLink.x);
			poke_hp_Link->Write(buffer);
		}
		else //Brendan
		{
			if (num_pokemons.y == 2)
			{
				hud_images[0]->elements[5]->Hitbox.x = 334;
			}
			if (num_pokemons.y == 1)
			{
				hud_images[0]->elements[4]->Hitbox.x = 334;
			}
			Text*item = (Text*)hud_images[1]->elements[3]->elements[1];
			item->Write(pokemon->name.c_str());
			hpbar_pBrendan = iPoint(pokemon->hp, pokemon->hp);
			hp2->Hitbox.w = (hpbar_pBrendan.y * 48) / hpbar_pBrendan.x;
			sprintf_s(buffer, 25, "%i/%i", hpbar_pBrendan.y, hpbar_pBrendan.x);
			poke_hp_Brendan->Write(buffer);
		}
	}
	else
	{
		//FINAL COMBAT
		if (trainer) //LOSE
		{
			if (App->scene->player->gameover == nullptr)
			{
				App->scene->player->gameover = App->gui->CreateImage({ 525,235,320,240 }, { 0,0 });
			}
			else
				App->scene->player->gameover->visible = true;
			App->scene->switch_map = 6;
			OpenClose(false);
			App->scene->start_menu->OpenClose(true);
			App->scene->hud->OpenClose(true);
		}
		else//VICTORY
		{

		}
	}

}

void PokemonCombatHud::CombatInfo(Pokemon* pokemon, Pokemon* pokemon_2)
{
	//pokemon 1
	Text*item_1 = (Text*)hud_images[1]->elements[1]->elements[1];
	item_1->Write(pokemon->name.c_str());
	hpbar_pLink = iPoint(pokemon->hp, pokemon->hp);
	cdtime = iPoint(pokemon->cooldown, pokemon->cooldown);
	hp1->Hitbox.w = (hpbar_pLink.y * 48) / hpbar_pLink.x;
	ability->elements[0]->Hitbox.h = 0;
	cooldown = false;
	sprintf_s(buffer, 25, "%i/%i", hpbar_pLink.y, hpbar_pLink.x);
	poke_hp_Link->Write(buffer);

	//Pokemon 2
	Text* item = (Text*)hud_images[1]->elements[3]->elements[1];
	item->Write(pokemon_2->name.c_str());
	hpbar_pBrendan = iPoint(pokemon_2->hp, pokemon_2->hp);
	hp2->Hitbox.w = (hpbar_pBrendan.y * 48) / hpbar_pBrendan.x;
	sprintf_s(buffer, 25, "%i/%i", hpbar_pBrendan.y, hpbar_pBrendan.x);
	poke_hp_Brendan->Write(buffer);

	//TOP HUD BALLS
	for (int i = 0; i < hud_images[0]->elements.size(); i++)
	{
		hud_images[0]->elements[i]->Hitbox.x = 344;
	}
	//ability sprites
	ability->Hitbox.y = 155;
	ability->elements[0]->Hitbox.y = 155;
}

void PokemonCombatHud::OpenClose(bool open)
{
	for (uint i = 0; i < hud_images.size(); i++) {
		hud_images[i]->visible = open;
		if (hud_images[i]->elements.size() > 0)
		{
			for (int j = 0; j < hud_images[i]->elements.size(); j++)
			{
				hud_images[i]->elements[j]->visible = open;
				for (int k = 0; k < hud_images[i]->elements[j]->elements.size(); k++)
				{
					hud_images[i]->elements[j]->elements[k]->visible = open;
				}
			}
		}
	}
	visible = open;
}

void PokemonCombatHud::Handle_Input()
{
	/*if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN && cooldown == false)
	{
		cooldown = true;
		cdtime.y = cdtime.x;
	}*/
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT && hpbar_pLink.y>0)
	{
		hpbar_pLink.y--;
		sprintf_s(buffer, 25, "%i/%i", hpbar_pLink.y, hpbar_pLink.x);
		poke_hp_Link->Write(buffer);
	}
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_REPEAT && hpbar_pBrendan.y>0)
	{
		hpbar_pBrendan.y--;
		sprintf_s(buffer, 25, "%i/%i", hpbar_pBrendan.y, hpbar_pBrendan.x);
		poke_hp_Brendan->Write(buffer);
	}
}

void PokemonCombatHud::Update()
{
	if (App->scene->combat)
	{
		if (cooldown) //player cd update 
		{
			if (cdtime.y > 0)
			{
				cdtime.y--;
				ability->elements[0]->Hitbox.h = (cdtime.y*ability->elements[0]->Hitbox.w) / cdtime.x;
			}
			else
			{
				cooldown = false;
			}
		}

		if (hpbar_pLink.y == 0)
		{
			num_pokemons.x--;
			LoadNewPokemon(App->combatmanager->change_pokemon(true), true);
		}
		else if (hpbar_pBrendan.y == 0)
		{
			num_pokemons.y--;
			LoadNewPokemon(App->combatmanager->change_pokemon(false), false);
		}

		hp1->Hitbox.w = (hpbar_pLink.y * 48) / hpbar_pLink.x;//being 48 the max pixels hp can have (atlas)
		hp2->Hitbox.w = (hpbar_pBrendan.y * 48) / hpbar_pBrendan.x;//being 48 the max pixels hp can have (atlas)

	}
}

void PokemonCombatHud::SetCd(uint newcd)
{
	cdtime.x = newcd;
}

PokemonCombatHud::~PokemonCombatHud()
{

}


// Entity Elements ---------------------------------------------------