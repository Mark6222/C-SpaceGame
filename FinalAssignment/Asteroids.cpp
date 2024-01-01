#include "Asteroids.h"

// The constructor
Asteroids::Asteroids()
{
    m_Texture.loadFromFile("graphics/asteroidfire.jpg");
    m_Sprite.setTexture(m_Texture);
}

void Asteroids::shoot(float startX, float startY, float targetX, float targetY)
{
    // Keep track of the bullet

    m_InFlight = true;

    m_Position.x = startX;

    m_Position.y = startY;

    float gradient = (startX - targetX) / (startY - targetY);

    if (gradient < 0)
    {

        gradient *= -1;
    }

    float ratioXY = m_AsteroidSpeed / (1 + gradient);

    m_AsteroidDistanceY = ratioXY;
    m_AsteroidDistanceX = ratioXY * gradient;

    if (targetX < startX)
    {

        m_AsteroidDistanceX *= -1;
    }

    if (targetY < startY)
    {

        m_AsteroidDistanceY *= -1;
    }

    // Set a max range of 1000 pixels from player

    float range = 1000;

    m_MinX = startX - range;

    m_MaxX = startX + range;

    m_MinY = startY - range;

    m_MaxY = startY + range;

    m_Sprite.setPosition(m_Position);
    Vector2f direction((targetX - startX), (targetY - startY));
    float angle = ((atan2(direction.y, direction.x) * 180) / 3.141) + 90;
    m_Sprite.setRotation(angle);
}

void Asteroids::stop()
{
    m_InFlight = false;
}

bool Asteroids::isInFlight()
{
    return m_InFlight;
}

FloatRect Asteroids::getPosition()
{
    return m_Sprite.getGlobalBounds();
}

Sprite Asteroids::getSprite()
{
    return m_Sprite;
}

void Asteroids::update(float elapsedTime)
{

    // Update the bullet position variables

    m_Position.x += m_AsteroidDistanceX * elapsedTime;

    m_Position.y += m_AsteroidDistanceY * elapsedTime;

    // Move the bullet

    m_Sprite.setPosition(m_Position);

    // Has the bullet gone out of range?

    if (m_Position.x < m_MinX || m_Position.x > m_MaxX || m_Position.y < m_MinY || m_Position.y > m_MaxY)

    {
        m_InFlight = false;
    }
}