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

Image::Image(SDL_Rect rectangle, iPoint position, std::string identifier, GuiGroup group) : j1GuiEntity(rectangle, position, identifier, true, group) {

	type = IMAGE;
	start = true;
	resize = true;
}


void Image::Update(j1GuiEntity* focused)
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

Text::Text(FontName search, const char* write, SDL_Color color, uint length, iPoint pos, uint size, bool draw, std::string identifier, GuiGroup group) :size(size), draw(draw), length(length), j1GuiEntity({ 0,0,0,0 }, pos, identifier, true, group)
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

	//CheckString(std::string(write));
	text_texture = App->font->Print(text.c_str(), length, color, font);
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
{ //SDL has already a function that does this :////////////////////////
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
		next_line = App->gui->CreateText(font_name, string.substr(length + 1, 100).c_str(), length, { position.x,position.y + volum.y / 2 }, size, color, draw, "inside text", belong);
	}
}

void Text::Draw()
{

	App->render->Blit(text_texture, position.x, position.y, NULL, 0, false);

}

void Text::Update(j1GuiEntity* focused)
{


}

void Text::Write(const char* string)
{

	text = string;
	//font = App->font->Load("fonts/zelda_fonts/ReturnofGanon.ttf", 23);
	text_texture = App->font->Print(text.c_str(), length, color, font);

}

Text::~Text() {

}

/////////////////////////////// BUTTON METHODS ///////////////////////////////
Button::Button(SDL_Rect rectangle, iPoint pos, iPoint stat2, iPoint stat3, bool animated, std::string identifier, GuiGroup group, const char* textstring, uint textsize, iPoint textpos) :j1GuiEntity(rectangle, pos, identifier, true, group)
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
		buttontext = new Text(GANONF, textstring, { 255,255,255,255 }, 20, { textpos.x,textpos.y }, textsize, true, "undefined", group);
	}

	start = true;
	if (animated == true) {
		anim = new Animation();
	}
	focusable = true;
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
		buttontext->Draw(); //check this
	}
}

void Button::Update(j1GuiEntity* focused)
{
	if (anim != nullptr) {
		texture2 = anim->GetCurrentFrame();
	}
	if (focused == this)
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

void Dialogue::Update(j1GuiEntity* focused)
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
		while (item != nullptr)  //mirar metode eric funcio sdl per tallar string
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
	if (App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_DOWN || (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN))
	{
		if (position)
		{
			selector->position.y = second->position.y;
			position = false;
		}
	}
	if (App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_DOWN || (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN))
	{
		if (position == false)
		{
			selector->position.y = first->position.y;
			position = true;
		}
	}
	if (App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_DOWN || (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN))
	{

		App->gui->Erase(selector);
		App->gui->Erase(second);
		App->gui->Erase(first);
		Dialogue*item = (Dialogue*)parent;
		App->gui->Erase(item->options);
		item->options = nullptr;
		//item->Clear(3);
		App->gui->Erase(item);
		if (selector->position.y == first->position.y)
		{
			App->scene->switch_map = 7;
			App->scene->player->dialog = nullptr;
		}
		else
		{
			App->scene->gamestate = TIMETOPLAY;
			App->scene->player->SetState(L_IDLE);
			//App->scene->player->timetoplay.Start(); TUDU MEDIUM -> LOOK AT THIS
		}
	}
}

Selector::~Selector() {}
//////////////////////////////// Main Menu /////////////////////////////////

