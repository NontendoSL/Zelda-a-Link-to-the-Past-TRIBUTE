#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "Animation.h"
#include "j1Scene.h"
#include "j1SceneIntro.h"
#include "j1Gui.h"
#include "j1GuiElements.h"
#include "j1Player.h"
#include "PokemonCombat.h"
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

	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &Hitbox, 0,resize);

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
	App->font->CalcSize(write, Hitbox.w, Hitbox.h, font);
}

void Text::CheckString(std::string string)
{ //SDL has already a function that does this :////////////////////////
  /*
  iPoint volum;
  App->font->CalcSize(string.c_str(), volum.x, volum.y, font);
  if (string.size() > length)
  {
  text = string.substr(0, length + 1);
  if (text[length] != ' ') {
  text += "-";
  }
  next_line = App->gui->CreateText(font_name, string.substr(length + 1, 100).c_str(), length, { position.x,position.y + volum.y / 2 }, size, color, draw, "inside text", belong);
  }*/
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
	App->tex->UnLoad(text_texture);
	text = string;
	text_texture = App->font->Print(text.c_str(), length, color, font);

}

Text::~Text() 
{

	App->tex->UnLoad(text_texture);
	App->font->UnLoad(font);
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
	int win_marge = (App->win->GetWidth() - App->gui->GetEntity("bg")->Hitbox.w*App->win->GetScale()) / 4;
	position = { win_marge + 40,150 };
	//TODO MID: Actual font needs a blue outline to match the original one, need to code that or edit the font creating the outline
	type = DIALOGUE;
	lines = App->gui->CreateText(GANONF, string, 330, {5, 0 }, 30, { 255,255,255,255 }, false,"dialogue text", ZELDA_HUD);
	
	identifier = "dialogue";
	App->scene->gamestate = GameState::CUTSCENE;
	for (int i = lines->Hitbox.w*2; i > lines->length*2; enters++)
	{
		i -= lines->length;
	}
	if (enters == 0)
	{
		enters++;
	}
	App->scene->player->SetAnimState(LinkState::L_IDLE);
}

void Dialogue::Draw()
{
	
	App->render->Blit((SDL_Texture*)App->gui->GetAtlas(), position.x, position.y, &Hitbox, 0);
	SDL_Rect viewport = { (position.x*2)+6*2, (position.y * 2) + 6*2, 178*2, 50*2 };
	SDL_RenderSetViewport(App->render->renderer, &viewport);
	//App->render->DrawQuad({ -500,-500,999,999 }, 255, 255, 255, 170); UNCOMMENT TO SEE VIEWPORT
	lines->Draw();
	SDL_RenderSetViewport(App->render->renderer, NULL);
}

void Dialogue::Update(j1GuiEntity* focused)
{
	if (push == true && SDL_GetTicks() - timer > 200)
	{
		push = false;
		PushLine(false);
	}
	if (end)
	{
		Clear();
	}
}

void Dialogue::PushLine(bool push)
{
	if (end == false)
	{
		lines->position.y -= lines->Hitbox.h/4+1;
		timer = SDL_GetTicks();
		this->push = push;
		if (pushes == 15 || pushes == enters*2-1)
		{
			end = true;
			App->scene->gamestate = GameState::INGAME;
		}
		pushes++;
	}
}

void Dialogue::Clear()
{
	RELEASE(lines);
	App->gui->Erase(App->gui->GetEntity(identifier.c_str()));
	App->scene->player->dialog = nullptr;
}

Dialogue::~Dialogue()
{

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
	options.push_back((Button*)App->gui->GetEntity("Continue_b"));
	options.push_back((Button*)App->gui->GetEntity("Newgame_b"));
	options.push_back((Button*)App->gui->GetEntity("Loadgame_b"));

	for (int i = 0; i < options.size(); i++)
	{
		options[i]->resize = false; //buttons on main menu are not resized 
	}

	belong = MAIN_MENU;
}

