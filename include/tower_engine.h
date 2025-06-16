// tower_engine.h
#pragma once

#include "../include/structs.h"

#define MAX_TOWERS 100

// Externe Variablen
extern TowerData towers[MAX_TOWERS];
extern towerCount;
extern TowerData cannon;
extern TowerData crossbow;

// Funktionen
float getDistanceAB(Vector2 A, Vector2 B);
void positionTowers(InputState input);
void addTower(TowerData t, int x_position, int y_position);
void everyDistance(void);
void towerManager(InputState input);

