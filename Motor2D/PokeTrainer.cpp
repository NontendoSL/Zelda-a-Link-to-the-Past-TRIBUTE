#include "PokeTrainer.h"
#include "CombatManager.h"
#include "j1EntityElementsScene.h"
#include "j1App.h"

PokeTrainer::PokeTrainer() :NPC()
{

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
		Pokemon* poke = App->combatmanager->CreatePokemon(temp, temp.attribute("id").as_int(1));
		poke->pokemon_player = false;
		pokedex.push_back(poke);
	}
	
	active = conf.attribute("active").as_bool(false);
	return true;
}

bool PokeTrainer::Start()
{
	direction = DOWN;
	state = IDLE;

	if(active)
		collision_feet = App->collision->AddCollider({ position.x, position.y, 15, 21 }, COLLIDER_TRAINER, this);

	return true;
}

bool PokeTrainer::Update(float dt)
{
	

	return true;
}

void PokeTrainer::Draw()
{
	if (active)
	{
		if (direction == UP)
		{
			SDL_Rect r = { 1,1,14,21 };
			App->render->Blit(App->entity_elements->texture_trainer, position.x, position.y, &r);
		}
		if (direction == DOWN)
		{
			SDL_Rect r = { 46,1,14,21 };
			App->render->Blit(App->entity_elements->texture_trainer, position.x, position.y, &r);
		}
		if (direction == LEFT)
		{
			SDL_Rect r = { 31,23,14,21 };
			App->render->Blit(App->entity_elements->texture_trainer, position.x, position.y, &r);
		}
		if (direction == RIGHT)
		{
			SDL_Rect r = { 16,45,14,21 };
			App->render->Blit(App->entity_elements->texture_trainer, position.x, position.y, &r);
		}
	}
}

Pokemon* PokeTrainer::GetPokemon(uint position)
{
	std::list<Pokemon*>::iterator item = pokedex.begin();
	for (uint i = 0; i < pokedex.size(); i++)
	{
		if (i == position)
		{
			return item._Ptr->_Myval;
		}
		item++;
	}
}

bool PokeTrainer::CleanUp()
{
	return true;
}
