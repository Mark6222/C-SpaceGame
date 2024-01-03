#include "Space.h"
#include "Ship1.h"

Ship1* SpawnShips(Ship1* ships, int numShips, IntRect arena, int position, float randomLocationX, int speed)
{

    int maxY = arena.height - 20;
    int minY = arena.top + 20;
    int maxX = arena.width - 20;
    int minX = arena.left + 20;
    ships[position].spawn(randomLocationX, minY, 1); //spawns ship at random x location and top of the screen
    ships[position].increaseSpeed(speed); //adds to the speed of the ship
    return ships; //returns collection of ships
}