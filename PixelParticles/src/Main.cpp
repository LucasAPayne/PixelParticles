#include <SFML/Graphics.hpp>
#include <vector>

enum class ParticleID
{
	EMPTY,
	SAND,
	WATER
};

struct Particle
{
	ParticleID ID;
	sf::RectangleShape Rect;

	Particle(ParticleID id = ParticleID::EMPTY)
		: ID(id)
	{
		Rect = sf::RectangleShape({ 10.f, 10.f });
		
		switch (ID)
		{
			case ParticleID::EMPTY:
				Rect.setFillColor(sf::Color(0, 0, 0));
				break;

			case ParticleID::SAND:
				Rect.setFillColor(sf::Color({ 194, 194, 128 }));
				break;

			case ParticleID::WATER:
				Rect.setFillColor(sf::Color({ 0, 0, 255 }));

			default:
				break;
		}
	}
};

// NOTE: This method works for simple simulation, but more complex physics with velocity and gravity will require a more complex method
// Swap the color and ID of two particles to simulate their movement
void swapParticles(Particle& p1, Particle& p2)
{
	Particle temp = p2;

	p2.ID = p1.ID;
	p2.Rect.setFillColor(p1.Rect.getFillColor());
	p1.ID = temp.ID;
	p1.Rect.setFillColor(temp.Rect.getFillColor());
}

int main()
{
	// Constants
	static const uint32_t SCREEN_WIDTH = 800;
	static const uint32_t SCREEN_HEIGHT = 600;
	static const uint32_t CELL_SIZE = 10;

	// Change value every frame, and use this to allow variation in sand simulation and allow water to flow to the right
	bool oddFrame = false;

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pixel Particles");
	window.setFramerateLimit(30);

	// Vector of cells. Each cell will contain a particle, either empty or of a certain material. Initialize all cells to empty
	std::vector<std::vector<Particle>> particles((int)(SCREEN_WIDTH / CELL_SIZE), std::vector<Particle>((int)(SCREEN_HEIGHT / CELL_SIZE)));

	// NOTE: In the future, the user will have the ability to place particles, but for now, simulations will be pre-set
	// Prompt the user to choose a driver program for either the sand or water simulation
	{
		sf::Font font;
		if (!font.loadFromFile("cthulhumbus.ttf"))
			printf("Font load error");

		sf::Text prompt;
		prompt.setFont(font);
		prompt.setString("Choose Simulation:\n\t\t"
			             "1 : Sand\n\t\t"
			             "2: Water");
		prompt.setCharacterSize(24);
		prompt.setFillColor(sf::Color::White);
		prompt.setPosition({ SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2.5});

		// The material to be chosen by the user
		ParticleID mat = ParticleID::EMPTY;

		bool intro = true;
		while (intro)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
					intro = false;
				}

				else if (event.type == sf::Event::KeyPressed)
				{
					if (event.key.code == sf::Keyboard::Num1)
					{
						mat = ParticleID::SAND;
						intro = false;
					}
					else if (event.key.code == sf::Keyboard::Num2)
					{
						intro = false;
						mat = ParticleID::WATER;
					}
				}
			}

			window.clear();
			window.draw(prompt);
			window.display();
		}

		// Initialize particle positions and IDs
		for (int y = 0; y < SCREEN_HEIGHT / CELL_SIZE; y++)
		{
			for (int x = 0; x < SCREEN_WIDTH / CELL_SIZE; x++)
			{
				int middleCol = SCREEN_WIDTH / CELL_SIZE / 2;
				// Add nonempty particles to the center column of the window
				if (mat == ParticleID::SAND)
				{
					if (x == middleCol)
						particles[x][y] = Particle(ParticleID::SAND);
				}
				else if (mat == ParticleID::WATER)
				{
					if (middleCol - x >= -1 && middleCol - x < 1)
						particles[x][y] = Particle(ParticleID::WATER);
				}

				particles[x][y].Rect.setPosition((float)x * CELL_SIZE, (float)y * CELL_SIZE);
			}
		}
	}

	while (window.isOpen())
	{
		oddFrame = !oddFrame;

		// HANDLE INPUT
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// UPDATE
		for (int y = SCREEN_HEIGHT / CELL_SIZE - 1; y >= 0; y--)
		{
			for (int x = 0; x < SCREEN_WIDTH / CELL_SIZE; x++)
			{
				// Get the ID of the current particle to apply the appropriate rules
				switch(particles[x][y].ID)
				{
					case ParticleID::SAND:
					{
						if (x > 0 && x < SCREEN_WIDTH / CELL_SIZE - 1 && y >= 0 && y < SCREEN_HEIGHT / CELL_SIZE - 1)
						{
							// Down empty
							if (particles[x][y + 1].ID == ParticleID::EMPTY)
								swapParticles(particles[x][y], particles[x][y + 1]);

							// Down left empty
							else if (particles[x - 1][y + 1].ID == ParticleID::EMPTY && oddFrame)
								swapParticles(particles[x][y], particles[x - 1][y + 1]);

							// Down right empty
							else if (particles[x + 1][y + 1].ID == ParticleID::EMPTY && !oddFrame)
								swapParticles(particles[x][y], particles[x + 1][y + 1]);
						}
					} break;

					// Water behaves similarly to sand, except it also checks as far left and right as possible (to fill its container)
					case ParticleID::WATER:
					{
						if (x > 0 && x < SCREEN_WIDTH / CELL_SIZE - 1 && y >= 0 && y < SCREEN_HEIGHT / CELL_SIZE - 1)
						{
							// Down empty
							if (particles[x][y + 1].ID == ParticleID::EMPTY)
								swapParticles(particles[x][y], particles[x][y + 1]);

							// Down left empty
							else if (particles[x - 1][y + 1].ID == ParticleID::EMPTY && oddFrame)
								swapParticles(particles[x][y], particles[x - 1][y + 1]);

							// Down right empty
							else if (particles[x + 1][y + 1].ID == ParticleID::EMPTY && !oddFrame)
								swapParticles(particles[x][y], particles[x + 1][y + 1]);

							// Left empty
							else if (particles[x - 1][y].ID == ParticleID::EMPTY && oddFrame)
								swapParticles(particles[x][y], particles[x - 1][y]);

							// Right empty
							else if (particles[x + 1][y].ID == ParticleID::EMPTY && !oddFrame)
								swapParticles(particles[x][y], particles[x + 1][y]);
						}
					} break;

					// Do nothing if no particle exists at the location
					case ParticleID::EMPTY:
						break;

					default:
						break;
				}
			}
		}

		// DISPLAY
		window.clear();

		for (int x = 0; x < SCREEN_WIDTH / CELL_SIZE; x++)
		{
			for (int y = 0; y < SCREEN_HEIGHT / CELL_SIZE; y++)
				window.draw(particles[x][y].Rect);
		}

		window.display();
	}

	return 0;
}
