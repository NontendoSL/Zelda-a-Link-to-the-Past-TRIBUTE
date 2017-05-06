/*bool Swampert::CheckTargetPos()
{
int distance_target = target->position.DistanceTo(position);

if (distance_target <= radar)
{
if (attacker == false && state != PC_ATTACKING)
{
state = PC_ATTACKING;
anim_state = PC_ATTACKING;
current_animation = App->anim_manager->GetAnimation(anim_state, direction, SWAMPERT); //this number may need to be changed?
current_animation->Reset();
}
}

return true;
}*/

/*//TEST CHANGE RESOLUTION AND SIZE OF SCREEN
if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
{
changeResolution = !changeResolution;
}

if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
{
App->render->Blit(maptex, -App->render->camera.x, -App->render->camera.y);
}
if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
{
SDL_RenderSetLogicalSize(App->render->renderer, 800, 400);
if (changeResolution)
SDL_SetWindowSize(App->win->window, 800, 400);
}
if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
{
SDL_RenderSetLogicalSize(App->render->renderer, 300, 300);
if (changeResolution)
SDL_SetWindowSize(App->win->window, 300, 300);
}
if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
{
SDL_RenderSetLogicalSize(App->render->renderer, 500, 200);
if (changeResolution)
SDL_SetWindowSize(App->win->window, 500, 200);
}
if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
{
SDL_RenderSetLogicalSize(App->render->renderer, 1200, 500);
if (changeResolution)
SDL_SetWindowSize(App->win->window, 1200, 500);
}
if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
{
SDL_RenderSetLogicalSize(App->render->renderer, 1300, 800);
if (changeResolution)
SDL_SetWindowSize(App->win->window, 1300, 800);
}
if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
{
SDL_SetWindowSize(App->win->window, 800, 300);
}
*/


/*//TEST DRAW LIVE OF LINK
if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
hp -= 2;
if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
attack *= 2;
if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
Save();*/

/*stats_temp.clear();
stats_temp = std::to_string(hp);
stats_temp.insert(0, "HP LINK -> ");
hp_text->Write(stats_temp.c_str());
stats_temp.clear();
stats_temp = std::to_string(attack);
stats_temp.insert(0, "ATTACK OF LINK -> ");
attack_text->Write(stats_temp.c_str());*/

/*//TEST CHANGE RESOLUTION AND SIZE OF SCREEN
if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
{
changeResolution = !changeResolution;
}

if (App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
{
App->render->Blit(maptex, -App->render->camera.x, -App->render->camera.y);
}
if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
{
SDL_RenderSetLogicalSize(App->render->renderer, 800, 400);
if (changeResolution)
SDL_SetWindowSize(App->win->window, 800, 400);
}
if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
{
SDL_RenderSetLogicalSize(App->render->renderer, 300, 300);
if (changeResolution)
SDL_SetWindowSize(App->win->window, 300, 300);
}
if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
{
SDL_RenderSetLogicalSize(App->render->renderer, 500, 200);
if (changeResolution)
SDL_SetWindowSize(App->win->window, 500, 200);
}
if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
{
SDL_RenderSetLogicalSize(App->render->renderer, 1200, 500);
if (changeResolution)
SDL_SetWindowSize(App->win->window, 1200, 500);
}
if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
{
SDL_RenderSetLogicalSize(App->render->renderer, 1300, 800);
if (changeResolution)
SDL_SetWindowSize(App->win->window, 1300, 800);
}
if (App->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
{
SDL_SetWindowSize(App->win->window, 800, 300);
}
*/


/*//TEST DRAW LIVE OF LINK
if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
hp -= 2;
if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
attack *= 2;
if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
Save();*/

/*stats_temp.clear();
stats_temp = std::to_string(hp);
stats_temp.insert(0, "HP LINK -> ");
hp_text->Write(stats_temp.c_str());
stats_temp.clear();
stats_temp = std::to_string(attack);
stats_temp.insert(0, "ATTACK OF LINK -> ");
attack_text->Write(stats_temp.c_str());*/

/*if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
{

App->render->camera.h;
switch (dir)
{
case UP:
if (App->map->MovementCost(position.x, position.y - speed, UP) == 2)
{
App->map->EditCost(position.x + 8, position.y - speed, 0);
}
break;
case DOWN:
if (App->map->MovementCost(position.x, position.y + (speed + height), DOWN) == 2)
{
App->map->EditCost(position.x + 8, position.y + (speed + height), 0);
}
break;
case LEFT:
if (App->map->MovementCost(position.x - speed, position.y, UP) == 2)
{
App->map->EditCost(position.x - speed, position.y + 8, 0);
}
break;
case RIGHT:
if (App->map->MovementCost(position.x + (speed + width), position.y , UP) == 2)
{
App->map->EditCost(position.x , position.y + 8, 0);
}
break;
}
}*/

/*
void ZeldaMenu::Move(bool x_axis, float speed) //bool x_axis is to know in wich axis do we move (x=true/y=false)
{
if (x_axis)
{
for (int i = 0; i < menu_buttons.size(); i++)
{
menu_buttons[i]->position.x += speed;
}
for (int i = 0; i < menu_images.size(); i++)
{
menu_images[i]->position.x += speed;
if (menu_images[i]->elements.size() > 0)
{
if (x_axis == false)
{
for (int j = 0; j < menu_images[i]->elements.size(); j++)
{
menu_images[i]->elements[j]->position.x += speed;
}
}
}
}
for (int i = 0; i < menu_texts.size(); i++)
{
menu_texts[i]->position.x += speed;
if (menu_texts[i]->next_line != nullptr)
{
Text*line = menu_texts[i]->next_line;
while (line != nullptr)
{
line->position.x += speed;
line = line->next_line;
}
}
}
position.x += speed;
}
else
{
for (int i = 0; i < menu_buttons.size(); i++)
{
menu_buttons[i]->position.y += speed;
}
for (int i = 0; i < menu_images.size(); i++)
{
menu_images[i]->position.y += speed;
if (menu_images[i]->elements.size() > 0)
{
for (int j = 0; j < menu_images[i]->elements.size(); j++)
{
menu_images[i]->elements[j]->position.y += speed;
}
}
}
for (int i = 0; i < menu_texts.size(); i++)
{
menu_texts[i]->position.y += speed;
if (menu_texts[i]->next_line != nullptr)
{
Text*line = menu_texts[i]->next_line;
while (line != nullptr)
{
line->position.y += speed;
line = line->next_line;
}
}
}
position.y += speed;
}
}*/