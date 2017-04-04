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
	
	file_name = conf.attribute("file").as_string("");
	return true;
}

bool PokeTrainer::Start()
{
	direction = DOWN;
	state = IDLE;
	offset_y = 15;
	offset_x = 15;

	texture = App->tex->Load(file_name.c_str());

	collision_feet = App->collision->AddCollider({ position.x - offset_x, position.y - offset_y, 14, 14 }, COLLIDER_TRAINER, this);

	return true;
}

bool PokeTrainer::Update()
{
	

	return true;
}

void PokeTrainer::Draw()
{
	if (direction == UP)
	{
		SDL_Rect r = { 1,1,14,21 };
		App->render->Blit(texture, position.x, position.y, &r);
	}
	if (direction == DOWN)
	{
		SDL_Rect r = { 26,1,14,21 };
		App->render->Blit(texture, position.x, position.y, &r);
	}
	if (direction == LEFT)
	{
		SDL_Rect r = { 31,23,14,21 };
		App->render->Blit(texture, position.x, position.y, &r);
	}
	if (direction == RIGHT)
	{
		SDL_Rect r = { 16,45,14,21 };
		App->render->Blit(texture, position.x, position.y, &r);
	}
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
