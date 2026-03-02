#include <SFML/Graphics.hpp>
#include <cstdlib>

#define WIDTH 1920
#define HEIGHT 1080

int n_rects = 0;
int rect_w = WIDTH / 10;
int rect_h = HEIGHT / 10;

void createRects(sf::RectangleShape* rects)
{
	for (int i = 0; i < n_rects; i++)
	{
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;

        int x = rect_w + rand() % (WIDTH - 2 * rect_w);
        int y = rand() % (HEIGHT - rect_h);
		
		rects[i] = sf::RectangleShape(sf::Vector2f(rect_w, rect_h));
		rects[i].setFillColor(sf::Color(r, g, b));
		rects[i].setPosition({x, y});
	}
}

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		n_rects = atoi(argv[1]) - 1;
		if (n_rects < 0) n_rects = 0;
	}

	//sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML default", sf::Style::Fullscreen);
	//sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML default", sf::Style::None, sf::State::Fullscreen);
	sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML default", sf::Style::None);
	//window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(500);
	sf::RectangleShape rects[n_rects];

	sf::RectangleShape whiteRect(sf::Vector2f(300, HEIGHT));
	whiteRect.setFillColor(sf::Color::White);

	int clicked = 0;

	while (window.isOpen())
	{
		//sf::Event event;
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			else if (event->is<sf::Event::MouseButtonPressed>())
			{
				createRects(rects);
				clicked = 1;
			}
			else if (event->is<sf::Event::MouseButtonReleased>())
			{

				clicked = 0;
			}
		}

		window.clear();
		if (clicked)
		{
			for (int i = 0; i < n_rects; i++)
			{
				window.draw(rects[i]);
			}
			window.draw(whiteRect);
		}
		window.display();
	}
}