void MainMenu::Input()
{
	if (active)
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
			App->gui->GetFocused()->listener->OnGui(App->gui->GetFocused(), CLICK_DOWN); //TODO HIGH MARC -> CLICK_DOWN
		}
		else if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_UP)
		{
			App->gui->GetFocused()->listener->OnGui(App->gui->GetFocused(), CLICK_UP);
		}
	}
}

Button* MainMenu::GetElement(uint id)
{
	active = true;
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

	force = (Image*)App->gui->GetEntity("force");
	picked = (Image*)App->gui->GetEntity("item_picked");
	gems = (Image*)App->gui->GetEntity("gems");
	bombs = (Image*)App->gui->GetEntity("bombs");
	arrows = (Image*)App->gui->GetEntity("arrows");
	hp = (Image*)App->gui->GetEntity("life");
	charge = (Image*)App->gui->GetEntity("charge");

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
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_START) == EVENTSTATE::E_DOWN)
	{
		App->scene->switch_menu = true;
	}
}

void ZeldaHud::Equip(const char* item)
{
	if (item == "none")
	{
		picked->Hitbox.y = 260;
		charge->Hitbox.x = 18;
		charge->Hitbox.y = 44;
		return;
	}
	if (item == "bow")
	{
		picked->Hitbox.y = 276;
		charge->Hitbox.x = 61;
		charge->Hitbox.y = 33;
		force->Hitbox.x = 65;
		force->Hitbox.y = 50;

	}
	if (item == "hookshot")
	{
		picked->Hitbox.y = 309;
		charge->Hitbox.x = 18;
		charge->Hitbox.y = 44;
		force->Hitbox.x = 21;
		force->Hitbox.y = 61;

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

	item_info = (Image*)App->gui->GetEntity("item_info");
	item_info_name = (Text*)App->gui->GetEntity("item_info_name");
	item_equiped = (Image*)App->gui->GetEntity("item_eq_sprite");
	item_eq_name = (Text*)App->gui->GetEntity("item_eq_name");
	item_info_desc = (Text*)App->gui->GetEntity("description_item_text");
	//buttons items
	items.push_back(new ItemMenu((Button*)App->gui->GetEntity("bow")));
	items.push_back(new ItemMenu((Button*)App->gui->GetEntity("hookshot")));
	items.push_back(new ItemMenu((Button*)App->gui->GetEntity("bomb")));
	for (int i = 0; i < items.size(); i++)
	{
		items[i]->ui_button->visible = false; //setting all the items invisible (unpicked)
	}
	//options buttons
	options.push_back(((Button*)App->gui->GetEntity("save_opt")));
	options.push_back(((Button*)App->gui->GetEntity("menu_opt")));
	options.push_back(((Button*)App->gui->GetEntity("quit_opt")));

	position = { 0,-224 };
	identifier = "start_menu";
	belong = ZELDA_MENU;
}

void ZeldaMenu::ResetInventory()
{

	for (int i = 0; i < items.size(); i++)
	{
		items[i]->picked = false;
		items[i]->ui_button->visible = false;
	}
	item_info->Hitbox.y = 370;
	item_info_name->Write("SELECT AN ITEM");
	item_equiped->Hitbox.y = 257;
	item_eq_name->Write("   PICK ITEM");
	item_info_desc->Write("Search for items!");
	empty = true;
	App->scene->hud->Equip("none");
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
						ShowItemInfo();
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
						ShowItemInfo();
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
						ShowItemInfo();
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
						ShowItemInfo();
						return;
					}
				}
			}
		}
	}
}

