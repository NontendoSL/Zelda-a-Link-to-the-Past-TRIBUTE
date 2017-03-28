#ifndef __j1GUIELEMENTS_H__
#define __j1GUIELEMENTS_H__

#include "j1Module.h"
#include "j1InputManager.h"

enum ButtonState{normal, over, clicked};
enum FontName{GANONF,PIXEL,PIXELMORE};
struct 	_TTF_Font;
class Animation;
class InputManager;
// ---------------------------------------------------

class Image : public j1GuiEntity
{
public:
	void Update();
	void Draw();

public:
	Image(SDL_Rect rectangle, iPoint position,std::string identifier, uint id);
	~Image();
	void AssignNumber(uint n);
public:
	bool start;


};

// ----------------------------------------------------

class Button : public j1GuiEntity
{
public:
	void Draw();
	void Update();
public:
	Button(SDL_Rect rectangle, iPoint pos, iPoint stat2, iPoint stat3, bool animated, std::string identifier, uint id, const char* textstring, uint textsize, iPoint textpos);
	~Button();

public:
	bool start, click;
	ButtonState state=normal;
	SDL_Rect texture2, texture3;
	Text* buttontext = nullptr;;
	Animation* anim=nullptr;
};

// -----------------------------------------------------

class Text : public j1GuiEntity
{
public:
	Text(FontName search, const char* write,uint length, iPoint pos, uint size,bool draw, std::string identifier, uint id);
	~Text();

public:
	void Draw();
	void Update();
	void Write(const char* string);
	void CheckString(std::string string);
	void Visible(bool yes);
private:
	std::string text;
	_TTF_Font* font;
	SDL_Texture* text_texture;
	uint length, size;
public:
	Text* next_line = nullptr;
	FontName font_name;
	bool draw = true;
};

// -------------------------------------------------------

class Dialogue: public j1GuiEntity
{
public:
	Dialogue(const char* string);
	~Dialogue();
public:
	void Draw();
	void Update();
	void AddLine(const char* string);
	void PushLine(bool push);
public:
	//std::list<Text*>text_lines;
	Text* lines;
	bool push;
	uint timer;
};

// ------------------------------------------------------

class ZeldaMenu :public j1GuiEntity, public InputListener
{
public:
	ZeldaMenu();
	~ZeldaMenu();
public:
	void Update();
	void Handle_Input();
	//void OnInputCallback(INPUTEVENT, EVENTSTATE); TOTO LOW -> check if its better to put button inputs here or not
public:
	void AddElement(j1GuiEntity* element);
	void Select(int value);
	void Open();
	void Close();
	void Move(bool axis, float speed);
	void Click();
	void UnClick();
	void Do();
	Image* GetImage(uint id);
	Button* GetSelected();
	void ShowItemInfo();
private:
	std::vector<Button*>menu_buttons;
	std::vector<Image*>menu_images;
	std::vector<Text*>menu_texts;
public:
	uint id_selected;
};

#endif // __j1GUIELEMENTS_H__