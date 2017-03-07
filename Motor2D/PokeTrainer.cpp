#include "PokeTrainer.h"

PokeTrainer::PokeTrainer(iPoint position) :NPC(position)
{
	name = "PokeTrainer";
	type = ENEMY;
}

PokeTrainer::~PokeTrainer()
{}

bool PokeTrainer::Awake(pugi::xml_node &conf, uint id)
{


	return true;
}

bool PokeTrainer::Start()
{


	return true;
}

bool PokeTrainer::Update()
{


	return true;
}

void PokeTrainer::Draw()
{

}

void PokeTrainer::AddItem(Item* item)
{

}

void PokeTrainer::Drop_item()
{

}

bool PokeTrainer::CleanUp()
{
	return true;
}
