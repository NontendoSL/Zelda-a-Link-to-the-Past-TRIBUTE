#include "Ganon.h"

Ganon::Ganon() :NPC()
{
	name = "Ganon";
	type = CREATURE;
}

Ganon::~Ganon()
{}

bool Ganon::Awake(pugi::xml_node &conf, uint id)
{


	return true;
}

bool Ganon::Start()
{


	return true;
}

bool Ganon::Update()
{


	return true;
}

void Ganon::Draw()
{

}

bool Ganon::CleanUp()
{
	return true;
}




