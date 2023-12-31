#include <SFML/Graphics.hpp>
#include "player.h"
#include "Space.h"
#include "TextureHolder.h"
using namespace sf;
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
	Time gameTimeTotal;
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;
	Player player;
	IntRect arena;
	FloatRect center;

	int numShips = 0;
	Ship1 *ships = nullptr;
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
					// populated later - if a key other than return is pressed during game play
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
				if (ships != nullptr)
				{
					delete[] ships;
					ships = nullptr;
				}
				delete[] ships;
				numShips = 3;
				ships = SpawnShips(numShips, arena);
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
			if (ships != nullptr)
			{
				for (int i = 0; i < numShips; i++)
				{
					if (ships[i].isAlive())
					{
						ships[i].update(dt.asSeconds());
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
			for (int i = 0; i < numShips; i++)
			{
				window.draw(ships[i].getSprite());
			}
			window.draw(player.getSprite());
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
	if (ships != nullptr)
	{
		delete[] ships;
		ships = nullptr;
	}
	return 0;
}