MainMenu::MainMenu()
{
	Button* menu_button = App->gui->CreateButton((j1Module*)App->intro, { 1,146,110,17 }, { 172 / 2,180 / 2 }, { 0,0 }, { 112,164 }, true, "Continue_b", MAIN_MENU);
	//menu_button->selected = true;
	menu_button->anim->PushBack({ 112,146,110,17 });
	menu_button->anim->PushBack({ 223,146,110,17 });
	menu_button->anim->PushBack({ 334,146,110,17 });
	menu_button->anim->PushBack({ 1,164,110,17 });
	menu_button->anim->PushBack({ 334,146,110,17 });
	menu_button->anim->PushBack({ 223,146,110,17 });
	menu_button->anim->speed = 0.25f;
	menu_button->resize = false;
	options.push_back(menu_button);
	menu_button = App->gui->CreateButton((j1Module*)App->intro, { 1,182,125,17 }, { 172 / 2,210 / 2 }, { 0,0 }, { 127,200 }, true, "Newgame_b", MAIN_MENU);
	menu_button->anim->PushBack({ 127,182,125,17 });
	menu_button->anim->PushBack({ 253,182,125,17 });
	menu_button->anim->PushBack({ 379,182,125,17 });
	menu_button->anim->PushBack({ 1,200,125,17 });
	menu_button->anim->PushBack({ 379,182,125,17 });
	menu_button->anim->PushBack({ 253,182,125,17 });
	menu_button->anim->speed = 0.25f;
	menu_button->resize = false;
	options.push_back(menu_button);
	menu_button = App->gui->CreateButton((j1Module*)App->intro, { 1,218,110,17 }, { 172 / 2, 240 / 2 }, { 0,0 }, { 112,236 }, true, "Loadgame_b", MAIN_MENU);
	menu_button->anim->PushBack({ 112,218,110,17 });
	menu_button->anim->PushBack({ 223,218,110,17 });
	menu_button->anim->PushBack({ 334,218,110,17 });
	menu_button->anim->PushBack({ 1,236,110,17 });
	menu_button->anim->PushBack({ 334,218,110,17 });
	menu_button->anim->PushBack({ 223,218,110,17 });
	menu_button->anim->speed = 0.25f;
	menu_button->resize = false;
	options.push_back(menu_button);
	belong = MAIN_MENU;
}

void MainMenu::Input()
{
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_DOWN)
	{
		Select(false);
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_DOWN)
	{
		Select(true);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_DOWN)
	{
		//assert(App->gui->GetFocused()->listener != nullptr);
		App->gui->GetFocused()->listener->OnGui(App->gui->GetFocused(), CLICK_DOWN);
	}
	else if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_UP)
	{
		App->gui->GetFocused()->listener->OnGui(App->gui->GetFocused(), CLICK_UP);
	}
}

Button* MainMenu::GetElement(uint id)
{
	return options[id];
}

void MainMenu::Select(bool next)
{
	for (int i = 0; i < options.size(); i++)
	{
		if (App->gui->GetFocused()->identifier == options[i]->identifier)
		{
			if (next)
			{
				if (options[i] == options.back())
				{
					App->gui->SetFocus((j1GuiEntity*)options.front());
				}
				else
				{
					App->gui->SetFocus((j1GuiEntity*)options[i + 1]);
				}
			}
			else
			{
				if (options[i] == options.front())
				{
					App->gui->SetFocus((j1GuiEntity*)options.back());
				}
				else
				{
					App->gui->SetFocus((j1GuiEntity*)options[i - 1]);
				}
			}
			return;
		}
	}
}

MainMenu::~MainMenu()
{

}

//////////////////////////////// ZELDA HUD ////////////////////////////////

ZeldaHud::ZeldaHud()
{
	Image* Sprite = App->gui->CreateImage({ 18,44,42,16 }, { 12,35 }, "charge", ZELDA_HUD);
	force = App->gui->CreateImage({ 21,61,34,10 }, { 4,3 }, "force", ZELDA_HUD, false);
	Sprite->elements.push_back(force);
	//hud->AddElement(Sprite);//[0] adding charge bar
	Sprite = App->gui->CreateImage({ 37,20,22,22 }, { 22,12 }, "item", ZELDA_HUD);
	picked = App->gui->CreateImage({ 279,257,16,16 }, { 3,3 }, "item_picked", ZELDA_HUD, false);
	Sprite->elements.push_back(picked);
	//hud->AddElement(Sprite);
	gems = App->gui->CreateImage({ 72,15,8,8 }, { 72,15 }, "gems", ZELDA_HUD); //Gems and value
	gems->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { -7,10 }, "number", ZELDA_HUD, false));
	gems->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 1,10 }, "number", ZELDA_HUD, false));
	gems->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 9,10 }, "number", ZELDA_HUD, false));
	//hud->AddElement(gems);
	//adding gems [2]
	bombs = App->gui->CreateImage({ 100,15,8,8 }, { 100,15 }, "bombs", ZELDA_HUD);
	bombs->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { -3,9 }, "number", ZELDA_HUD, false));
	bombs->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 5,9 }, "number", ZELDA_HUD, false));
	//hud->AddElement(bombs);
	// adding bombs
	arrows = App->gui->CreateImage({ 121,15,14,8 }, { 121,15 }, "arrows", ZELDA_HUD);
	arrows->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { -3,9 }, "number", ZELDA_HUD, false));
	arrows->elements.push_back(App->gui->CreateImage({ 259,13,7,7 }, { 5,9 }, "number", ZELDA_HUD, false));
	//hud->AddElement(arrows);
	//adding arrows
	hp = App->gui->CreateImage({ 178,15,44,7 }, { 178,15 }, "life", ZELDA_HUD);
	//hud->AddElement(hp);
	hp->elements.push_back(App->gui->CreateImage({ 177,24,7,7 }, { -21,9 }, "hp1", ZELDA_HUD, false));
	hp->elements.push_back(App->gui->CreateImage({ 177,24,7,7 }, { -12,9 }, "hp2", ZELDA_HUD, false)); //heart containers
	hp->elements.push_back(App->gui->CreateImage({ 177,24,7,7 }, { -3,9 }, "hp3", ZELDA_HUD, false));

	position = { 0,0 };
	identifier = "hud";
	belong = ZELDA_HUD;
}

