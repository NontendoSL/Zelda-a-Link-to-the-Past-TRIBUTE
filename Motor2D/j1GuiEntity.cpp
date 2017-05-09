#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1GuiEntity.h"

j1GuiEntity::j1GuiEntity() {

}
j1GuiEntity::j1GuiEntity(SDL_Rect rectangle, iPoint position, std::string identifier, bool resize, GuiGroup group) : Hitbox(rectangle), position(position), identifier(identifier), resize(resize), belong(group) {
	//id = Next_id++;
}

void j1GuiEntity::CalculateDiferential()
{
	if (parent != nullptr)
	{
		position.x += parent->position.x;
		position.y += parent->position.y;
		diferential.x = (position.x - parent->position.x);
		diferential.y = (position.y - parent->position.y);
	}
}

void j1GuiEntity::MoveInside(iPoint new_pos)
{
	if (parent != nullptr)
	{
		position = new_pos - parent->position;
		CalculateDiferential();
	}
}

void j1GuiEntity::Update(j1GuiEntity* focused) {



}

void j1GuiEntity::Draw() {



}

void j1GuiEntity::AssignNumber(uint n) {



}

j1GuiEntity::~j1GuiEntity() {

}

// class GuiEntity ---------------------------------------------------