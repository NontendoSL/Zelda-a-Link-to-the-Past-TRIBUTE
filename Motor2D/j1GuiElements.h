#ifndef __j1GUIELEMENTS_H__
#define __j1GUIELEMENTS_H__

#include "j1Module.h"

enum ButtonState{normal, over, clicked};
struct 	_TTF_Font;
class Animation;
// ---------------------------------------------------

class Image : public j1GuiEntity
{
public:
	void Update();
	void Draw();

public:
	Image(SDL_Rect rectangle, iPoint position,std::string identifier="undefined", uint id=0);
	~Image();
	void AssignNumber(uint n);
public:
	bool start;
	std::list<Image*> elements;

};

// ----------------------------------------------------

class Button : public j1GuiEntity
{
public:
	void Draw();
	void Update();
public:
	Button(SDL_Rect rectangle, iPoint pos, iPoint stat2, iPoint stat3, bool animated, const char* textstring, uint textsize, iPoint textpos, std::string identifier="undefined", uint id = 0);
	~Button();

public:
	bool start;
	ButtonState state;
	SDL_Rect texture2, texture3;
	Text* buttontext = nullptr;;
	Animation* anim=nullptr;
};

// -----------------------------------------------------

class Text : public j1GuiEntity
{
public:
	Text(const char* write, iPoint pos, uint size, std::string identifier="undefined", uint id=0);
	~Text();

public:
	void Draw();
	void Update();
	void Write(const char* string);
private:
	const char* text;
	_TTF_Font* font;
	SDL_Texture* text_texture;

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
	std::list<Text*>text_lines;
	bool push;
	uint timer;
};

// ------------------------------------------------------

class Menu :public j1GuiEntity
{
public:
	Menu();
	~Menu();
public:
	void AddElement(j1GuiEntity* element);
	void Select(int value);
	void Open();
	void Close();
	void Move(bool axis, float speed);
private:
	std::vector<j1GuiEntity*>menu_elements;
public:
	uint id_selected;
};

#endif // __j1GUIELEMENTS_H__