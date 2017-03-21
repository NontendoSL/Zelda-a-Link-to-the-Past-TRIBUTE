#include "BCTrooper.h"

BCTrooper::BCTrooper() :NPC()
{
	name = "BCTrooper";
	type = CREATURE;
}

BCTrooper::~BCTrooper()
{}

bool BCTrooper::Awake(pugi::xml_node &conf, uint id)
{


	return true;
}

bool BCTrooper::Start()
{


	return true;
}

bool BCTrooper::Update()
{


	return true;
}

void BCTrooper::Draw()
{

}

void BCTrooper::AddItem(Item* item)
{

}

void BCTrooper::Drop_item()
{

}

bool BCTrooper::CleanUp()
{
	return true;
}




