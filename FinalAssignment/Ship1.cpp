#include "Ship1.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>
#include <math.h>
void Ship1::spawn(float startX, float startY, int seed)
{
	m_Texture.loadFromFile("graphics/Ship1.png");
    m_Sprite = Sprite(m_Texture);
	srand((int)time(0) * seed);
	m_Position.x = startX;
	m_Position.y = startY;

	m_Sprite.setOrigin(25, 25);
	m_Sprite.setPosition(m_Position);

	SHIP1_HEALTH = 1;
}

bool Ship1::hit()
{
	SHIP1_HEALTH--;

	if (SHIP1_HEALTH < 0) {
		m_Alive = false;
		m_Texture.loadFromFile("graphics/blood.png");
        m_Sprite.setTexture(m_Texture);

		return true;
	}

	return false;
}

bool Ship1::isAlive()
{
	return m_Alive;
}

FloatRect Ship1::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Ship1::getSprite()
{
	return m_Sprite;
}

void Ship1::update(float elapsedTime)
{
	m_Position.y += SPEED * elapsedTime;
	m_Sprite.setPosition(m_Position);

	m_Sprite.setRotation(180.0f);

}

bool Ship1::isOutOfBounds(int maxY) {
	FloatRect position = m_Sprite.getGlobalBounds();
	if (maxY < m_Position.y) {
		return true;
	}
	else {
		return false;
	}
}