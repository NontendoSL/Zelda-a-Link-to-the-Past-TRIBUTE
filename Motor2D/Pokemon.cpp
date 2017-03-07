#include "Pokemon.h"

Pokemon::Pokemon(iPoint position) :NPC(position)
{
	name = "Pokemon";
	type = ENEMY;
}

Pokemon::~Pokemon()
{}

bool Pokemon::Awake(pugi::xml_node &conf, uint id)
{


	return true;
}

bool Pokemon::Start()
{


	return true;
}

bool Pokemon::Update()
{


	return true;
}

void Pokemon::Draw()
{

}

void Pokemon::AddItem(Item* item)
{

}

void Pokemon::Drop_item()
{

}

bool Pokemon::CleanUp()
{
	return true;
}




