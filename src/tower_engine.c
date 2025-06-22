// tower_engine.c

// Zugriff auf Bibliothecken
#include <stdio.h>		// Standart I/O Library
#include <stdbool.h>		// Verwenden von Boolians
#include <string.h>		// Verwenden von Strings
#include <math.h>	// Verwenden von sqrt();

#include <SDL3/SDL.h>

// Zugriff auf eigene Header
#include "../include/structs.h"
#include "../include/library.h"		
	bool running_first_frame;
	bool placing = false;
	float old_x = 0;
	float old_y = 0;

#include "../include/entity_engine.h"	
	int entityCount;
#include "../include/graphics_engine.h"	
#include "../include/tower_engine.h"	
#include "../include/player_engine.h"

// Deklarieren / Initialisieren von Variablen
Place place;

TowerData towers[MAX_TOWERS];
int towerCount = 0;

TowerData activeTowers[MAX_ACTIVE_TOWERS];
int activeTowerCount = 0;

// Initialisierung der Towers
TowerData crossbow = {				
	"Crossbow",						// Type
	"Shots at single Entitys over long range",	  // Description
	1,								// Texture Index
	{0, 0},							// Position

	90,								// Price
	5,								// Damage
	50,								// Reload Time in Frames
	500,							// Range
	true,							// Trifft Luft?
	false,							// Trifft Gepanzert?
	false							// Macht Splash Damage?
};
TowerData cannon = {
	"Cannon",						// Type
	"Deals Splash damage over medium range",  // Description
	2,								// Texture Index
	{0, 0},							// Position

	120,							// Price
	20,								// Damage
	100,							// Reload Time in Frames
	250,							// Range
	false,							// Trifft Luft?
	true,							// Trifft Gepanzert?
	true							// Macht Splash Damage?
};
TowerData minigun = {
	"Minigun",						// Type
	"Fastly shots at single Entitys over medium range",  // Description
	3,								// Texture Index
	{0, 0},							// Position

	175,							// Price
	1,								// Damage
	1,								// Reload Time in Frames
	250,							// Range
	true,							// Trifft Luft?
	false,							// Trifft Gepanzert?
	false							// Macht Splash Damage?
};
TowerData launcher = {
	"Rocket Launcher",				// Type
	"Deals Splash damage over long range",  // Description
	4,								// Texture Index
	{0, 0},							// Position

	200,							// Price
	15,								// Damage
	100,							// Reload Time in Frames
	1000,							// Range
	true,							// Trifft Luft?
	false,							// Trifft Gepanzert?
	true							// Macht Splash Damage?
};
TowerData saw = {
	"Saw",							// Type
	"Cuts through everything over small range",  // Description
	5,								// Texture Index
	{0, 0},							// Position

	200,							// Price
	2,								// Damage
	5,								// Reload Time in Frames
	50,								// Range
	false,							// Trifft Luft?
	false,							// Trifft Gepanzert?
	false							// Macht Splash Damage?
};
TowerData sniper = {
	"Sniper",						// Type
	"Shots with precision and infinite range",  // Description
	6,								// Texture Index
	{0, 0},							// Position

	225,							// Price
	30,								// Damage
	250,							// Reload Time in Frames
	1500,							// Range
	true,							// Trifft Luft?
	true,							// Trifft Gepanzert?
	false							// Macht Splash Damage?
};


// Abstand zwischen Punkt A & B berechnen
float getDistanceAB(Vector2 A, Vector2 B) {

	Vector2 C;

	C.x = A.x;
	C.y = B.y;

	int dis_ac = A.y - C.y;
	int dis_bc = B.x - C.x;

	if (dis_ac < 0) { dis_ac *= -1; }
	if (dis_bc < 0) { dis_bc *= -1; }

	float dis_ab = sqrt( (dis_ac * dis_ac) + (dis_bc * dis_bc) );

	return dis_ab;
}

// Tower zur generellen Towerliste hinzufügen
void addTower(TowerData t, int amount) {

	if (running_first_frame) {	// Nur im ersten Frame durchlaufen

		for (int i = 0; i < amount; i++) {

			if (towerCount >= MAX_TOWERS) {    // Sicherheitscheck
				printf("Too much Towers!\n");
				return;
			}

			// Daten auf Listeneintrag kopieren
			towers[towerCount] = t;

			towerCount++;	// Anzahl erhöhen
		}
		
	}                                    
}

// platzierten Tower zur Liste der aktiven Towers hinzufügen
void addToActiveTowers(TowerData t) {

	if (activeTowerCount >= MAX_ACTIVE_TOWERS) {
		printf("Too many active towers!\n");
		return;
	}

	activeTowers[activeTowerCount] = t;
	activeTowerCount++;
}

// Funktion um zu bestimmen ob Entity in Reichweite des Towers ist
bool entityInRange(TowerData t, EntityData* e) {

	float distance = getDistanceAB(t.position, e->position);

	if (distance <= t.range && e->health > 0) {

		return true;
	}

	return false;
}

