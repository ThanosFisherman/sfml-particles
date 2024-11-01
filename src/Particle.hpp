//
// Created by thanos on 29/10/2024.
//

#ifndef PARTICLE_H
#define PARTICLE_H
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#ifndef M_PI
#define M_PI 3.14159265358979323846 // Define M_PI if it's not defined
#endif
#include <cmath>
#include "Config.h"
#include "HSBtoRGB.hpp"
#include "ValueNoise.hpp"
#include "Framerate.hpp"
#include <iostream>

struct Particle {
    sf::Vector2f position;
    sf::Vector2f vel;
    sf::Color color;
};


inline float noiseVal = 0;
inline Framerate framerate;
#endif //PARTICLE_H


inline void fromAngle(const float angle, sf::Vector2f &target) {
    target.x = std::cos(angle);
    target.y = std::sin(angle);
}

// Utility function to set length of an sf::Vector2f
inline void setLength(sf::Vector2f &vector, const float newLength) {
    const float currentLength = std::sqrt(vector.x * vector.x + vector.y * vector.y);

    if (currentLength == 0.0f) {
        vector.x = 0.0f;
        vector.y = 0.0f;
        return;
    }

    // Calculate the scale factor and apply it to the vector
    const float scale = newLength / currentLength;
    vector.x = vector.x * scale;
    vector.y = vector.y * scale;
}

inline void display(sf::RenderWindow &window, Particle &particle) {
    const auto h = std::fmod(255 * noiseVal * COLOR_FREQ, 255.0f);
    sf::Color color = HSBtoRGB((h / 255) * 360, 1, 1);
    color.a = TRANSPARENCY;
    particle.color = color;
    sf::CircleShape circle(RADIUS);
    circle.setPosition(particle.position);
    circle.setFillColor(particle.color);
    window.draw(circle);
}

inline void update(Particle &particle) {
    ValueNoise &valueNoise = ValueNoise::getInstance();
    noiseVal = valueNoise.noise(particle.position.x * STRENGTH, particle.position.y * STRENGTH);
    fromAngle(noiseVal * ANGLE_MAG * (M_PI * 2), particle.vel);
    setLength(particle.vel, SPEED);
    const float fps = framerate.getFPS();

    particle.vel.x /= fps;
    particle.vel.y /= fps;
    particle.position.x += particle.vel.x;
    particle.position.y += particle.vel.y;
}
