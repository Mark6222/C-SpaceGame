#include <SFML/Graphics.hpp>
#include "player.h"
#include "Space.h"
#include "TextureHolder.h"
#include "Asteroids.h"
#include "Ammo.h"
#include <iostream>
#include <sstream>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;
int main()
{
	VertexArray space;
	Texture textureSpace;
	textureSpace.loadFromFile("graphics/space.png");

	enum class State{PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	State state = State::GAME_OVER;

	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y), "space", Style::Titlebar);

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
	Ship1 *ships = new Ship1[numShips]; //created an collection of ships 
	int position = 0;
	bool firstShip = false; // for the drawing so it dosent crash
	float shipTime = 0.1; // this is the time between each ship that is spawned
	

	Asteroids asteroids[100]; // this is essentially the players ammo
	int currentAsteroid = 0;

	int astroidsSpare = 2000;
	int astroidsInClip = 100;
	int clipSize = 50;
	float fireRate = 2;
	Time lastPressed; 
	// created a collection of ammo with the astroid sprite
	Ammo *ammo = new Ammo[astroidsInClip]; // this is part of the HUD displaying ammo at bottom of the screen
	int x = 0;
	for (int i = 0; i < astroidsInClip; i++) {
		ammo = SpawnAmmo(ammo, 0 + x,  resolution.y - 60, i);// fills the array with ammo positioned beside eachother
		x = x + 20;
	}

	window.setMouseCursorVisible(false);// cant see mouse
	Sprite spriteCrosshair;

	Texture textureCrosshair;
	textureCrosshair.loadFromFile("graphics/crosshair2.jpg");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);
	spriteCrosshair.setScale(0.5f, 0.5f);

	int numShipsAlive = 0;
	int score = 0;
	int hiScore = 0;

	Clock timeGone; // this is for increasing the difficulty as time goes on
	float timePoint = 10; //every 10 second the difficulty will increase
	int speed = 0; // speed of ships

	//****************************************************
	// added text on screen for time passed and kill count
	Text TimePassed;
	Text Kills;
	Font font;
	FloatRect textRect = TimePassed.getLocalBounds();
	font.loadFromFile("fonts/spaceFont.otf");
	TimePassed.setFont(font);
	TimePassed.setCharacterSize(75);
	TimePassed.setFillColor(Color::Blue);
	TimePassed.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	TimePassed.setPosition(20, 20);
	int kills = 0;
	Kills.setFont(font);
	Kills.setCharacterSize(75);
	Kills.setFillColor(Color::Blue);
	Kills.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	Kills.setPosition(20, 80);
	//********************************************************

	//*************************************************************
	// added sound effects for shooting flying ships and background
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/Shoot.wav");
	vector<Sound> shotSounds; //collection of sounds to allow fo sounds to play over each other
	int shootIndex = 0;
	SoundBuffer shipBuffer;
	shipBuffer.loadFromFile("sound/flyby.wav");
	vector<Sound> shipSounds;
	int shipIndex = 0;
	Clock shipSoundTime;
	SoundBuffer backgroundBuffer;
	backgroundBuffer.loadFromFile("sound/background.wav");
	Sound backgroundSound;
	backgroundSound.setBuffer(backgroundBuffer);
	backgroundSound.setLoop(true);
	backgroundSound.setVolume(10);
	backgroundSound.play();
	//***************************************************************

	//*********************************************************************
	// added text, sprite of player and ammo on the screen for start screen
	Sprite introSprite;
	Texture introTexture;
	introTexture.loadFromFile("graphics/asteroid.png");
	introSprite.setTexture(introTexture);
	introSprite.setPosition(resolution.x / 2.0f, resolution.y / 2.0f);
	introSprite.setScale(10, 10);
	introSprite.setOrigin(25, 25);

	Font introFont;
	introFont.loadFromFile("fonts/spaceFont.otf");
	Text intro;
	intro.setFont(introFont);
	intro.setCharacterSize(80);
	intro.setFillColor(Color::Yellow);
	intro.setString("Press Enter and then a number to start");
	FloatRect introRect = intro.getLocalBounds();
	intro.setOrigin(introRect.left + introRect.width / 2.0f, introRect.top + introRect.height / 2.0f);
	intro.setPosition(resolution.x / 2.0f, resolution.y / 2.0f - 100);
	Text GameTitle;
	GameTitle.setFont(introFont);
	GameTitle.setCharacterSize(150);
	GameTitle.setFillColor(Color::Blue);
	GameTitle.setString("Asteroids");
	FloatRect TitleoRect = GameTitle.getLocalBounds();
	GameTitle.setOrigin(TitleoRect.left + TitleoRect.width / 2.0f, TitleoRect.top + TitleoRect.height / 2.0f);
	GameTitle.setPosition(resolution.x / 2.0f, resolution.y / 2.0f - 300);
	//created new collection of astroids which are placed randomly on screen
	Ammo* introAmmo = new Ammo[astroidsInClip];
	for (int i = 0; i < astroidsInClip; i++) {
		srand((int)time(0) * i);
		float randomLocationX = (rand() % (int)resolution.x);
		float randomLocationY = (rand() % (int)resolution.y);
		introAmmo = SpawnAmmo(introAmmo, randomLocationX, randomLocationY, i);
	}
	//***************************************************************


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
					// added sound for shooting every time the player shoots
					Sound shoot;
					shoot.setBuffer(shootBuffer);
					shotSounds.push_back(shoot);
					Sound& sound = shotSounds[shootIndex];
					sound.play(); // plays that sound from the collection
					shootIndex++;
					// calculated the angle for the offset of 2 extra bullets at either side of the center bullet using pythagoras therom 
					float radius = sqrt(pow(mouseWorldPosition.x - player.getCenter().x, 2) + pow(mouseWorldPosition.y - 100 - player.getCenter().y, 2));
					float angleCenter = atan2(mouseWorldPosition.y - 100 - player.getCenter().y, mouseWorldPosition.x - player.getCenter().x);
					float angleleft = 10 * (3.141 / 180);
					float angleRight = -10 * (3.141 / 180);
					// getting the x and y cord of left and right bullet
					float targetLeftX = mouseWorldPosition.x + radius * cos(angleCenter + angleleft);
					float targetLeftY = mouseWorldPosition.y - 100 + radius * sin(angleCenter + angleleft);
					float targetRightX = mouseWorldPosition.x + radius * cos(angleCenter + angleRight);
					float targetRightY = mouseWorldPosition.y - 100 + radius * sin(angleCenter + angleRight);
					asteroids[currentAsteroid].shoot(player.getCenter().x, player.getCenter().y,mouseWorldPosition.x, mouseWorldPosition.y - 100);
					currentAsteroid++;
					if (currentAsteroid > 99) {
						currentAsteroid = 0;
					}
					//shooting offset bullets
					asteroids[currentAsteroid].shoot(player.getCenter().x, player.getCenter().y, targetLeftX, targetLeftY);
					currentAsteroid++;
					asteroids[currentAsteroid].shoot(player.getCenter().x, player.getCenter().y, targetRightX, targetRightY);
					currentAsteroid++;
					lastPressed = gameTimeTotal;
					astroidsInClip--;
				}
			}
		} // end of WASD
		if (state == State::LEVELING_UP)
		{
			state = State::PLAYING;
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
			// setting the player view to the middle of the screen
			mainView.setCenter(resolution.x / 2.0, resolution.y / 2.0);
			int maxX = arena.width;
			int minX = arena.left;
			int maxY = arena.height;
			//position is the index of each ship, when it spawns 99 it returns to 0
			if (position == 99) {
				position = 0;
			}
			//adding difficulty every 10 seconds
			if (timeGone.getElapsedTime().asSeconds() > timePoint) {
				timePoint = timePoint + 10;
				if (shipTime > 0.04) {
					shipTime = shipTime - 0.01;
					speed = speed + 100;
				}
			}
			// playing the ships sound effect every 0.5 sconds
			if (shipSoundTime.getElapsedTime().asSeconds() >= 0.5)
			{
				Sound shipSound;
				shipSound.setBuffer(shipBuffer);
				shipSounds.push_back(shipSound);
				Sound& soundFlying = shipSounds[shipIndex];
				soundFlying.setVolume(5);
				soundFlying.play();
				shipIndex++;
				shipSoundTime.restart();
			}
			// spawning ships at the top of the screen in a random x cordinate and adding it to the ships collection
			if (spaceTime.getElapsedTime().asSeconds() >= shipTime)
			{
				srand((int)time(0)* position);
				float randomLocationX = (rand() % (maxX - minX));
				firstShip = true;
				ships = SpawnShips(ships, numShips, arena, position, randomLocationX, speed);
				position++;
				spaceTime.restart();
				numShipsAlive++;
			}
			//updates the movements of each ship
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
			// setting the updated values for time and kills
			std::stringstream t;
			std::stringstream k;
			t << "Time: " << (int)timeGone.getElapsedTime().asSeconds();
			TimePassed.setString(t.str());
			k << "Kills: " << kills;
			Kills.setString(k.str());
			// Collision detection
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numShips; j++)
				{
					if (asteroids[i].isInFlight() &&
						ships[j].isAlive())
					{
						if (asteroids[i].getPosition().intersects //if the astroids hit the ship
						(ships[j].getPosition()))
						{
							asteroids[i].stop();
							if (ships[j].hit()) { // if the astroids hit the ship then add to the kill count
								kills++;
								numShipsAlive--;
								if (numShipsAlive == 0) {
									state = State::LEVELING_UP;
								}
							}

						}
					}

				}
			}
			//collision detection for ships hitting player
			for (int i = 0; i < numShips; i++)
			{
				if (player.getPosition().intersects(ships[i].getPosition()) && ships[i].isAlive()) // if a ship hit a player
				{
					if (player.hit(gameTimeTotal))
					{
					}
					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
						//delete[] ships;
						//delete[] ammo;
						player.resetPlayerStats();
						timeGone.restart();
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
			window.draw(space, &textureSpace); //drawing the space image tiles
			if (firstShip)
			{
				for (int i = 0; i < numShips; i++)
				{
					if (ships != nullptr && ships[i].isAlive())
					{
						window.draw(ships[i].getSprite()); // draws ships as there spawned
					}
				}
			}
			// Draw the bullets
			for (int i = 0; i < 100; i++) {
				if (asteroids[i].isInFlight()) {
					window.draw(asteroids[i].getSprite()); // draws aseroids as there shot
				}
			}
			window.draw(player.getSprite());
			// drawing the time and kills
			window.draw(TimePassed);
			window.draw(Kills);
			for (int i = 0; i < astroidsInClip; i++) {
				window.draw(ammo[i].getSprite()); //draws the ammo bar on the bottom of the screen
			}
		}
		if (state == State::LEVELING_UP)
		{
		}
		if (state == State::PAUSED)
		{
		}
		if (state == State::GAME_OVER)
		{
			window.clear();
			window.setView(mainView);
			window.draw(space, &textureSpace);
			for (int i = 0; i < astroidsInClip; i++) {
				window.draw(introAmmo[i].getSprite()); //draws random astroids on the screen
			}
			window.draw(introSprite); 
			window.draw(intro);
			window.draw(GameTitle);
			window.display();
		}
		window.display();
	} // end update scene
	delete[] ships;
	delete[] ammo;
	return 0;
}