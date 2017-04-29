#include "BCTrooper.h"

BCTrooper::BCTrooper() : NPC()
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

bool BCTrooper::Update(float dt)
{


	return true;
}

void BCTrooper::Draw()
{

}


bool BCTrooper::CleanUp()
{
	return true;
}




