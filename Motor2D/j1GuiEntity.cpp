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
	vector_pos = App->gui->GetEntitiesSize() - 1;
}
j1GuiEntity::j1GuiEntity(SDL_Rect rectangle, iPoint position, std::string identifier, uint id, bool resize) : Hitbox(rectangle), position(position), identifier(identifier), id(id), resize(resize) {
	//id = Next_id++;
	vector_pos = App->gui->GetEntitiesSize() - 1;
}

void j1GuiEntity::CalculateDiferential()
{
	position.x += parent->position.x;
	position.y += parent->position.y;
	diferential.x = (position.x - parent->position.x);
	diferential.y = (position.y - parent->position.y);
}

void j1GuiEntity::Update() {



}

void j1GuiEntity::Draw() {



}

void j1GuiEntity::Handle_Input() {



}

void j1GuiEntity::AssignNumber(uint n) {



}

j1GuiEntity::~j1GuiEntity() {
	
}

// class GuiEntity ---------------------------------------------------