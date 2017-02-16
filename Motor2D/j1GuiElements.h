#ifndef __j1GUIELEMENTS_H__
#define __j1GUIELEMENTS_H__

#include "j1Module.h"

enum ButtonState{normal, over, clicked};
struct 	_TTF_Font;
// ---------------------------------------------------

class Image : public j1GuiEntity
{
public:
	void Update();
	void Draw();

public:
	Image(SDL_Rect rectangle, iPoint position);
	~Image();
public:
	bool start;
	p2List<j1GuiEntity*>elements;

};

// ----------------------------------------------------

class Button : public j1GuiEntity
{
public:
	void Draw();
	void Update();
public:
	Button(SDL_Rect rectangle, iPoint pos, iPoint stat2, iPoint stat3, const char* textstring, uint textsize, iPoint textpos);
	~Button();

public:
	bool start;
	ButtonState state;
	SDL_Rect texture2, texture3;
	Text* buttontext;
};

// -----------------------------------------------------

class Text : public j1GuiEntity
{
public:
	Text(const char* write, iPoint pos, uint size);
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

#endif // __j1GUIELEMENTS_H__