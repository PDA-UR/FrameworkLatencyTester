// based on chatgpt
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

const int WIDTH = 1920;
const int HEIGHT = 1080;
const int NUM_RECTS = 1000;

std::vector<sf::RectangleShape> rectangles;

int randint(int low, int high)
{
    return low + rand() % (high - low);
}

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "framework", sf::Style::Fullscreen);
    // window.setVerticalSyncEnabled(false); disabled by default according to docs

    bool isPressed = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                isPressed = true;
                for (int i = 0; i < NUM_RECTS; i++)
                {
                    int w = randint(100, 500);
                    int h = randint(100, 500);
                    int x = randint(100, WIDTH - w);
                    int y = randint(0, HEIGHT - h);
                    int r = randint(0, 255);
                    int g = randint(0, 255);
                    int b = randint(0, 255);
                    sf::RectangleShape rect(sf::Vector2f(w, h));
                    rect.setFillColor(sf::Color(r, g, b, 255));
                    rect.setPosition(x, y);
                    rectangles.push_back(rect);
                }
                sf::RectangleShape rect(sf::Vector2f(100, 100));
                rect.setFillColor(sf::Color(255, 255, 255, 255));
                rect.setPosition(0, 0);
                rectangles.push_back(rect);
            }
            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                isPressed = false;
            }
        }

        // Draw objects based on the current state
        window.clear(sf::Color::Black);
        if (isPressed)
        {
            for (const auto &rect : rectangles)
            {
                window.draw(rect);
            }
        }

        // Update the window
        window.display();
    }

    return 0;
}