void ZeldaMenu::Equip(const char* item)
{
	App->scene->player->Unequip();
	if (item == "bow")
	{
		item_equiped->Hitbox.y = 276;
		App->scene->player->Equip((Weapon*)App->scene->player->bow);
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

void ZeldaMenu::Update(j1GuiEntity* focused)
{

}

void ZeldaMenu::SelectOption(bool down)
{
	for (int i = 0; i < options.size(); i++)
	{
		if (options[i] == App->gui->GetFocused())
		{
			if (down)
			{
				if (options[i] != options.back())
				{
					App->gui->SetFocus(options[i + 1]);
					return;
				}
				else 
				{
					App->gui->SetFocus(options[0]);
					return;
				}
			}
			else
			{
				if (options[i] != options.front())
				{
					App->gui->SetFocus(options[i - 1]);
					return;
				}
				else
				{
					App->gui->SetFocus(options[options.size()-1]);
					return;
				}
			}
		}
	}
}

void ZeldaMenu::Input()
{
	if (active)
	{
		if (on_options == false)
		{
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MRIGHT) == EVENTSTATE::E_DOWN)
			{
				Select(true);
			}
			else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MLEFT) == EVENTSTATE::E_DOWN)
			{
				Select(false);
			}
		}
		else
		{
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_DOWN)
			{
				SelectOption(true);
			}
			else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_DOWN)
			{
				SelectOption(false);
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_DOWN)
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
		if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_SELECT) == EVENTSTATE::E_DOWN)
		{
			on_options = !on_options;
			if (on_options)
			{
				App->gui->SetGui(ZELDA_MENU_OPTION);
			}
			else
			{
				App->gui->SetGui(ZELDA_MENU);
			}
		}
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_START) == EVENTSTATE::E_DOWN)
		{
			App->scene->switch_menu = true;
		}
	}
}

void ZeldaMenu::Move(bool x_axis, float speed) //bool x_axis is to know in wich axis do we move (x=true/y=false)
{
	App->gui->MoveGroup(ZELDA_MENU, x_axis, speed);
	App->gui->MoveGroup(ZELDA_MENU_OPTION, x_axis, speed);
}

Button* ZeldaMenu::GetFirst(bool option)
{
	active = true;
	if (option)
	{
		return options[0];
	}
	else 
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (items[i]->picked == true)
			{
				if (items[i]->ui_button != nullptr)
					return items[i]->ui_button;
			}
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

}

ZeldaMenu::~ZeldaMenu()
{

}


/////////////////////////////////////POKEMON COMBAT HUD//////////////////////////

PokemonCombatHud::PokemonCombatHud(PokemonCombat* Link, PokemonCombat* Brendan)
{

	int hotfix = App->win->GetHeight() / App->win->GetScale();
	//hp bar left
	hp1 = (Image*)App->gui->GetEntity("left hp");
	//hp bar right
	hp2 = (Image*)App->gui->GetEntity("right hp");
	//Names
	App->gui->GetEntity("left box")->elements.push_back(App->gui->CreateText(POKE1, Link->name.c_str(), 50, { 6,4 }, 15, { 0,0,0,255 }, true, "link_pk_name", POKEMON_COMBAT));
	App->gui->GetEntity("right box")->elements.push_back(App->gui->CreateText(POKE1, Brendan->name.c_str(), 50, { 6,4 }, 15, { 0,0,0,255 }, true, "brendan_pk_name", POKEMON_COMBAT));
	// HP Poke Link
	hpbar_pLink = iPoint(Link->hp, Link->hp);
	sprintf_s(buffer, 25, "%i/%i", hpbar_pLink.y, hpbar_pLink.x);// CANVIAR A STD::STRING FRASE(%I,VALUE)
	poke_hp_Link = App->gui->CreateText(POKE1, buffer, 50, { 4,15 }, 12, { 0,0,0,255 }, true, "poke_hp_Link", POKEMON_COMBAT);
	App->gui->GetEntity("left box")->elements.push_back(poke_hp_Link);
	// HP Poke Brendan
	hpbar_pBrendan = iPoint(Brendan->hp, Brendan->hp);
	sprintf_s(buffer, 25, "%i/%i", hpbar_pBrendan.y, hpbar_pBrendan.x); //CHANGE THIS FOR NOT USING THE BUFFER
	poke_hp_Brendan = App->gui->CreateText(POKE1, buffer, 50, { 4,15 }, 12, { 0,0,0,255 }, true, "poke_hp_brendan", POKEMON_COMBAT);
	App->gui->GetEntity("right box")->elements.push_back(poke_hp_Brendan);


	//x-> Link - y->Brendan
	num_pokemons = { 3, 3 };
	cooldown = false;
	cdtime = iPoint(Link->cooldown, Link->cooldown);
	Image*test = (Image*)App->gui->GetEntity("bottom hud");
	App->gui->GetEntity("bottom hud")->position.y = (App->win->GetHeight()/2) - App->gui->GetEntity("bottom hud")->Hitbox.h;
}

