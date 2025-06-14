// tower_engine.c

#include <stdio.h>		// Standart I/O Library
#include <stdbool.h>		// Verwenden von Boolians
#include <math.h>	// Verwenden von sqrt();

#include "../include/structs.h"		// Zugriff auf Header-Datei
#include "../include/library.h"		// Zugriff auf Header-Datei
	bool running_first_frame;

#include "../include/entity_engine.h"


float getDistanceAB(Vector2 A, Vector2 B) {

	Vector2 C;

	C.x = A.x;
	C.y = B.y;

	int dis_ac = A.y - C.y;
	int dis_bc = B.x - C.x;

	if (dis_ac < 0) { dis_ac *= -1; }
	if (dis_bc < 0) { dis_bc *= -1; }

	float dis_ab = sqrt( (dis_ac * dis_ac) + (dis_bc * dis_bc) );

	printf("%f", dis_ab);

	return dis_ab;
}