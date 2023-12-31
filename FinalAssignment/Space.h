#ifndef Space_H
#define Space_H
#include "Ship1.h"
using namespace sf;
int createSpace(VertexArray& rVA, IntRect arena);
Ship1* SpawnShips(int numShips, IntRect arena);
#endif