void PokemonCombatHud::Move(bool x_axis, float speed)
{
	App->gui->GetEntity("top hud")->position.x += speed;
	App->gui->GetEntity("bottom hud")->position.x += speed;
}

void PokemonCombatHud::LoadNewPokemon(PokemonCombat* pokemon, bool trainer) //true Link - false Brendan
{
	if (pokemon != nullptr)
	{
		if (trainer) //Link
		{
			if (num_pokemons.x == 2)
			{
				App->gui->GetEntity("Link_Pokeball_3")->Hitbox.x = 334;
				ability->Hitbox.y = 120;
				ability->elements[0]->Hitbox.y = 120;
			}
			if (num_pokemons.x == 1)
			{
				App->gui->GetEntity("Link_Pokeball_2")->Hitbox.x = 334;
				ability->Hitbox.y = 190;
				ability->elements[0]->Hitbox.y = 190;
			}

			((Text*)App->gui->GetEntity("link_pk_name"))->Write(pokemon->name.c_str());
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
				App->gui->GetEntity("Bren_Pokeball_3")->Hitbox.x = 334;
			}
			if (num_pokemons.y == 1)
			{
				App->gui->GetEntity("Bren_Pokeball_2")->Hitbox.x = 334;
			}

			((Text*)App->gui->GetEntity("brendan_pk_name"))->Write(pokemon->name.c_str());
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
			//OpenClose(false);
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
				//App->scene->pokecombat->OpenClose(false);
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
				//App->scene->pokecombat->OpenClose(false);
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

void PokemonCombatHud::CombatInfo(PokemonCombat* pokemon, PokemonCombat* pokemon_2)
{
	//pokemon 1
	((Text*)App->gui->GetEntity("link_pk_name"))->Write(pokemon->name.c_str());
	hpbar_pLink = iPoint(pokemon->hp, pokemon->hp);
	cdtime = iPoint(pokemon->cooldown, pokemon->cooldown);
	hp1->Hitbox.w = (hpbar_pLink.y * 48) / hpbar_pLink.x;
	ability->elements[0]->Hitbox.h = 0;
	cooldown = false;
	sprintf_s(buffer, 25, "%i/%i", hpbar_pLink.y, hpbar_pLink.x);
	poke_hp_Link->Write(buffer);
	pokemon->target = pokemon_2;

	//Pokemon 2
	((Text*)App->gui->GetEntity("brendan_pk_name"))->Write(pokemon_2->name.c_str());
	hpbar_pBrendan = iPoint(pokemon_2->hp, pokemon_2->hp);
	hp2->Hitbox.w = (hpbar_pBrendan.y * 48) / hpbar_pBrendan.x;
	sprintf_s(buffer, 25, "%i/%i", hpbar_pBrendan.y, hpbar_pBrendan.x);
	poke_hp_Brendan->Write(buffer);
	pokemon_2->target = pokemon;

	//TOP HUD BALLS
	for (int i = 0; i < App->gui->GetEntity("top hud")->elements.size(); i++)
	{
		App->gui->GetEntity("top hud")->elements[i]->Hitbox.x = 344;
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
			LoadNewPokemon(App->combatmanager->change_pokemon(), true);
		}
		else if (hpbar_pBrendan.y <= 0)
		{
			num_pokemons.y--; //TODO HIGH MARC
			LoadNewPokemon(App->combatmanager->change_pokemon(), false);
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

/////////////////////////////////////POKEMON WORLD HUD//////////////////////////

PokemonWorldHud::PokemonWorldHud()
{
	poke_bar.push_back((Button*)App->gui->GetEntity("pk_bar_hud_1"));
	poke_bar.push_back((Button*)App->gui->GetEntity("pk_bar_hud_2"));
	poke_bar.push_back((Button*)App->gui->GetEntity("pk_bar_hud_3"));
	j1GuiEntity*tt = App->gui->GetEntity("bag_coins_pk");
	App->gui->GetEntity("bag_coins_pk")->position.x = App->win->GetWidth()/2 - App->gui->GetEntity("bag_coins_pk")->position.x;
}

void PokemonWorldHud::Input()
{
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_SELECT) == EVENTSTATE::E_DOWN)
	{
		active = !active;
		if (active)
		{
			App->gui->SetFocus(GetFirst());
			App->scene->gamestate = INMENU;
		}
		else
		{
			CloseAll();
			App->scene->gamestate = INGAME;
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_START) == EVENTSTATE::E_DOWN)
	{
		if (active)
		{
			CloseAll();
			active = false;
		}
		App->scene->poke_menu->active = true;
		App->scene->poke_menu->MoveIn(true);
		App->gui->SetGui(POKEMON_MENU);
		App->scene->gamestate = INMENU;
	}
	if (active)
	{
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_DOWN)
		{
			Select(true);
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_DOWN)
		{
			Select(false);
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_B) == EVENTSTATE::E_DOWN)
		{
			selecting = !selecting;
			MoveOut(selecting);
			if (selecting)
				App->gui->GetFocused()->listener->OnGui(App->gui->GetFocused(), CLICK_DOWN);
			else
				App->gui->GetFocused()->listener->OnGui(App->gui->GetFocused(), CLICK_UP);
		}
	}
}

