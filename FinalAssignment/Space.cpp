#include <SFML/Graphics.hpp>
#include "player.h"
#include "Space.h"
#include "TextureHolder.h"
#include "Asteroids.h"
#include <iostream>
using namespace sf;
using namespace std;
int main()
{
	VertexArray space;
	Texture textureSpace;
	textureSpace.loadFromFile("graphics/space.png");

	enum class State
	{
		PAUSED,
		LEVELING_UP,
		GAME_OVER,
		PLAYING
	};

	State state = State::GAME_OVER;

	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "space", Style::Fullscreen);

	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	Clock clock;
	Clock spaceTime;
	Time gameTimeTotal;
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;
	Player player;
	IntRect arena;
	FloatRect center;

	int numShips = 100;
	Ship1 *ships = new Ship1[numShips];
	int position = 0;
	bool firstShip = false;
	float shipTime = 0.1;
	

	Asteroids asteroids[100];
	int currentAsteroid = 0;

	int astroidsSpare = 2000;
	int astroidsInClip = 20;
	int clipSize = 6;
	float fireRate = 4;
	Time lastPressed; 

	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;

	Texture textureCrosshair;
	textureCrosshair.loadFromFile("graphics/crosshair2.jpg");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);
	spriteCrosshair.setScale(0.5f, 0.5f);
	int numShipsAlive = 0;
	int score = 0;
	int hiScore = 0;
	while (window.isOpen())
	{
		/*
		************************
		Handle input
		************************
		*/
		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				// Pause game while playing
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					clock.restart();
				}
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}
				if (state == State::PLAYING)
				{
					// Reloading
					if (event.key.code == Keyboard::R) {

						if (astroidsSpare >= clipSize) {

							// Plenty of bullets. Reload.

							astroidsInClip = clipSize;

							astroidsSpare -= clipSize;

						}
						else if (astroidsSpare > 0) {
							// Only few bullets left

							astroidsInClip = astroidsSpare;

							astroidsSpare = 0;

						}

						else {

							// More here later - reload failed sound

						}

					}//End if (event.key.code == Keyboard :: R)
				}
			}
		} // end event polling
		// handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		if (state == State::PLAYING)
		{
			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}
			if (Keyboard::isKeyPressed(Keyboard::A))
				player.moveLeft();
			else
			{
				player.stopLeft();
			}
			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
			// Fire a bullet
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && astroidsInClip > 0) {
					float radius = sqrt(pow(mouseWorldPosition.x - player.getCenter().x, 2) + pow(mouseWorldPosition.y - player.getCenter().y, 2));
					float angleCenter = atan2(mouseWorldPosition.y - player.getCenter().y, mouseWorldPosition.x - player.getCenter().x);
					float angleleft = 10 * (3.141 / 180);
					float angleRight = -10 * (3.141 / 180);
					float targetLeftX = mouseWorldPosition.x + radius * cos(angleCenter + angleleft);
					float targetLeftY = mouseWorldPosition.y + radius * sin(angleCenter + angleleft);
					float targetRightX = mouseWorldPosition.x + radius * cos(angleCenter + angleRight);
					float targetRightY = mouseWorldPosition.y + radius * sin(angleCenter + angleRight);
					asteroids[currentAsteroid].shoot(player.getCenter().x, player.getCenter().y,mouseWorldPosition.x, mouseWorldPosition.y);
					currentAsteroid++;
					if (currentAsteroid > 99) {
						currentAsteroid = 0;
					}
					asteroids[currentAsteroid].shoot(player.getCenter().x, player.getCenter().y, targetLeftX, targetLeftY);
					currentAsteroid++;
					asteroids[currentAsteroid].shoot(player.getCenter().x, player.getCenter().y, targetRightX, targetRightY);
					currentAsteroid++;
					lastPressed = gameTimeTotal;
					astroidsInClip--;
				}
			}// End fire a bullet
		} // end of WASD
		if (state == State::LEVELING_UP)
		{
			if (event.key.code == Keyboard::Num1)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num2)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num3)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num4)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num5)
			{
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num6)
			{
				state = State::PLAYING;
			}
			if (state == State::PLAYING)
			{
				// Prepare the level - we will update this later
				arena.width = resolution.x;
				arena.height = resolution.y;
				arena.left = 0;
				arena.top = 0;

				int tileSize = createSpace(space, arena);
				player.spawn(arena, resolution, tileSize);
				clock.restart();
			}
		} // end of leveling up
		/*
		*************************
		UPDATE THE FRAME
		*************************
		*/
		if (state == State::PLAYING)
		{
			Time dt = clock.restart();
			gameTimeTotal += dt;
			float dtAsSeconds = dt.asSeconds();
			float speed = 0.0009;

			mouseScreenPosition = Mouse::getPosition(window);
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);
			player.update(speed, Mouse::getPosition());
			Vector2f playerPosition(player.getCenter());
			center.width = resolution.x;
			center.height = resolution.y;
			float centerX = center.left + center.width / 2.0f;
			float centerY = center.top + center.height / 2.0f;
			mainView.setCenter(centerX, centerY);
			int maxX = arena.width;
			int minX = arena.left;
			int maxY = arena.height;
			if (position == 99) {
				position = 0;
			}
			if (spaceTime.getElapsedTime().asSeconds() >= shipTime)
			{
				srand((int)time(0)* position);
				float randomLocationX = (rand() % (maxX - minX));
				firstShip = true;
				ships = SpawnShips(ships, numShips, arena, position, randomLocationX);
				position++;
				spaceTime.restart();
				numShipsAlive++;
			}
			for (int i = 0; i < numShips; i++)
			{
				if (ships != nullptr && ships[i].isAlive())
				{
					ships[i].update(dt.asSeconds());
				}
			}
			// Update any bullets that are in-flight
			for (int i = 0; i < 100; i++) {
				if (asteroids[i].isInFlight()) {
					asteroids[i].update(dtAsSeconds);
				}

			}
			spriteCrosshair.setPosition(mouseWorldPosition.x, mouseWorldPosition.y);
			// Collision detection
			// Have any zombies been shot?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numShips; j++)
				{
					if (asteroids[i].isInFlight() &&
						ships[j].isAlive())
					{
						if (asteroids[i].getPosition().intersects
						(ships[j].getPosition()))
						{
							// Stop the bullet
							asteroids[i].stop();

							// Register the hit and see if it was a kill
							if (ships[j].hit()) {
								// Not just a hit but a kill too
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}

								numShipsAlive--;

								// When all the zombies are dead (again)
								if (numShipsAlive == 0) {
									state = State::LEVELING_UP;
								}
							}

						}
					}

				}
			}// End zombie being shot


			// Have any zombies touched the player?
			for (int i = 0; i < numShips; i++)
			{
				if (player.getPosition().intersects(ships[i].getPosition()) && ships[i].isAlive())
				{
					if (player.hit(gameTimeTotal))
					{
						// More here later
					}
					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;

					}
				}
			}
		}
		/*
		********************************
		Draw the scene
		********************************
		*/
		if (state == State::PLAYING)
		{
			window.clear();
			window.setView(mainView);
			window.draw(space, &textureSpace);
			if (firstShip)
			{
				for (int i = 0; i < numShips; i++)
				{
					if (ships != nullptr && ships[i].isAlive())
					{
						window.draw(ships[i].getSprite());
					}
				}
			}
			// Draw the bullets
			for (int i = 0; i < 100; i++) {
				if (asteroids[i].isInFlight()) {
					window.draw(asteroids[i].getSprite());
				}
			}
			window.draw(player.getSprite());
			window.draw(spriteCrosshair);
		}
		if (state == State::LEVELING_UP)
		{
		}
		if (state == State::PAUSED)
		{
		}
		if (state == State::GAME_OVER)
		{
		}
		window.display();
	} // end update scene
	delete[] ships;
	return 0;
}