void ZeldaHud::Update(j1GuiEntity* focused)
{
	AssignValues(gems, App->scene->player->gems);
	AssignValues(bombs, App->scene->player->bombs);
	AssignValues(arrows, App->scene->player->arrows);
	force->Hitbox.w = App->scene->player->charge;
}

void ZeldaHud::Input()
{

}

void ZeldaHud::Equip(const char* item)
{
	if (item == "bow")
	{
		picked->Hitbox.y = 276;
	}
	if (item == "hookshot")
	{
		picked->Hitbox.y = 309;
	}
	if (item == "bomb")
	{
		picked->Hitbox.y = 344;
	}
}

void ZeldaHud::Move(bool axis, float speed, bool move_all)
{
	App->gui->MoveGroup(ZELDA_HUD, axis, speed, move_all);
}

void ZeldaHud::AssignValues(Image* assigner, uint var)
{
	int number = var % 10, i = assigner->elements.size() - 1;
	assigner->elements[i--]->AssignNumber(number);
	number = var / 10;
	number %= 10;
	assigner->elements[i--]->AssignNumber(number);
	if (assigner->elements.size() > 2)
	{
		number = var / 100;
		assigner->elements[i]->AssignNumber(number);
	}
}

ZeldaHud::~ZeldaHud()
{


}
/////////////////////////////// MENU METHODS ///////////////////////////////

ZeldaMenu::ZeldaMenu()
{
	type = MENU;
	App->input_manager->AddListener(this);
	//items
	Image* Sprite = App->gui->CreateImage({ 1,255,256,224 }, { 0,-224 }, "bg", ZELDA_MENU);
	Hitbox = Sprite->Hitbox;//? unnecessary, look it after


	item_info = App->gui->CreateImage({ 370,372,32,32 }, { 18,154 - 224 }, "item_info", ZELDA_MENU);
	item_info_name = App->gui->CreateText(PIXEL, "SELECT AN ITEM", 200, { 19,193 - 224 }, 10, { 255,255,255,255 }, true, "item_info_name", ZELDA_MENU); // text name description
	item_equiped = App->gui->CreateImage({ 279,256,16,16 }, { 200,23 - 224 }, "item_picked", ZELDA_MENU);
	item_eq_name = App->gui->CreateText(PIXEL, "   PICK ITEM", 100, { 184,44 - 224 }, 10, { 255,255,255,255 }, true, "item_eq_name", ZELDA_MENU);
	App->gui->CreateImage({ 450,273,64,48 }, { 176,77 - 224 }, "pendants", ZELDA_MENU);
	item_info_desc = App->gui->CreateText(PIXELMORE, "Search for items!", 180, { 59,155 - 224 }, 20, { 255,255,255,255 }, true, "description_item_text", ZELDA_MENU);
	//Text* line = App->gui->CreateText(PIXELMORE, "The great warriors charge the bow to shoot further.", 22, { 59,155 - 224 }, 20, { 255,255,255,255 },true, "description item text", ZELDA_MENU);
	//line = App->gui->CreateText(PIXELMORE, "The more power you apply to it, more distance you will reach.", 22, { 59,155 - 224 }, 20);
	//line = App->gui->CreateText(PIXELMORE, "Use this bombs near big rocks to destroy them", 22, { 59,155 - 224 }, 20);

	//buttons items
	items.push_back(new ItemMenu(App->gui->CreateButton(App->scene, { 271,301,32,32 }, { 48,21 - 224 }, { 304,301 }, { 337,301 }, false, "hookshot", ZELDA_MENU)));
	items.push_back(new ItemMenu(App->gui->CreateButton(App->scene, { 271,336,32,32 }, { 72,21 - 224 }, { 304,336 }, { 337,336 }, false, "bomb", ZELDA_MENU)));


	for (int i = 0; i < items.size(); i++)
	{
		items[i]->ui_button->visible = false; //setting all the items invisible (unpicked)
	}

	position = { 0,-224 };
	//OpenClose(false);
	identifier = "start_menu";
	belong = ZELDA_MENU;
	//TODO HIGH-> edit Write function on item description
}

