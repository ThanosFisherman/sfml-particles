#include <cmath>
#include <SFML/Graphics.hpp>
#include "FlowFields.hpp"


int main() {
    // Get the current screen resolution and color depth
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    float width = 0.45f * static_cast<float>(desktopMode.size.x);
    float height = width / 16.0f * 9.0f;

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(width, height), desktopMode.bitsPerPixel), "Particle Flow");
    sf::View view(sf::Vector2f(width / 2, height / 2), sf::Vector2f(width, -height));
    window.setView(view);
    window.setVerticalSyncEnabled(true);

    sf::Font font;

    if (!font.openFromFile("../assets/OpenSans.ttf")) {
        std::cerr << "Error loading font\n";
        return -1;
    }

    // Create a text object to display FPS
    sf::Text fpsText(font);
    fpsText.setCharacterSize(24);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setPosition(sf::Vector2f(10, height - 20));
    fpsText.scale(sf::Vector2f(1, -1));

    FlowFields flowFields(width, height);

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }


        // Update particles
        framerate.update();
        fpsText.setString("FPS: " + std::to_string(static_cast<int>(framerate.getFPS())));

        window.clear();

        flowFields.draw(window);

        window.draw(fpsText);

        window.display();
    }

    return 0;
}
