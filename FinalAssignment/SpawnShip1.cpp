#include "Space.h"
#include "Ship1.h"

Ship1* SpawnShips(int numShips, IntRect arena)
{
    
    Ship1* Ship1s = new Ship1[numShips];

    int maxY = arena.height - 20;
    int minY = arena.top + 20;
    int maxX = arena.width - 20;
    int minX = arena.left + 20;

    for (int i = 0; i < numShips; i++)
    {
        float randomLocation = (rand() % maxX - minX);
        Ship1s[i].spawn(randomLocation, minY, i);
    }
    
    return Ship1s;
}