void PokemonWorldHud::Select(bool down)
{
	iPoint swap = { 0, 0 };
		for (int i = 0; i < poke_bar.size(); i++)
		{
			if (App->gui->GetFocused() == poke_bar[i])
			{
				swap.x = i;
				if (down)
				{
					if (i == poke_bar.size() - 1)
					{
						swap.y = 0;
					}
					else
					{
						swap.y = i + 1;
					}
				}
				else
				{
					if (i == 0)
					{
						swap.y = poke_bar.size() - 1;
					}
					else
					{
						swap.y = i - 1;
					}
				}
			}
		}
		if (selecting)
		{
			SwapBars(swap.x, swap.y);
		}
		else
		{
			App->gui->SetFocus(poke_bar[swap.y]);
		}
	
}

void PokemonWorldHud::SwapBars(uint first, uint second)
{
	int first_pos = poke_bar[first]->position.y;
	poke_bar[first]->position.y = poke_bar[second]->position.y;
	poke_bar[second]->position.y = first_pos;

	Button* first_b = poke_bar[first];
	poke_bar[first] = poke_bar[second];
	poke_bar[second] = first_b;


	//PLACEHOLDER FOR SWAPING POKEMONS ORDER IN PLAYER CODE SO IT AFFECTS COMBATS
}

std::string PokemonWorldHud::GetPokeOrder(uint poke_n)
{
	return poke_bar[poke_n]->elements[1]->identifier;
}

void PokemonWorldHud::MoveOut(bool out, int id)
{
	if (id == -1) // this is if we wanna move in/out an especific bar and not the focused one (testing)
	{
		if (out)
		{
			if(App->gui->GetFocused()->position.x<-65)
				App->gui->GetFocused()->position.x += 69;
		}
		else
		{
			if (App->gui->GetFocused()->position.x>-65)
				App->gui->GetFocused()->position.x -= 69;
		}
	}
	else
	{
		if (out)
		{
			if (App->gui->GetFocused()->position.x<-65)
				poke_bar[id]->position.x += 69;
		}
		else
		{
			if (App->gui->GetFocused()->position.x>-65)
				poke_bar[id]->position.x -= 69;
		}
	}
}

