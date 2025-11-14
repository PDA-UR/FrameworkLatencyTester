#include <SFML/Graphics.hpp>

#define WIDTH 1920
#define HEIGHT 1080

int main()
{
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML default", sf::Style::Fullscreen);
    sf::RectangleShape rect(sf::Vector2f(WIDTH, HEIGHT));
    rect.setFillColor(sf::Color::White);

	int clicked = 0;

    while (window.isOpen())
    {
		sf::Event event;
        while (window.pollEvent(event))
        {
			switch (event.type)
			{
			case sf::Event::Closed:
				  window.close();
				  break;
			case sf::Event::MouseButtonPressed:
				  clicked = 1;
				  break;
			case sf::Event::MouseButtonReleased:
				  clicked = 0;
				  break;
			}
        }

        window.clear();
		if (clicked)
		{
			window.draw(rect);
		}
        window.display();
    }
}
