// level_engine.c

#include <stdio.h>

#include "../include/library.h"
    bool add_entities;
#include "../include/structs.h"

#include "../include/level_engine.h"
#include "../include/entity_engine.h"
#include "../include/graphics_engine.h"

Level level = { 0 };

int currentLevel = 0;
int currentWave = 0;
const int totalLevels = 2;

bool waveSpawned = false;
bool waitingForNextWave = false;


void levelScreen(int nextLevel) {

    char buffer[64];

    if (!passedFrames(100)) {
        snprintf(buffer, sizeof(buffer), "ROUND %d", nextLevel);
        renderText(buffer, 650, 450, true);
    }   
}


void spawnWave(int level, int wave) {
    if (level == 0) {
        if (wave == 0) {
            spawnAndCloneEntity(theChicken, 10, 200);
        }
        else if (wave == 1) {
            spawnAndCloneEntity(theHopper, 5, 100);
        }
        else if (wave == 2) {
            spawnAndCloneEntity(theChicken, 10, 100);
            spawnEntity(theBoar, 0);
        }
    }
    else if (level == 1) {
        if (wave == 0) {
            spawnAndCloneEntity(theHopper, 8, 150);
        }
        else if (wave == 1) {
            spawnEntity(theBoar, 5);
        }
    }
}


int getWaveCount(int level) {
    if (level == 0) return 3;  // Anzahl Waves in Level 0
    if (level == 1) return 2;  // Anzahl Waves in Level 1
    return 0;
}

void updateLevel(void) {
    if (currentLevel >= totalLevels) {
        if (entityCount == 0) {
            // endGame();
        }
        return;
    }

    if (!waveSpawned) {
        spawnWave(currentLevel, currentWave);  // nur einmal
        waveSpawned = true;
    }

    if (waveSpawned && entityCount == 0) {
        int delayId = 100 + currentLevel * 10 + currentWave;  // eindeutige ID für passedFramesInternal

        if (passedFramesInternal(delayId, 150)) {
            // Zeit abgelaufen -> nächste Welle
            currentWave++;
            waveSpawned = false;

            if (currentWave >= getWaveCount(currentLevel)) {
                currentLevel++;
                currentWave = 0;
                levelScreen(currentLevel);
            }
        }
    }
}


void levelManager(void) {

    updateLevel();

    updateLevel();
}
