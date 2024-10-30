//
// Created by thanos on 29/10/2024.
//
#ifndef FLOWFIELDS_HPP
#define FLOWFIELDS_HPP


#include <vector>

#include "Particle.hpp"

class FlowFields {
private:
    std::vector<Particle> particles;
    std::random_device rd{};
    std::mt19937 gen{rd()};
    float width;
    float height;
    std::uniform_real_distribution<float> distribX;
    std::uniform_real_distribution<float> distribY;

public:
    FlowFields(const float width, const float height) {
        this->width = width;
        this->height = height;
        // Initialize the distribution with min and max range {
        particles.reserve(MAX_SIZE); // Pre-allocate for 10,000 particles
        // Initialize the distribution inside the constructor block
        distribX = std::uniform_real_distribution<float>(0, width - 1.0f);
        distribY = std::uniform_real_distribution<float>(0, height - 1.0f);
    }

    ~FlowFields() {
        particles.clear();
    }

    void draw(sf::RenderWindow& window) {
        for (auto& particle: particles) {
            update(particle);
            display(window,particle);
        }

        if (particles.size() < MAX_SIZE) {
            // Initialize particles with random positions
            for (int i = 0; i < 100; ++i) {
                float x = distribX(gen);
                float y = distribY(gen);

                particles.push_back({
                    {x, y},
                    {0,0},
                    sf::Color::Black
                });
            }
        }
    }
};
#endif // FLOWFIELDS_HPP
