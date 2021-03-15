#include <SFML/Graphics.hpp>
#include <vector>

enum class ParticleID
{
	EMPTY,
	SAND
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

			default:
				break;
		}
	}
};

// NOTE: This method works for simple simulation, but more complex physics will require a more complex method
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

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Pixel Particles");
	window.setFramerateLimit(60);

	// Vector of cells. Each cell will contain a particle, either empty or of a certain material. Initialize all cells to empty
	std::vector<std::vector<Particle>> particles((int)(SCREEN_WIDTH / CELL_SIZE), std::vector<Particle>((int)(SCREEN_HEIGHT / CELL_SIZE)));

	for (int y = 0; y < SCREEN_HEIGHT / CELL_SIZE; y++)
	{
		for (int x = 0; x < SCREEN_WIDTH / CELL_SIZE; x++)
		{
			// Put sand in the middle column
			if (x == SCREEN_WIDTH / CELL_SIZE / 2)
				particles[x][y] = Particle(ParticleID::SAND);

			particles[x][y].Rect.setPosition((float)x * CELL_SIZE, (float)y * CELL_SIZE);
		}
	}

	while (window.isOpen())
	{
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
				// Get the ID of the current particle
				ParticleID partID = particles[x][y].ID;

				switch(partID)
				{
					case ParticleID::SAND:
					{
						if (x >= 0 && x < SCREEN_WIDTH / CELL_SIZE - 1 && y >= 0 && y < SCREEN_HEIGHT / CELL_SIZE - 1)
						{
							// Down empty
							if (particles[x][y + 1].ID == ParticleID::EMPTY)
								swapParticles(particles[x][y], particles[x][y + 1]);

							// Down left empty
							else if (particles[x - 1][y + 1].ID == ParticleID::EMPTY)
								swapParticles(particles[x][y], particles[x - 1][y + 1]);

							// Down right empty
							else if (particles[x + 1][y + 1].ID == ParticleID::EMPTY)
								swapParticles(particles[x][y], particles[x + 1][y + 1]);
						}
					}

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
