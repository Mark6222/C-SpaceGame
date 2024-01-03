#include "Ammo.h"

void Ammo::spawn(int x, int y) {
	m_Texture.loadFromFile("graphics/asteroidfire.jpg");
	m_Sprite = Sprite(m_Texture);
	m_Position.x = x;
	m_Position.y = y;

	m_Sprite.setOrigin(25, 25);
	m_Sprite.setPosition(m_Position);
}

Sprite Ammo::getSprite() {
	return m_Sprite;
}