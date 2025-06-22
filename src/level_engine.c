// level_engine.c

#include <stdio.h>

#include "../include/library.h"
#include "../include/structs.h"

#include "../include/level_engine.h"
#include "../include/entity_engine.h"
#include "../include/graphics_engine.h"

// Initialisieren von Variablen
int currentLevel = 0;
int currentWave = 0;
const int totalLevels = 7;

bool waveSpawned = false;
bool waitingForNextWave = false;

// Preview Funktion um aktuelle Runde anzuzeigen, aktuell nicht in Verwendung
void levelScreen(int nextLevel) {

    char buffer[64];

    snprintf(buffer, sizeof(buffer), "ROUND %d", nextLevel);
    renderText(buffer, 650, 450, true);
}

// Funktion um Welle an Gegner zu spawnen, basierend auf aktuellem Level und Welle
void spawnWave(int level, int wave) {
    if (level == 0) {
        if (wave == 0) {
            spawnAndCloneEntity(theChicken, 10, 200);
        }
    }
    else if (level == 1) {
        if (wave == 0) {
            spawnAndCloneEntity(theHopper, 5, 100);
        }
        else if (wave == 1) {
            spawnAndCloneEntity(theChicken, 10, 100);
            spawnEntity(theBoar, 0);
        }
    }
    else if (level == 2) {
        if (wave == 0) {
            spawnAndCloneEntity(theChicken, 15, 0);
        }
    }
    else if (level == 3) {
        if (wave == 0) {
            spawnAndCloneEntity(theChicken, 20, 100);
            spawnAndCloneEntity(theBallon, 3, 100);
        }
        if (wave == 1) {
            spawnAndCloneEntity(theChicken, 20, 50);
            spawnAndCloneEntity(theHopper, 7, 200);
        }
    }
    else if (level == 4) {
        if (wave == 0) {
            spawnAndCloneEntity(theWarthog, 5, 100);
        }
    }
    else if (level == 5) {
        if (wave == 0) {
            spawnAndCloneEntity(theCar, 3, 200);
        }
        if (wave == 1) {
            spawnAndCloneEntity(theChicken, 30, 50);
        }
    }
    else if (level == 6) {
        if (wave == 0) {
            spawnAndCloneEntity(theWarthog, 12, 200);
            spawnAndCloneEntity(theBallon, 40, 50);
        }
        if (wave == 1) {
            spawnAndCloneEntity(theTitan, 3, 200);
        }
    }
}

// Funktion um Wellen anzahlen für Level abzurufen
int getWaveCount(int level) {
    if (level == 0) return 1;  // Anzahl Waves in Level 0
    if (level == 1) return 2;  // Anzahl Waves in Level 1
    if (level == 2) return 1;  // Anzahl Waves in Level 2
    if (level == 3) return 2;  // Anzahl Waves in Level 3
    if (level == 4) return 1;  // Anzahl Waves in Level 4
    if (level == 5) return 2;  // Anzahl Waves in Level 5
    if (level == 6) return 2;  // Anzahl Waves in Level 6
    return 0;
}

// Funktion um Level zu aktualisieren
void updateLevel(void) {
    // Überprüfung ob alle Level absolviert wurden
    if (currentLevel >= totalLevels) {
        if (entityCount == 0) {
            // endGame();
        }
        return;
    }

    // Welle spawnen
    if (!waveSpawned) {
        spawnWave(currentLevel, currentWave);  // nur einmal
        waveSpawned = true;
    }

    // Ende der Welle abwarten
    if (waveSpawned && entityCount == 0) {
        int delayId = 100 + currentLevel * 10 + currentWave;  // eindeutige ID für passedFramesInternal

        // Bei Ende kurzes Päuschen einlegen
        if (passedFramesInternal(delayId, 150)) {
            // nächste Welle vorbereiten
            currentWave++;
            waveSpawned = false;

            // eventuell nächstes Level aufrufen
            if (currentWave >= getWaveCount(currentLevel)) {
                currentLevel++;
                currentWave = 0;
                //levelScreen(currentLevel);
            }
        }
        else {
            //levelScreen(currentWave + 2);
        }
    }
}

// Funktion um Level zu Verwalten
void levelManager(void) {

    updateLevel();

}




// This File mostly contains Code provided by ChatGPT