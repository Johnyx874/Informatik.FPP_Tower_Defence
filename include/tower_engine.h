// tower_engine.h
#pragma once

#include "../include/structs.h"

#define MAX_TOWERS 100
#define MAX_ACTIVE_TOWERS 100

// Externe Variablen
extern Place place;
extern TowerData towers[MAX_TOWERS];
extern int towerCount;
extern TowerData activeTowers[MAX_ACTIVE_TOWERS];
extern int activeTowerCount;
extern TowerData cannon;
extern TowerData crossbow;

// Funktionen
float getDistanceAB(Vector2 A, Vector2 B);
void addTower(TowerData t, int amount);
void addToActiveTowers(TowerData t);
void dealDamage(TowerData t, EntityData* e);
void processActiveTowers(void);
void placeTower(TowerData t, InputState input);
void placeController(InputState input);
void towerManager(InputState input);

