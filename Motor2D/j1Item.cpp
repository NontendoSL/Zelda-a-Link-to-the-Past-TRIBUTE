#include "j1Item.h"


j1Item::j1Item(iPoint position) :j1SceneElement(position)
{
	type = ITEM;
}

j1Item::~j1Item()
{}

bool j1Item::Update()
{

	return true;
}

void j1Item::Draw()
{


}