void ZeldaMenu::ResetInventory()
{
	/*if (menu_buttons.size() > 0)
	{
	menu_buttons.clear();
	}
	selected = nullptr;
	App->scene->hud->GetImage(1)->elements[0]->Hitbox.y = 257; //setting item selected in box to nothing
	menu_texts[2]->Visible(false);
	menu_texts[3]->Visible(false);
	menu_texts[4]->Visible(false);
	menu_images[1]->Hitbox.y = 372;
	menu_images[2]->Hitbox.y = 256;
	menu_texts[0]->Write("SELECT AN ITEM");
	menu_texts[1]->Write("PICK ITEM");*/

	//TODO MED-> redo this function

}

void ZeldaMenu::Select(bool next)
{
	if (empty == false)
	{
		int picked_id = 0;
		for (int i = 0; i < items.size(); i++)
		{
			if (App->gui->GetFocused()->identifier == items[i]->ui_button->identifier)
			{
				picked_id = i;
			}
		}
		if (next)
		{
			if (items[picked_id] != items.back())
			{
				for (int i = picked_id + 1; i != picked_id; i++)
				{
					if (items[i]->picked)
					{
						App->gui->SetFocus(items[i]->ui_button);
						return;
					}
					if (i == items.size() - 1)
					{
						i = -1;
					}
				}
			}
			else
			{
				for (int i = 0; i != picked_id; i++)
				{
					if (items[i]->picked)
					{
						App->gui->SetFocus(items[i]->ui_button);
						return;
					}
				}
			}
		}
		else
		{
			if (items[picked_id] != items.front())
			{
				for (int i = picked_id - 1; i != picked_id; i--)
				{
					if (items[i]->picked)
					{
						App->gui->SetFocus(items[i]->ui_button);
						return;
					}
					if (i == 0)
					{
						i = items.size();
					}
				}
			}
			else
			{
				for (int i = items.size() - 1; i != picked_id; i--)
				{
					if (items[i]->picked)
					{
						App->gui->SetFocus(items[i]->ui_button);
						return;
					}
				}
			}
		}
	}

	/*
	if (id_selected + value < menu_buttons.size() && id_selected + value >= 0)
	{
	menu_buttons[id_selected]->click = false;
	menu_buttons[id_selected]->selected = false;
	id_selected += value;
	menu_buttons[id_selected]->selected = true;
	selected = menu_buttons[id_selected];
	ShowItemInfo();
	}
	*/
}

void ZeldaMenu::Equip(const char* item)
{
	App->scene->player->Unequip();
	if (item == "bow")
	{
		item_equiped->Hitbox.y = 276;
		App->scene->player->Equip((Weapon*)App->scene->player->hook);
		item_eq_name->Write("BOW ARROWS");
	}
	if (item == "hookshot")
	{
		item_equiped->Hitbox.y = 309;
		App->scene->player->Equip((Weapon*)App->scene->player->hook);
		item_eq_name->Write(" HOOKSHOT");
	}
	if (item == "bomb")
	{
		item_equiped->Hitbox.y = 344;
		App->scene->player->Equip((Weapon*)App->scene->player->bombmanager);
		item_eq_name->Write("      BOMBS");
	}
	App->scene->hud->Equip(item);
	return;
}


