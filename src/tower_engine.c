// tower_engine.c

// Zugriff auf Bibliothecken
#include <stdio.h>		// Standart I/O Library
#include <stdbool.h>		// Verwenden von Boolians
#include <string.h>		// Verwenden von Strings
#include <math.h>	// Verwenden von sqrt();

// Zugriff auf eigene Header
#include "../include/structs.h"
#include "../include/library.h"		
	bool running_first_frame;
#include "../include/entity_engine.h"	
	int entityCount;
#include "../include/graphics_engine.h"	
#include "../include/tower_engine.h"	

TowerData towers[MAX_TOWERS];
int towerCount = 0;

TowerData cannon = { "cannon", 1, {0, 0} };
TowerData crossbow = { "crossbow", 2, {0, 0} };

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


void positionTowers(void) {

	for (int i = 0; i <= towerCount; i++) {

		if (strcmp(towers[i].type, "") != 0) {

			renderTower(towers[i].textureIndex, towers[i].position.x, towers[i].position.y);
		}
	}
}


void addTower(TowerData t, int x_position, int y_position) {

	if (running_first_frame) {

		if (towerCount >= MAX_TOWERS) {    // Sicherheitscheck
			printf("Too much Towers!\n");
			return;
		}

		// Daten auf Listeneintrag kopieren
		strcpy_s(towers[towerCount].type, 50, t.type);
		towers[towerCount].textureIndex = t.textureIndex;
		towers[towerCount].position.x = x_position;
		towers[towerCount].position.y = y_position;

		towerCount++;	// Anzahl erhöhen
	}                                    
}


void everyDistance(void) {

	for (int Ti = 0; Ti < towerCount; Ti++) {
		for (int Ei = 0; Ei < entityCount; Ei++) {

			float distance = getDistanceAB(towers[Ti].position, entities[Ei].position);

			//printf("\nTower %d -> Entity %d = %f\n", Ti, Ei, distance);

			if (distance <= 110) {
				deleteEntity(&entities[Ei]);
			}
		}
	}
}


void towerManager(void) {

	addTower(cannon, 600, 500);

	addTower(crossbow, 1000, 100);

	positionTowers();

	everyDistance();
	

	
	

	//placeTower(crossbow, 1000, 100);
	
}