#include "j1DynamicItems.h"

DynamicItems::DynamicItems(iPoint position) :j1SceneElement(position)
{
	type = DYNITEMS;
	name = "DynItems";
}

DynamicItems::~DynamicItems()
{

}

bool DynamicItems::Awake(pugi::xml_node &conf)
{
	return true;
}

bool DynamicItems::Start()
{
	return true;
}

bool DynamicItems::Update()
{
	return true;
}

void DynamicItems::Draw()
{

}

bool DynamicItems::CleanUp()
{
	return true;
}