void ZeldaMenu::Click()
{
	/*
	if (menu_buttons.size() > 0)
	{
	menu_buttons[id_selected]->click = true;
	Do();
	}*/

	//TODO: do this on OnGui();
}

void ZeldaMenu::Do()
{
	/*
	//Image* item = App->scene->hud->GetImage(1);
	//Image* item_menu = App->scene->start_menu->GetImage(2);
	// Create a function equip item to change hud and menu equiped sprite item;
	if (i_name == "bow")
	{
	App->scene->player->Unequip();
	//App->scene->player->Equip(bow);
	item->elements[0]->Hitbox.y = 276; // change hud equiped sprite
	item_menu->Hitbox.y = 276; // change menu equiped sprite
	menu_texts[1]->Write("BOW ARROWS"); //On listener->GuiAction put this aswell

	}
	else if (i_name == "hookshot")
	{
	App->scene->player->Unequip();
	if (App->scene->player->Equip((Weapon*)App->scene->player->hook) == true)
	{
	item->elements[0]->Hitbox.y = 309;
	item_menu->Hitbox.y = 309;
	menu_texts[1]->Write(" HOOKSHOT");
	}
	}
	else if (i_name == "bomb")
	{
	App->scene->player->Unequip();
	if (App->scene->player->Equip((Weapon*)App->scene->player->bombmanager) == true)
	{
	item->elements[0]->Hitbox.y = 344;
	item_menu->Hitbox.y = 344;
	menu_texts[1]->Write("      BOMBS");
	}
	}
	*/
}


void ZeldaMenu::UnClick()
{
	//if(menu_buttons.size()>0)
	//	menu_buttons[id_selected]->click = false;
}

void ZeldaMenu::Update(j1GuiEntity* focused)
{

}

void ZeldaMenu::Input()
{
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_DOWN)
	{
		Select(true);
		ShowItemInfo();
	}
	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_DOWN)
	{
		Select(false);
		ShowItemInfo();
	}
	else if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_DOWN)
	{
		//assert(App->gui->GetFocused()->listener != nullptr);
		if (App->gui->GetFocused() != nullptr)
		{
			App->gui->GetFocused()->listener->OnGui(App->gui->GetFocused(), CLICK_DOWN);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_UP || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_UP)
	{
		if (App->gui->GetFocused() != nullptr)
		{
			App->gui->GetFocused()->listener->OnGui(App->gui->GetFocused(), CLICK_UP);
		}
	}
}

void ZeldaMenu::OnInputCallback(INPUTEVENT action, EVENTSTATE e_state)
{
	if (App->scene->inventory == true && App->scene->ingame == true && this->identifier == "start_menu")
	{
		switch (action)
		{

		case MRIGHT:
			if (e_state == E_DOWN)
			{
				Select(1);
			}
			break;
		case MLEFT:
			if (e_state == E_DOWN)
			{
				Select(-1);
			}
			break;
		case BUTTON_A:
			if (e_state == E_DOWN)
			{
				Click();
			}
			else if (e_state == E_UP)
			{
				UnClick();
			}
			break;
		}
	}
}

/*void ZeldaMenu::OpenClose(bool open)
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
}*/

void ZeldaMenu::Move(bool x_axis, float speed) //bool x_axis is to know in wich axis do we move (x=true/y=false)
{
	App->gui->MoveGroup(ZELDA_MENU, x_axis, speed);
}

Button* ZeldaMenu::GetFirst()
{
	//	if (empty)
	//	return nullptr;

	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->picked == true)
		{
			if (items[i]->ui_button != nullptr)
				return items[i]->ui_button;
		}
	}
	return nullptr;
}

void ZeldaMenu::PickItem(const char* name)
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->ui_button->identifier == name)
		{
			items[i]->picked = true;
			items[i]->ui_button->visible = true;
			if (empty)
			{
				empty = false;
				App->gui->SetFocus(items[i]->ui_button);
			}
		}
	}

	//ShowItemInfo();
}


