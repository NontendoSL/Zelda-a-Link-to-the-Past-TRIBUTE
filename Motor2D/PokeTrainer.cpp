#include "PokeTrainer.h"
#include "CombatManager.h"
#include "j1App.h"
PokeTrainer::PokeTrainer() :NPC()
{
	c_type = POKETRAINER;
}

PokeTrainer::~PokeTrainer()
{}

bool PokeTrainer::Awake(pugi::xml_node &conf)
{
	position.x = conf.attribute("pos_x").as_int(0);
	position.y = conf.attribute("pos_y").as_int(0);
	name = conf.attribute("name").as_string("");
	for (pugi::xml_node temp = conf.child("pokemon"); temp != NULL; temp = temp.next_sibling())
	{
		Pokemon* poke = App->combatmanager->CreatePokemon(temp, 1);
		poke->pokemon_player = false;
		pokedex.push_back(poke);
	}
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
