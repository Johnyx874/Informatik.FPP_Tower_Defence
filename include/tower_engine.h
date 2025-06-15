// tower_engine.h
#pragma once

#include "../include/structs.h"

#define MAX_TOWERS 100

// Externe Variablen
extern TowerData towers[MAX_TOWERS];
extern TowerData cannon;
extern TowerData crossbow;

// Funktionen
float getDistanceAB(Vector2 A, Vector2 B);
void positionTower(TowerData* t, int x, int y);
void placeTower(TowerData t, int x_position, int y_position);
void towerManager(void);