void ZeldaMenu::ShowItemInfo()
{
	if (empty == false)
	{
		if (App->gui->GetFocused()->identifier == "bow")
		{
			item_info->Hitbox.y = 268;
			item_info_name->Write("BOW ARROWS");
			item_info_desc->Write("The great warriors charge the bow to shoot further.");
		}
		if (App->gui->GetFocused()->identifier == "hookshot")
		{
			item_info->Hitbox.y = 301;
			item_info_name->Write("HOOKSHOT");
			item_info_desc->Write("The more power you apply to it, more distance you will reach.");
		}
		if (App->gui->GetFocused()->identifier == "bomb")
		{
			item_info->Hitbox.y = 336;
			item_info_name->Write("BOMBS");
			item_info_desc->Write("Can even destroy biggest rocks.");
		}
	}

	/*if (selected != nullptr)
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
	if (selected->identifier == "bomb")
	{
	menu_images[1]->Hitbox.y = 336;
	menu_texts[0]->Write("BOMBS");
	menu_texts[2]->Visible(false);
	menu_texts[3]->Visible(false);
	menu_texts[4]->Visible(true);
	}
	}*/

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
	hud_images.push_back(App->gui->CreateImage({ 335,19,254,51 }, { 0,0 }, "top hud", POKEMON_COMBAT));
	int hotfix = App->win->GetHeight() / App->win->GetScale();
	hud_images.push_back(App->gui->CreateImage({ 597,18,254,33 }, { 0,hotfix - 33 }, "bottom hud", POKEMON_COMBAT));
	//UP pokeballs
	//Link
	hud_images[0]->elements.push_back(App->gui->CreateImage({ 344,72,7,7 }, { 42,11 }, "Link_Pokeball_1", POKEMON_COMBAT));
	hud_images[0]->elements.push_back(App->gui->CreateImage({ 344,72,7,7 }, { 52,13 }, "Link_Pokeball_2", POKEMON_COMBAT));
	hud_images[0]->elements.push_back(App->gui->CreateImage({ 344,72,7,7 }, { 62,14 }, "Link_Pokeball_3", POKEMON_COMBAT));
	//Brendan
	hud_images[0]->elements.push_back(App->gui->CreateImage({ 344,72,7,7 }, { 204,11 }, "Bren_Pokeball_1", POKEMON_COMBAT));
	hud_images[0]->elements.push_back(App->gui->CreateImage({ 344,72,7,7 }, { 194,13 }, "Bren_Pokeball_2", POKEMON_COMBAT));
	hud_images[0]->elements.push_back(App->gui->CreateImage({ 344,72,7,7 }, { 184,14 }, "Bren_Pokeball_3", POKEMON_COMBAT));
	//Left sprites
	ability = App->gui->CreateImage({ 561,155,30,30 }, { 3,0 }, "left ability", POKEMON_COMBAT);
	hud_images[1]->elements.push_back(ability);
	ability->elements.push_back(App->gui->CreateImage({ 525,155,30,0 }, { 0,0 }, "left cd", POKEMON_COMBAT));
	hud_images[1]->elements.push_back(App->gui->CreateImage({ 426,84,90,23 }, { 35,7 }, "left box", POKEMON_COMBAT));
	//hp bar left
	hp1 = App->gui->CreateImage({ 464,110,48,2 }, { 38,16 }, "left hp", POKEMON_COMBAT);
	hud_images[1]->elements[1]->elements.push_back(hp1);
	//Right sprites
	hud_images[1]->elements.push_back(App->gui->CreateImage({ 525,85,30,30 }, { 221,0 }, "right ability", POKEMON_COMBAT));
	hud_images[1]->elements.push_back(App->gui->CreateImage({ 426,84,90,23 }, { 129,7 }, "right box", POKEMON_COMBAT));
	//hp bar right
	hp2 = App->gui->CreateImage({ 464,110,48,2 }, { 38,16 }, "right hp", POKEMON_COMBAT);
	hud_images[1]->elements[3]->elements.push_back(hp2);
	//Names
	hud_images[1]->elements[1]->elements.push_back(App->gui->CreateText(POKE1, Link->name.c_str(), 50, { 6,4 }, 15, { 0,0,0,255 }, true, "link_pk_name", POKEMON_COMBAT));
	hud_images[1]->elements[3]->elements.push_back(App->gui->CreateText(POKE1, Brendan->name.c_str(), 50, { 6,4 }, 15, { 0,0,0,255 }, true, "brendan_pk_name", POKEMON_COMBAT));
	// HP Poke Link
	hpbar_pLink = iPoint(Link->hp, Link->hp);
	sprintf_s(buffer, 25, "%i/%i", hpbar_pLink.y, hpbar_pLink.x);// CANVIAR A STD::STRING FRASE(%I,VALUE)
	poke_hp_Link = App->gui->CreateText(POKE1, buffer, 50, { 4,15 }, 12, { 0,0,0,255 }, true, "poke_hp_Link", POKEMON_COMBAT);
	hud_images[1]->elements[1]->elements.push_back(poke_hp_Link);
	// HP Poke Brendan
	hpbar_pBrendan = iPoint(Brendan->hp, Brendan->hp);
	sprintf_s(buffer, 25, "%i/%i", hpbar_pBrendan.y, hpbar_pBrendan.x);
	poke_hp_Brendan = App->gui->CreateText(POKE1, buffer, 50, { 4,15 }, 12, { 0,0,0,255 }, true, "poke_hp_brendan", POKEMON_COMBAT);
	hud_images[1]->elements[3]->elements.push_back(poke_hp_Brendan);

	//x-> Link - y->Brendan
	num_pokemons = { 3, 3 };
	cooldown = false;
	cdtime = iPoint(Link->cooldown, Link->cooldown);
	Link->target = Brendan;
	Brendan->target = Link;

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
			if (hpbar_pLink.x > 0)
			{
				hp1->Hitbox.w = (hpbar_pLink.y * 48) / hpbar_pLink.x;
			}

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
			if (hpbar_pBrendan.x > 0) {
				hp2->Hitbox.w = (hpbar_pBrendan.y * 48) / hpbar_pBrendan.x;
			}

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
				App->scene->player->gameover = App->gui->CreateImage({ 525,235,320,240 }, { 0,0 }, "lose");
			}
			else
			{
				App->scene->player->gameover->visible = true;
			}
			App->scene->switch_map = 6;
			OpenClose(false);
			//App->scene->start_menu->OpenClose(true);
			//App->scene->hud->OpenClose(true);
			if (App->scene->player->score - 250 > 0)
			{
				App->scene->player->score -= 250;
			}
		}
		else//VICTORY
		{
			int divider = 1;
			if (App->scene->player->winover != nullptr)
			{
				divider = 2;
			}
			switch (num_pokemons.x)
			{
			case 3:
				App->scene->player->score += (500 / divider);
				break;
			case 2:
				App->scene->player->score += (250 / divider);
				break;
			case 1:
				App->scene->player->score += (125 / divider);
				break;
			}
			if (App->scene->player->winover == nullptr || App->scene->player->winover->identifier == "lose") //first loop win
			{
				App->scene->player->winover = App->gui->CreateImage({ 1,481,320,240 }, { 0,0 }, "win");
				App->scene->player->winover->elements.push_back(App->gui->CreateText(POKE1, "LINK", 10, { 72,74 }, 40, { 38,38,38,255 }));
				App->scene->player->winover->elements.push_back(App->gui->CreateText(POKE1, std::string(std::to_string(App->scene->player->gems) + " RP").c_str(), 10, { 81,106 }, 40, { 38,38,38,255 }));
				int minutes = App->scene->player->game_timer.ReadSec() / 60;
				int sec = App->scene->player->game_timer.ReadSec() - minutes * 60;
				if (sec < 10)
					sec = 10;
				App->scene->player->winover->elements.push_back(App->gui->CreateText(POKE1, std::string(std::to_string(minutes) + ":" + std::to_string(sec)).c_str(), 10, { 106,127 }, 40, { 38,38,38,255 }));
				App->scene->player->score += App->scene->player->gems * 5;
				if (App->scene->player->score - App->scene->player->game_timer.ReadSec() > 0)
				{
					App->scene->player->score -= App->scene->player->game_timer.ReadSec();
				}
				App->scene->player->winover->elements.push_back(App->gui->CreateText(POKE1, std::string(std::to_string(App->scene->player->score)).c_str(), 10, { 83,149 }, 40, { 128,0,0,255 }));

				App->scene->switch_map = 1;
				App->scene->gamestate = INMENU;
				App->scene->pokecombat->OpenClose(false);
				App->scene->player->Unequip();
				App->entity_elements->DelteWeapons();
				App->scene->player->hook = nullptr;
				App->scene->player->bombmanager = nullptr;
				App->scene->start_menu->ResetInventory();
			}
			else if (App->scene->player->winover->identifier == "win" && App->scene->player->winover->visible == false) //second loop win
			{
				App->scene->player->winover->visible = true;
				Text* item = (Text*)App->scene->player->winover->elements[1];
				item->Write(std::string(std::to_string(App->scene->player->gems) + " RP").c_str());
				item = (Text*)App->scene->player->winover->elements[2];
				int minutes = App->scene->player->game_timer.ReadSec() / 60;
				int sec = App->scene->player->game_timer.ReadSec() - minutes * 60;
				if (sec < 10)
					sec = 10;
				item->Write(std::string(std::to_string(minutes) + ":" + std::to_string(sec)).c_str());
				App->scene->player->score += App->scene->player->gems * 5;
				if (App->scene->player->score - App->scene->player->game_timer.ReadSec() > 0)
				{
					App->scene->player->score -= App->scene->player->game_timer.ReadSec();
				}
				item = (Text*)App->scene->player->winover->elements[3];
				item->Write(std::string(std::to_string(App->scene->player->score)).c_str());
				App->scene->switch_map = 1;
				App->scene->gamestate = INMENU;
				App->scene->pokecombat->OpenClose(false);
				for (int i = 0; i < App->scene->player->winover->elements.size(); i++)
				{
					App->scene->player->winover->elements[i]->visible = true;
				}
				App->scene->player->Unequip();
				App->entity_elements->DelteWeapons();
				App->scene->player->hook = nullptr;
				App->scene->player->bombmanager = nullptr;
				App->scene->start_menu->ResetInventory();

			}
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
	pokemon->target = pokemon_2;

	//Pokemon 2
	Text* item = (Text*)hud_images[1]->elements[3]->elements[1];
	item->Write(pokemon_2->name.c_str());
	hpbar_pBrendan = iPoint(pokemon_2->hp, pokemon_2->hp);
	hp2->Hitbox.w = (hpbar_pBrendan.y * 48) / hpbar_pBrendan.x;
	sprintf_s(buffer, 25, "%i/%i", hpbar_pBrendan.y, hpbar_pBrendan.x);
	poke_hp_Brendan->Write(buffer);
	pokemon_2->target = pokemon;

	//TOP HUD BALLS
	for (int i = 0; i < hud_images[0]->elements.size(); i++)
	{
		hud_images[0]->elements[i]->Hitbox.x = 344;
	}
	//ability sprites
	ability->Hitbox.y = 155;
	ability->elements[0]->Hitbox.y = 155;

	num_pokemons = { 3, 3 };
}

