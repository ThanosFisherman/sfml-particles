//
// Created by thanos on 29/10/2024.
//
#ifndef HSBTORGB_H
#define HSBTORGB_H

#include <cmath>
#include <SFML/Graphics/Color.hpp>

sf::Color HSBtoRGB(float hue, float saturation, float brightness);

#endif // HSBTORGB_H

inline sf::Color HSBtoRGB(const float hue, const float saturation, const float brightness) {
    float chroma = brightness * saturation;
    float x = chroma * (1 - std::abs(fmod(hue / 60.0, 2) - 1));
    float m = brightness - chroma;

    float r, g, b;

    if (hue >= 0 && hue < 60) {
        r = chroma;
        g = x;
        b = 0;
    } else if (hue >= 60 && hue < 120) {
        r = x;
        g = chroma;
        b = 0;
    } else if (hue >= 120 && hue < 180) {
        r = 0;
        g = chroma;
        b = x;
    } else if (hue >= 180 && hue < 240) {
        r = 0;
        g = x;
        b = chroma;
    } else if (hue >= 240 && hue < 300) {
        r = x;
        g = 0;
        b = chroma;
    } else {
        r = chroma;
        g = 0;
        b = x;
    }

    // Convert to RGB range and apply offset
    auto red = static_cast<uint8_t>((r + m) * 255);
    auto green = static_cast<uint8_t>((g + m) * 255);
    auto blue = static_cast<uint8_t>((b + m) * 255);

    return {red, green, blue};
}