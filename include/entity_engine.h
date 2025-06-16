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
extern EntityData theSecond;

// Funktionen
void moveAlongPath(EntityData* e);
void moveEntity(EntityData e);
void spawnEntity(EntityData e, int offset);
void spawnAndCloneEntity(EntityData e, int amount, int spacing);
void deleteEntity(EntityData* e);
void entityManager(InputState input);
