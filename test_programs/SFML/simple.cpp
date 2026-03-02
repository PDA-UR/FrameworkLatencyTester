#include <SFML/Graphics.hpp>

#define WIDTH 1920
#define HEIGHT 1080

int main()
{
    //sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML default", sf::Style::Fullscreen);
    //sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML default", sf::Style::None, sf::State::Fullscreen);
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "SFML default", sf::Style::None);
    sf::RectangleShape rect(sf::Vector2f(WIDTH, HEIGHT));
    rect.setFillColor(sf::Color::White);

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
		    window.draw(rect);
	    }
	    window.display();
    }
}