void PokemonCombatHud::GetDamage(uint damage, bool trainer)
{
	if (trainer)
	{
		hpbar_pLink.y -= damage;
		sprintf_s(buffer, 25, "%i/%i", hpbar_pLink.y, hpbar_pLink.x);
		poke_hp_Link->Write(buffer);
	}
	else
	{
		hpbar_pBrendan.y -= damage;
		sprintf_s(buffer, 25, "%i/%i", hpbar_pBrendan.y, hpbar_pBrendan.x);
		poke_hp_Brendan->Write(buffer);
	}
}

void PokemonCombatHud::OpenClose(bool open)
{
	for (uint i = 0; i < hud_images.size(); i++)
	{
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

void PokemonCombatHud::Input()
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

void PokemonCombatHud::Update(j1GuiEntity* focused)
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

		if (hpbar_pLink.y <= 0)
		{
			num_pokemons.x--;
			LoadNewPokemon(App->combatmanager->change_pokemon(true), true);
		}
		else if (hpbar_pBrendan.y <= 0)
		{
			num_pokemons.y--;
			LoadNewPokemon(App->combatmanager->change_pokemon(false), false);
		}
		if (hpbar_pLink.x>0)
			hp1->Hitbox.w = (hpbar_pLink.y * 48) / hpbar_pLink.x;//being 48 the max pixels hp can have (atlas)
		if (hpbar_pBrendan.x>0)
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