void PokemonWorldHud::CloseAll()
{
	if (selecting)
	{
		MoveOut(false);
		selecting = false;
	}
	App->gui->SetFocus(nullptr);
}

Button* PokemonWorldHud::GetFirst()
{
	if (active)
		return poke_bar[0];
	else
		return nullptr;
}

PokemonWorldHud::~PokemonWorldHud()
{


}

//----------------------------POKEMON WORLD MENU----------------------------

PokemonWorldMenu::PokemonWorldMenu()
{
	Image* bg_poke = (Image*)App->gui->GetEntity("pokemon menu bg");
	bg_poke->position.x = App->win->GetWidth() / 2;
	for (int i = 0; i < bg_poke->elements.size(); i++)
	{
		menu_opt.push_back((Button*)bg_poke->elements[i]);
	}
}

void PokemonWorldMenu::Input()
{
	if (active)
	{
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_DOWN)
		{
			Select(true);
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_DOWN)
		{
			Select(false);
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_DOWN)
		{
			App->gui->GetFocused()->listener->OnGui(App->gui->GetFocused(), CLICK_DOWN);
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_UP || App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_UP)
		{
			App->gui->GetFocused()->listener->OnGui(App->gui->GetFocused(), CLICK_UP);
		}
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_START) == EVENTSTATE::E_DOWN)
		{
			active = false;
			MoveIn(false);
			App->gui->SetGui(POKEMON_HUD);
			App->scene->gamestate = INGAME;
		}
	}

}


void PokemonWorldMenu::Select(bool down)
{

	for (int i = 0; i < menu_opt.size(); i++)
	{
		if (App->gui->GetFocused() == menu_opt[i])
		{
			if (down)
			{
				if (menu_opt[i] == menu_opt.back())
				{
					App->gui->SetFocus(menu_opt[0]);
					return;
				}
				else
				{
					App->gui->SetFocus(menu_opt[i+1]);
					return;
				}
			}
			else
			{
				if (menu_opt[i] == menu_opt.front())
				{
					App->gui->SetFocus(menu_opt[menu_opt.size() - 1]);
					return;
				}
				else
				{
					App->gui->SetFocus(menu_opt[i - 1]);
					return;
				}
			}
		}
	}
}

void PokemonWorldMenu::MoveIn(bool inside)
{
	if (inside && App->gui->GetEntity("pokemon menu bg")->position.x >= App->win->GetWidth()/2)
	{
		App->gui->GetEntity("pokemon menu bg")->position.x -= App->gui->GetEntity("pokemon menu bg")->Hitbox.w;
	}
	else if (App->gui->GetEntity("pokemon menu bg")->position.x<App->win->GetWidth() / 2)
	{
		App->gui->GetEntity("pokemon menu bg")->position.x += App->gui->GetEntity("pokemon menu bg")->Hitbox.w;
	}
}

Button* PokemonWorldMenu::GetFirst()
{
	return menu_opt[0];
}


PokemonWorldMenu::~PokemonWorldMenu()
{

}
// Entity Elements ---------------------------------------------------

PokemonWorldBag::PokemonWorldBag()
{

	for (int i = 0; i < App->gui->GetEntity("pokemon bag")->elements.size(); i++)
	{
		if (App->gui->GetEntity("pokemon bag")->elements[i]->type == BUTTON)
		{
			bag_item.push_back(new BagItem((Button*)App->gui->GetEntity("pokemon bag")->elements[i]));
			if (App->gui->GetEntity("pokemon bag")->elements[i]->identifier != "pk_bag:CLOSE")
			{
				bag_item[i - 1]->ui_button->visible = false;
				bag_item[i - 1]->ui_button->elements[0]->visible = false;
			}
		}
		App->gui->GetEntity("pokemon bag")->elements[i]->parent = App->gui->GetEntity("pokemon bag");
	}
	bag_item[4]->amount = 1;
	bag_item[4]->order = 0;
}