// Funktion um zu bestimmen ob Tower Entity schaden zufügen kann
void dealDamage(TowerData t, EntityData* e) {

	if (e->health <= 0) return;

	// nur Anti-Luft kann Luft angreifen
	if (e->attr_air) {
		if (t.hits_air) {

			e->health -= t.damage;
			printf("dealt damage\nHP: %d\n", e->health);
		}
	}
	// nur Anti-Panzerung kann Gepanzert angreifen
	else if (e->attr_armored) {
		if (t.hits_armored) {

			e->health -= t.damage;
			printf("dealt damage\nHP: %d\n", e->health);
		}
	}
	else {

		// normale Entitys können von allen Towers angegriffen werden
		e->health -= t.damage;
		printf("dealt damage\nHP: %d\n", e->health);
	}
}


// Liste der aktiven Towers durchgehen und alle Einträge berechnen
void processActiveTowers(void) {

	for (int t = 0; t < activeTowerCount; t++) {		// Liste der aktiven Towers iterieren

		int attacked_entitys = 0;

		if (passedFrames(activeTowers[t].reload_time)) {	// Nachladezeit abwarten

			printf("reloaded\n");

			for (int e = 0; e < entityCount; e++) {	// Liste der Entitys iterieren

				if (entityInRange(activeTowers[t], &entities[e])) {	// überprüfen ob Entity in Reichweite ist

					printf("entity found\n");

					dealDamage(activeTowers[t], &entities[e]);	// Versuch Entity anzugreifen
					attacked_entitys++;

					// für Splash Damage, Schleife mehrfach durchlaufen
					if (activeTowers[t].hits_multiple) {
						if (attacked_entitys == 3) {
							break;
						}
					}
					else {
						break;
					}
				}
			}
		}
		
		// aktiven Tower rendern
		renderTower(activeTowers[t].textureIndex, activeTowers[t].position.x, activeTowers[t].position.y);
	}
}


// Platziere Tower mit Maus
void placeTower(TowerData t, InputState input) {

	// Place Indicator rendern
	renderIndicator(1, 0, 0);

	int unused_tower = -1;

	// Nach "freiem" Tower suchen
	for (int i = 0; i < towerCount; i++) {

		if (strcmp(towers[i].type, t.type) == 0) {
			if (towers[i].position.x == 0 && towers[i].position.y == 0) {
				unused_tower = i;
				break;
			}
		}
	}
	if (unused_tower == -1) { place = (Place){ 0 }; printf("no tower found\n"); }

	// Tower bei Mauszeiger rendern
	if (unused_tower >= 0) {

		// Normale Textur wenn genug Cash
		if (player.cash >= t.price) {
			renderTower(towers[unused_tower].textureIndex, input.x_mouse_position, input.y_mouse_position);
		}
		// Ausgegraute Textur wenn zu wenig Cash
		else {
			renderTower((towers[unused_tower].textureIndex + 100), input.x_mouse_position, input.y_mouse_position);
		}

		// Range Indicator bei Mauszeiger (= Tower) rendern
		renderIndicator(t.range, input.x_mouse_position, input.y_mouse_position);
	}
	
	// Bei linker Maustaste Tower platzieren und Cash abziehen
	if (input.button_left) {
		if (unused_tower >= 0) {
			if (player.cash >= t.price) {
				towers[unused_tower].position.x = input.x_mouse_position;
				towers[unused_tower].position.y = input.y_mouse_position;

				addToActiveTowers(towers[unused_tower]);

				player.cash -= t.price;

				printf("%s\n", t.type);

				place = (Place){ 0 };
			}
			else {
				renderText("Too expensive!", input.x_mouse_position, input.y_mouse_position, false);
			}
		}
	}

	// Name und Beschreibung des Towers anzeigen
	renderText(t.type, 650, 700, true);
	renderText(t.description, 650, 740, true);

}

// Verwaltung des Tower Placing Systems
void placeController(InputState input) {

	// Wenn ESC gedrückt: Platzieren beenden
	if (input.key_escape) {
		place = (Place){ 0 }; // (Place) wichtig für Compiler, handelt sich um Place-Struct
	}

	// Wenn 1 gedrückt: Crossbow Tower platzieren
	if (input.key_1) {
		place.crossbow = true;
	}
	if (place.crossbow) { placeTower(crossbow, input); }

	// Wenn 2 gedrückt: Cannon Tower platzieren
	if (input.key_2) {
		place.cannon = true;
	}
	if (place.cannon) { placeTower(cannon, input); }
	
	// Wenn 3 gedrückt: Minigun Tower platzieren
	if (input.key_3) {
		place.minigun = true;
	}
	if (place.minigun) { placeTower(minigun, input); }

	// Wenn 4 gedrückt: Launcher Tower platzieren
	if (input.key_4) {
		place.launcher = true;
	}
	if (place.launcher) { placeTower(launcher, input); }

	// Wenn 5 gedrückt: Saw Tower platzieren
	if (input.key_5) {
		place.saw = true;
	}
	if (place.saw) { placeTower(saw, input); }

	// Wenn 6 gedrückt: Sniper Tower platzieren
	if (input.key_6) {
		place.sniper = true;
	}
	if (place.sniper) { placeTower(sniper, input); }
}

// Tower verwalten
void towerManager(InputState input) {

	// Tower hinterlegt hinzufügen
	addTower(crossbow, 5);

	addTower(cannon, 5);

	addTower(minigun, 5);

	addTower(launcher, 5);

	addTower(saw, 5);

	addTower(sniper, 5);

	// Platzieren verwalten
	placeController(input);

	// platzierte Tower verwalten
	processActiveTowers();
}




// This File partially contains Code provided by ChatGPT