#pragma once
#ifndef Asteroids_hpp
#define Asteroids_hpp

#include <SFML/Graphics.hpp>

using namespace sf;

class Asteroids
{
private:
    // Where is the bullet?
    Vector2f m_Position;

    // What each bullet looks like
    Sprite m_Sprite;
    Texture m_Texture;

    // Is this bullet currently whizzing through the air?
    bool m_InFlight = false;

    // How fast does a bullet travel?
    float m_AsteroidSpeed = 1000;

    // What fraction of 1 pixel does the bullet travel horizontally and vertically each frame?
    // These values will be derived from m_BulletSpeed
    float m_AsteroidDistanceX;
    float m_AsteroidDistanceY;

    // Where is this bullet headed to?
    float m_XTarget;
    float m_YTarget;

    // Some boundaries so the bullet doesn't fly forever
    float m_MaxX;
    float m_MinX;
    float m_MaxY;
    float m_MinY;

public:
    // The constructor
    Asteroids();

    // Stop the bullet
    void stop();

    // Returns the value of m_InFlight
    bool isInFlight();

    // Launch a new bullet
    void shoot(float startX, float startY,
        float xTarget, float yTarget);

    // Tell the calling code where the bullet is in the world
    FloatRect getPosition();

    Sprite getSprite();

    // Update the bullet each frame
    void update(float elapsedTime);

};


#endif