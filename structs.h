#pragma once

// Struktur für einen 2D-Vektor
typedef struct {
    int x, y;
} Vector2;

// Struktur für einen Pfad mit mehreren Punkten
typedef struct {
    Vector2* points;
    int count;
} Path;

// Struktur für ein einzelnes Entity
typedef struct {

    char type[50];            // Type des Entitys
    int textureIndex;         // Von dem Entity verwendete Textur
    Vector2 position;         // Aktuelle Position
    int currentTargetIndex;   // Index des nächsten Wegpunkts
    int speed;                // Bewegungsgeschwindigkeit (Pixel pro Frame)
    int health;

} EntityData;