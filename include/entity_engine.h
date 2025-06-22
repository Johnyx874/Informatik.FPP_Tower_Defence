// entity_engine.h
#pragma once

#include "structs.h"

#define MAX_ENTITIES 100

// Externe Variablen
extern Vector2 pathPoints[];
extern Path path;
extern EntityData entities[MAX_ENTITIES];
extern int entityCount;

extern EntityData theChicken;
extern EntityData theBoar;
extern EntityData theHopper;
extern EntityData theBallon;
extern EntityData theCar;
extern EntityData theWarthog;
extern EntityData theTitan;

// Funktionen
void spawnEntity(EntityData e, int offset);
void spawnAndCloneEntity(EntityData e, int amount, int spacing);
//void deleteEntity(EntityData* e);
void killEntity(EntityData* e, int id);
void moveAlongPath(EntityData* e);
void moveEntities(void);
void entityManager(InputState input);
