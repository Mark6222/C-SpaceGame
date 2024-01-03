#ifndef Ammo_hpp
#define Ammo_hpp

#include <SFML/Graphics.hpp>
#include <math.h>
using namespace sf;

class Ammo {

private:
	Vector2f m_Position;
	Sprite m_Sprite;
	Texture m_Texture;

public:
	Sprite getSprite();
	void spawn(int x, int y);
};

#endif /* player_hpp*/