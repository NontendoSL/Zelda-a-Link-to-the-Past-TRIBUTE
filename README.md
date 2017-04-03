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
	
		Arrow Keys -> Move in any of the 8 directions and navigate through the menu.
		Enter -> Start game from menu and open/close menu when in game.
		Spacebar -> Use the equipped item. Hold it to charge the stamina bar.
		R -> Select item in the menu.
		E -> Attack.
		Q -> Open chests.
		
	Controller:
	
		D-Pad -> move in any of the 8 directions and navigate through the menu.
		Start -> Navigate trhough game intro and open/close the ingame menu.
		A -> Select items from the menu. Open chests.
		X -> Attack
		B -> Use equipped item. Hold it to charge the stamina bar.

	DEBUG MODE:
	
		F1 -> Display navigation map.
		F10 -> Display colliders.
		B -> Add rupees to your inventory.
		N -> Add arrows to your inventory.
		M -> Add bombs to your inventory.
		X -> Lower Pokemon HP in combat.
		Z -> Triggers Ability Cooldown in combat.


CHANGELOG

v0.4:

	Pokemon are now displayed in Victory Road as enemies.
	Victory Road has been implemented.
	The hookshot has been implemented and it only works with Sudowoodos in the Pokemon World.
	The player will get the Hookshot in the first big chest of the Victory Road.
	Bombs have been implemented. The player will get them in the second big chest of the Victory Road.
	The player now can get hurt by the enemies.
	The enemies now get sent away after getting hit.
	Pokemon Combat map has been created. The player can switch between what Pokemon to use.
	The Pokemon UI has been implemented after Victory Road.
	
v0.3:

	Enemies, bushes, pots and chests now have hurtboxes.
	Attack has been implemented and now the player can either kill soldiers or destroy bushes, pots
	or the chest. They drop rupees when gone.
	Some soldiers now move randomly.
	UI has been updated. The player can now acces the item menu and can switch between weapons.

v0.2:

	Added static enemies with collisions (they don't have neither hitboxes nor hurtboxes).
	Added charging bar.
	Dynamic Objects have been implemented but they are not functional yet (they only have colliders).
	Game Intro has been added with provisional menu theme.
	Added Gamepad interaction (tested with Xbox 360 and PS3 so far). 
	Basic UI interaction regarding rupees, arrows and bombs.

v0.1:

	Added Link house map and surroundings.
	You can move using the arrow keys in the keyboard.
	Printed enemies are used as walls. They're not functional yet.
	Enemy and map collisions implemented.
	Link's house door doesn't get printed at the moment.
	
AUTHORS

	We are a group of Video Games students from CITM, UPC fundation in Terrassa. Our members are:
		
		Jordi Oña
		Elliot Jiménez
		Sergio Álvarez
		Sergio Sáez
		Yabel Rodríguez
		Albert Llopart
		Marc Fabián

SOCIAL MEDIA

Twitter -> https://twitter.com/nontendo_sl
Github -> https://github.com/NontendoSL/Zelda-a-Link-to-the-Past-TRIBUTE

NOTES

	This project is made with SDL libraries.
	We implement pugixml library to manage our .tmx & .xml files.
	Brofiler is implemented to manage timmings in our game.