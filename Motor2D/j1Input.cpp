#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1App.h"
#include "j1InputManager.h"
#include "SDL/include/SDL.h"
#include "j1Render.h"

#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name="input";

	keyboard = new j1KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);
	memset(controller_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_CONTROLLER_BUTTONS);
}

// Destructor
j1Input::~j1Input()
{
	delete[] keyboard;
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	SDL_Init(SDL_INIT_GAMECONTROLLER);

	//Add mapping from config.xml
	SDL_GameControllerAddMapping(config.child("controller").attribute("file").as_string(""));

	if (SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	SDL_StartTextInput();
	//SDL_StopTextInput();

	/* Open the first available controller. */
	for (int i = 0; i < SDL_NumJoysticks(); ++i) 
	{
		if (SDL_IsGameController(i)) 
		{
			controller = SDL_GameControllerOpen(i);
			if (controller) 
			{
				break;
			}
			else 
			{
				fprintf(stderr, "Could not open gamecontroller %i: %s\n", i, SDL_GetError());
			}
		}
	}
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	mouse_motion_x = 0;
	mouse_motion_y = 0;

	static SDL_Event event;

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keys[i] == 1)
		{
			if (keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	for (int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if (mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if (mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}
	for (int i = 0; i < NUM_CONTROLLER_BUTTONS; ++i)
	{
		if (controller_buttons[i] == KEY_DOWN || controller_buttons[i] == KEY_REPEAT)
		{
			controller_buttons[i] = KEY_REPEAT;
			App->input_manager->InputDetected(i, EVENTSTATE::E_REPEAT);
		}
			
		if (controller_buttons[i] == KEY_UP)
		{
			App->input_manager->InputDetected(i, EVENTSTATE::E_UP);
			controller_buttons[i] = KEY_IDLE;
		}
	}

	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
				//case SDL_WINDOWEVENT_LEAVE:
			case SDL_WINDOWEVENT_HIDDEN:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				windowEvents[WE_HIDE] = true;
				break;

				//case SDL_WINDOWEVENT_ENTER:
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_FOCUS_GAINED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				windowEvents[WE_SHOW] = true;
				break;
			}
			break;

		case SDL_TEXTINPUT:
			last_input=event.text.text;
			input_print = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			mouse_buttons[event.button.button - 1] = KEY_DOWN;
			//LOG("Mouse button %d down", event.button.button-1);
			break;

		case SDL_MOUSEBUTTONUP:
			mouse_buttons[event.button.button - 1] = KEY_UP;
			//LOG("Mouse button %d up", event.button.button-1);
			break;

		case SDL_MOUSEMOTION:
		{
			int scale = App->win->GetScale();
			mouse_motion_x = event.motion.xrel / scale;
			mouse_motion_y = event.motion.yrel / scale;
			mouse_x = event.motion.x / scale;
			mouse_y = event.motion.y / scale;
			mouse_pos.x = event.motion.x / scale;
			mouse_pos.y = event.motion.y / scale;
			break;
		}

		case SDL_CONTROLLERBUTTONDOWN:
			controller_buttons[event.cbutton.button] = KEY_DOWN;
			App->input_manager->InputDetected(event.cbutton.button, EVENTSTATE::E_DOWN);
			break;

		case SDL_CONTROLLERBUTTONUP:
			controller_buttons[event.cbutton.button] = KEY_UP;
			//App->input_manager->InputDetected(event.cbutton.button, EVENTSTATE::E_UP);
			break;


		case SDL_CONTROLLERDEVICEADDED:
			if (SDL_IsGameController(event.cdevice.which))
			{
				controller = SDL_GameControllerOpen(event.cdevice.which);
			}
			break;

		case SDL_CONTROLLERDEVICEREMOVED:
			if (controller)
			{
				SDL_GameControllerClose(controller);
			}
			break;
		}
	}
	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");

	/* Attempt to open every controller. */
	if (controller)
	{
		//TODO MID
		//SDL_GameControllerClose(controller);
	}

	SDL_QuitSubSystem(SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER);
	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}

void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{
	x = mouse_motion_x;
	y = mouse_motion_y;
}

iPoint j1Input::GetMousePosition()
{
	return iPoint(mouse_x - App->render->camera.x / 2, mouse_y - App->render->camera.y / 2);
}

iPoint* j1Input::GetMousePosition_pointer()
{
	return &mouse_pos;
}