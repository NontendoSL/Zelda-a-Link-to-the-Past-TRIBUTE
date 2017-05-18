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
		Spacebar -> Use the equipped item. Hold it to charge the stamina bar. Click it to continue dialogs.
		R -> Select item in the menu.
		E -> Attack.
		Q -> Interact with chests and NPCs.
		Z -> Cast the special Ability in combat.
		+/- -> Up/Down the audio volume.
		0 -> Open shop in Pokémon World.
		
	Controller:
	
	NORMAL:

		D-Pad -> move in any of the 8 directions and navigate through the menu.
		Start -> Navigate trhough game intro and open/close the ingame menu.
		A -> Select items from the menu. Interact with chests and NPCs.
		X -> Attack.
		B -> Use equipped item. Hold it to charge the stamina bar (if equipped). Use Pokemon special ability in combat.

	TACTIC:

		D-Pad -> move in any of the 8 directions and navigate through the menu.
		Start -> Navigate trhough game intro and open/close the ingame menu.
		Y -> Select items from the menu. Interact with chests and NPCs.
		Left grid -> Attack.
		Right grid -> Use equipped item. Hold it to charge the stamina bar (if equipped). Use Pokemon special ability in combat.


	DEBUG MODE:
	
		F1 -> Display navigation map.
		F2 -> Display weapons collision map.
		F10 -> Display colliders.
		X -> Lower Pokémon HP in combat.
		I,J,K,L -> Movement mini-teleports (4 directions).
		G -> Defeats enemy pokémon to instawin the combat.
		1 -> Teleport to League Hall.
		3 -> Teleport to First Pokémon Combat.
		8 -> Teleport to Pyramid of Power(Boss Fight).
		

OBJECTIVE
	
	The main objective of this Vertical Slice is to show you the potential of our game through the core mechanics. We have divided
	it into 3 different parts:
		1st STAGE -> we are in Kakariko Village. Fight agains some soldiers to reach the "magic hole".

		2nd STAGE -> We are teletransported into the Victory Road of Hoenn, a beautiful region of the Pokémon World. Link is
			     confused, so he will explore this big cave to come back to his home.
			     - FIRST FLOOR > explore this level until achieve the hookshot inside a big chest. You will be able to pass across
			     the broken bridges, with the help of sudowoodos (the Pokémon Tree)
			     - SECOND FLOOR > big rocks will block your path, so take the bombs inside the other big chest and place them next
			     to the rocks. You will wake up the slept golems, and will chase you annoy them.
		3rd STAGE -> Wou will be in the combat arena, where Brendan will give you a little taste of an intense Pokémon Combat.

THE POKÉMON COMBAT
	
	You will have 3 Pokémon with a melee and a special attack, that will do more damage to the Pokémon enemy. The special attack
	has got a cooldown time to be cast again. At the moment, enemy IA won't attack you. It's only a test to see how this mechanic
	will work. (different Pokémon with different attacks & types that will do more or less damage depending the Pokémon).
	 

CHANGELOG

v0.9 ALPHA

	All game features have been added and finished.
	The boss and miniboss now have visible hp bar.
	The boss now has the 3 phases correctly implemented.
	Added Link uncle.
	Changed some paths in order to make the game easier and faster to the player.
	Pokémon items limited to 3 each Pokémon.
	Added new tactical control.
	Added menu shortcuts to change control and to close the game.
	Pokémon combat enhanced, all Pokémon have and do their habilities.
	Added master Ricard.
	Added Pokémon tutorial.
	Added intro placeholder.
	All dialogs enhanced.
	Save and Load function added.
v0.8

	The boss has been implemented.
	Added new enemies for the boss fight.
	All pokémon combat implemented and working.
	Player bag implemented.
	Bag UI implemented.
	Pokémon items implemented.
	Pokémon money implemented.
	The shop has been implemented, use 0 in Pokémon World.
	Shop UI implemented.
	All elite four members working properly.
	Minor map changes.
	Minor animation changes.

v0.7

	Added Pokémon World maps:
	- Victory League Outdoor.
	- Victory League Indoor.
	- Elite Four Dark Room.
	- Elite Four Ghost Room.
	- Elite Four Ice Room.
	- Elite Four Dragon Room.
	- League Champion Room.
	Added Zelda World last map:
	- Pyramid of Power.
	Added signs to show the path.
	More sounds, musics and Fx added.
	Now the bow has an indicator of when the arrow will be shot.
	Objects can be grabbed and can be thrown at enemies.
	Zelda UI upgraded, now you can exit by selecting exit option.
	Pokémon UI upgraded.
	Minor changes in sounds and Fx.
	Some dialog addition and changes.
	Added all pokemon NPC.

v0.6

	Added Chain & Ball Trooper (miniboss).
	All Zelda World maps implemented:
	 - Link house Outdoor.
	 - Link's Uncle's Room.
	 - Hyrule Castle (interior & exterior).
	 - Zelda's cell Room.
	Bow and arrows added (you can shoot them to damage and knockback enemies).
	Particle System implemented.
	Zelda UI corrections.
	Cutscene Placeholders added (Dialogues boxes to show some info of the game, at the moment).
	BOW MECHANIC: hold Spacebar/B until reach the half of the bar to shoot an arrow.
	
	*WARNING:* in this release, the player can not access to Pokemon World. 

v0.5 VERTICAL SLICE

	Now the game uses full screen.
	Geodudes will attack de player if he collides with them. They also drop bombs when dying.
	Golems will chase the player after having been woken up with a bomb.
	Soldiers now chase the player and show a more legit behaviour. The way player interacts with them
	has been adjusted to feel more real and fair.
	Picked items are show, in the menu correctly.	
	Big chests now are not refilled after re-entering their room.
	Game over screen added.
	Victory screen added (it shows playtime and score).
	More sounds, musics & Fx added.
	Added NPC Brendan. The player can talk to him to trigger an event (Pokémon combat).
	Pokémon combat prototype implemented with a new combat map.
	Victory Road has been modified in certain spots that would allow the player to finish the game
	sooner than intended.	

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
