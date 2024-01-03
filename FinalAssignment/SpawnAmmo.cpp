#include "Space.h"
#include "Ammo.h"

Ammo* SpawnAmmo(Ammo* ammo,int x, int y, int index)
{
    ammo[index].spawn(x, y); // spawns ammo for the HUD
    return ammo;
}