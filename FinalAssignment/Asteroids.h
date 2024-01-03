#pragma once
#ifndef Asteroids_hpp
#define Asteroids_hpp

#include <SFML/Graphics.hpp>

using namespace sf;

class Asteroids
{
private:
    Vector2f m_Position;

    Sprite m_Sprite;
    Texture m_Texture;

    bool m_InFlight = false;

    // How fast the asteroid travels
    float m_AsteroidSpeed = 1000;

    // What fraction of 1 pixel does the bullet travel horizontally and vertically each frame?
    float m_AsteroidDistanceX;
    float m_AsteroidDistanceY;

    float m_XTarget;
    float m_YTarget;

    float m_MaxX;
    float m_MinX;
    float m_MaxY;
    float m_MinY;

public:
    Asteroids();
    void stop();
    bool isInFlight();
    void shoot(float startX, float startY,
    float xTarget, float yTarget);
    FloatRect getPosition();
    Sprite getSprite();
    void update(float elapsedTime);
};


#endif