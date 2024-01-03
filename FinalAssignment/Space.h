#ifndef Space_H
#define Space_H
#include "Ship1.h"
#include "Ammo.h"
using namespace sf;
int createSpace(VertexArray& rVA, IntRect arena);
Ship1* SpawnShips(Ship1* ships, int numShips, IntRect arena, int position, float randomLocationX, int speed); // for spawning the ships
Ammo* SpawnAmmo(Ammo* ammo, int x, int y, int index); // for spawning each ammo on the players HUD
#endif