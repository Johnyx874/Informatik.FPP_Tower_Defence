// player_engine.c

#include <stdio.h>
#include <stdbool.h>

#include "../include/structs.h"

#include "../include/entity_engine.h"
#include "../include/graphics_engine.h"

// Initialisierung des Players
Player player = { 
	"Player 1",		// Name
	150,				// Cash
	10				// Health
};			



// wenn Entity die gesamte Map durchlaufen hat, Damage von Entity auf Player übertragen
void checkDamage(void) {

	for (int i = 0; i <= entityCount; i++) {

		if (entities[i].position.x >= 1300) {

			player.health -= 1;

			printf("Player HP: %d\n", player.health);

			entities[i] = (EntityData){ 0 };
		}
	}
}

// Bonus von Entity auf Player übertragen
void giveBonus(int bonus) {

	player.cash += bonus;

	printf("Player Cash: %d\n", player.cash);
}

// Verwaltung von player
void playerManager(void) {
	
	checkDamage();

	char buffer[64];

	snprintf(buffer, sizeof(buffer), "HP: %d", player.health);
	renderText(buffer, 70, 30, false);

	snprintf(buffer, sizeof(buffer), "Cash: %d", player.cash);
	renderText(buffer, 1000, 30, false);

}