PokemonWorldBag::~PokemonWorldBag()
{

}

void PokemonWorldBag::Input()
{
	if (active)
	{
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MDOWN) == EVENTSTATE::E_DOWN)
		{
			Select(true);
		}
		else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::MUP) == EVENTSTATE::E_DOWN)
		{
			Select(false);
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN || App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_DOWN)
		{
			AddItem("pk_bag:DEF PROTEIN", true);
			App->gui->GetFocused()->listener->OnGui(App->gui->GetFocused(), CLICK_DOWN);
		}
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_UP || App->input_manager->EventPressed(INPUTEVENT::BUTTON_A) == EVENTSTATE::E_UP)
		{
			App->gui->GetFocused()->listener->OnGui(App->gui->GetFocused(), CLICK_UP);
		}
		if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{
			AddItem("pk_bag:DEF PROTEIN", false);
		}
		if (App->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
		{
			AddItem("pk_bag:CITRUS BERRY", true);
		}
		if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		{
			AddItem("pk_bag:CITRUS BERRY", false);
		}
	}
}

void PokemonWorldBag::Select(bool down)
{
	if (empty == false)
	{
		//int picked_id = 0;
		int number_pickeds = 0;
		int picked_order_id = 0;
		for (int i = 0; i < bag_item.size(); i++)
		{
			if (App->gui->GetFocused()->identifier == bag_item[i]->ui_button->identifier)
			{
				picked_order_id = bag_item[i]->order;
			}
			if (bag_item[i]->order > 0)
			{
				number_pickeds++;
			}
		}
		if (down)
		{
			if (picked_order_id != 0)
			{
				for (int i = 0; i < bag_item.size(); i++)
				{
					if (bag_item[i]->amount > 0 && bag_item[i]->order == picked_order_id - 1)
					{
						App->gui->SetFocus(bag_item[i]->ui_button);
						ShowItemInfo();
						return;
					}
				}
			}
			/*else
			{
				for (int i = 0; i < bag_item.size(); i++)
				{
					if (bag_item[i]->amount > 0 && i == number_pickeds)
					{
						App->gui->SetFocus(bag_item[i]->ui_button);
						//ShowItemInfo();
						return;
					}
				}
			}*/
		}
		else
		{
			if (picked_order_id != number_pickeds)
			{
				for (int i = 0; i < bag_item.size(); i++)
				{
					if (bag_item[i]->amount > 0 && bag_item[i]->order == picked_order_id + 1)
					{
						App->gui->SetFocus(bag_item[i]->ui_button);
						ShowItemInfo();
						return;
					}
				}
			}
			/*else
			{
					App->gui->SetFocus(bag_item[bag_item.size()-1]->ui_button);
					//ShowItemInfo();
					return;
			}*/
		}
	}

}

Button * PokemonWorldBag::GetFirst()
{
	active = true;
	return bag_item[bag_item.size() - 1]->ui_button;
}

void PokemonWorldBag::MoveIn(bool in)
{

}

bool PokemonWorldBag::AddItem(const char* identifier, bool add) //ret false if cant add more items (MAX 3)
{
	for (int i = 0; i < bag_item.size(); i++)
	{
		if (bag_item[i]->ui_button->identifier == identifier)
		{
			if (add)
			{
				if (bag_item[i]->amount == 0)
				{
					Insert(i);
					bag_item[i]->amount++;
					SetAmmount(i);
				}
				else if (bag_item[i]->amount == 3)
				{
					return false;
				}
				else
				{
					bag_item[i]->amount++;
					SetAmmount(i);
				}
			}
			else
			{
				if (bag_item[i]->amount == 1)
				{
					bag_item[i]->amount--;
					Remove(i);
				}
				else if (bag_item[i]->amount>1)
				{
					bag_item[i]->amount--;
					SetAmmount(i);
				}
			}
		}
	}

	return true;
}

