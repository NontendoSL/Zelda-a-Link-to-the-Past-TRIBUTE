What is The Legend of Zelda: A Link to Pokemon World?
	
	It's a project for a subject called Project 2 of Video Games Design and Development  
	degree at Centre de la Imatge i Multimèdia (CITM), Terrassa. 
	It's an action RPG game based on the original "A Link to The past" (1991, SNES)
	and mixed with Pokémon Emerald (2004, GBA). 
	We have taken the essence of Zelda: a Link to the Past and 
	added our personal touch: an amazing journey to the Pokémon World. Link will become the best Pokémon 
	trainer after fighting against some of them with his sword on his hand. Ganon has rapped the princess 
	Zelda and you will have to do this travel to rescue her.


CONTROLS

	Keyboard:
	
		Arrow Keys -> Move in any of the 8 directions.
		Enter -> Start game from menu.
		Spacebar -> Hold it to charge the stamina bar.
		
	Controller:
	
		D-Pad -> move in any of the 8 directions.

	DEBUG MODE:
	
		F1 -> Display navigation map.
		F10 -> Display colliders.
		B -> Add rupees to your inventory.
		N -> Add arrows to your inventory.
		M -> Add bombs to your inventory.


CHANGELOG

v0.1:

	Added Link house map and surroundings.
	You can move using the arrow keys in the keyboard.
	Printed enemies are used as walls. They're not functional yet.
	Enemy and map collisions implemented.
	Link's house door doesn't get printed at the moment.
	
v0.2:

	Added static enemies with collisions (they don't have neither hitboxes nor hurtboxes).
	Added charging bar.
	Dynamic Objects have been implemented but they are not functional yet (they only have colliders).
	Game Intro has been added with provisional menu theme.
	Added Gamepad interaction (tested with Xbox 360 and PS3 so far) *Notice that you need to have your 
	controller plugged in before starting the game or else it will not work.
	Basic UI interaction regarding rupees, arrows and bombs.
	
	
AUTHORS

	We are a group of Video Games students from CITM, UPC fundation in Terrassa. Our members are:
		
		Jordi Oña
		Elliot Jiménez
		Sergio Álvarez
		Sergio Sáez
		Yabel Rodríguez
		Albert Llopart
		Marc Fabián

NOTES

	This project is made with SDL libraries.
	We implement pugixml library to manage our .tmx & .xml files.
	Brofiler is implemented to manage timmings in our game.