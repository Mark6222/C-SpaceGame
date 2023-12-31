#ifndef Ship1_hpp
#define Ship1_hpp
#include <SFML/Graphics.hpp>

using namespace sf;

class Ship1
{
private:
	float SPEED = 500;
	float SHIP1_HEALTH;

	Vector2f m_Position;
	Sprite m_Sprite;
    Texture m_Texture;

	bool m_Alive = true;

public:
    bool hit();
    bool isAlive();
    void spawn(float startX, float startY, int seed);
    FloatRect getPosition();
    Sprite getSprite();
    void update(float elapsedTime);
};


#endif