void PokemonWorldBag::Insert(uint item_id) // TODO VERY VERY HIGH FOR GOLD (NOT FOR ALPHA) REDO THIS POSITION CALCULATION BCAUSE MOTHER OF GOD...
{
	bag_item[item_id]->ui_button->visible = true;
	bag_item[item_id]->ui_button->elements[0]->visible = true;
	bag_item[item_id]->ui_button->MoveInside(bag_item[bag_item.size() - 1]->ui_button->position);
	bag_item[bag_item.size() - 1]->ui_button->MoveInside({ bag_item[bag_item.size() - 1]->ui_button->position.x,bag_item[bag_item.size() - 1]->ui_button->position.y + bag_item[bag_item.size() - 1]->ui_button->Hitbox.h });
	bag_item[item_id]->order = 1;
	for (int i = 0; i < bag_item.size(); i++)
	{
		if (bag_item[i]->order > 0 && bag_item[item_id] != bag_item[i])
		{
			bag_item[i]->order++;
		}
	}
}

void PokemonWorldBag::Remove(uint item_id)
{
	bag_item[item_id]->ui_button->visible = false;
	bag_item[item_id]->ui_button->elements[0]->visible = false;
	for (int i = 0; i < bag_item.size(); i++)
	{
		if (bag_item[i]->ui_button->position.y > bag_item[item_id]->ui_button->position.y) // we check if the button is below the one we want to remove (so we have to push him up)
		{
			bag_item[i]->ui_button->MoveInside({ bag_item[i]->ui_button->position.x,bag_item[i]->ui_button->position.y - 18 });
		}
	}
	if (bag_item[item_id]->ui_button == App->gui->GetFocused())
	{
		Select(true);
	}
	for (int i = 0; i < bag_item.size(); i++)
	{
		if (bag_item[i]->order > bag_item[item_id]->order)
		{
			bag_item[i]->order--;
		}
	}
}

void PokemonWorldBag::SetAmmount(uint item_id)
{
	switch (bag_item[item_id]->amount)
	{
	case 0:
		Remove(item_id);
		break;
	case 1:
		bag_item[item_id]->ui_button->elements[0]->Hitbox.x = 631;
		break;
	case 2:
		bag_item[item_id]->ui_button->elements[0]->Hitbox.x = 648;
		break;
	case 3:
		bag_item[item_id]->ui_button->elements[0]->Hitbox.x = 665;
		break;
	}

}

void PokemonWorldBag::ShowItemInfo()
{

	if (App->gui->GetFocused()->identifier == "pk_bag:DEF PROTEIN")
	{
		((Text*)App->gui->GetEntity("bag_item_description"))->Write("Protein that raises a pokemon defense.");
		App->gui->GetEntity("item png")->Hitbox.x = 175;
	}
	if (App->gui->GetFocused()->identifier == "pk_bag:HP UP")
	{
		((Text*)App->gui->GetEntity("bag_item_description"))->Write("Increases the maximum HP of a pokemon.");
		App->gui->GetEntity("item png")->Hitbox.x = 197;
	}
	if (App->gui->GetFocused()->identifier == "pk_bag:X ATTACK")
	{
		((Text*)App->gui->GetEntity("bag_item_description"))->Write("Raises a pokemon basic attack damage.");
		App->gui->GetEntity("item png")->Hitbox.x = 241;
	}
	if (App->gui->GetFocused()->identifier == "pk_bag:CITRUS BERRY")
	{
		((Text*)App->gui->GetEntity("bag_item_description"))->Write("This magical berry fully heals your pokemons at the end of each combat.");
		App->gui->GetEntity("item png")->Hitbox.x = 219;
	}
	if (App->gui->GetFocused()->identifier == "pk_bag:CLOSE")
	{
		((Text*)App->gui->GetEntity("bag_item_description"))->Write("Closes the bag.");
		App->gui->GetEntity("item png")->Hitbox.x = 153;
	}
}