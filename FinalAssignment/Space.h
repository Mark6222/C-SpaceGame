#ifndef Space_H
#define Space_H
#include "Ship1.h"
using namespace sf;
int createSpace(VertexArray& rVA, IntRect arena);
Ship1* SpawnShips(Ship1* ships, int numShips, IntRect arena, int position, float randomLocationX);
#endif