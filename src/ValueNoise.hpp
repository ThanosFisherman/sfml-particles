#ifndef VALUENOISE_HPP
#define VALUENOISE_HPP
#include <functional>
#include <random>
#include <SFML/System/Vector2.hpp>

class ValueNoise {
public:
    explicit ValueNoise() {
        std::random_device rd{};
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distrFloat;
        auto randFloat = std::bind(distrFloat, gen);

        // create an array of random values and initialize permutation table
        for (unsigned k = 0; k < kMaxTableSize; ++k) {
            r[k] = randFloat();
            permutationTable[k] = k;
        }

        // shuffle values of the permutation table
        std::uniform_int_distribution<unsigned> distrUInt;
        auto randUInt = std::bind(distrUInt, gen);
        for (unsigned k = 0; k < kMaxTableSize; ++k) {
            unsigned i = randUInt() & kMaxTableSizeMask;
            std::swap(permutationTable[k], permutationTable[i]);
            permutationTable[k + kMaxTableSize] = permutationTable[k];
        }
    }

    static float smoothstep(const float &t) {
        return t * t * (3 - 2 * t);
    }

    [[nodiscard]] float eval(sf::Vector2f p) const {
        int xi = std::floor(p.x);
        int yi = std::floor(p.y);

        float tx = p.x - xi;
        float ty = p.y - yi;

        int rx0 = xi & kMaxTableSizeMask;
        int rx1 = (rx0 + 1) & kMaxTableSizeMask;
        int ry0 = yi & kMaxTableSizeMask;
        int ry1 = (ry0 + 1) & kMaxTableSizeMask;

        // random values at the corners of the cell using permutation table
        const float &c00 = r[permutationTable[permutationTable[rx0] + ry0]];
        const float &c10 = r[permutationTable[permutationTable[rx1] + ry0]];
        const float &c01 = r[permutationTable[permutationTable[rx0] + ry1]];
        const float &c11 = r[permutationTable[permutationTable[rx1] + ry1]];

        // remapping of tx and ty using the Smoothstep function
        float sx = smoothstep(tx);
        float sy = smoothstep(ty);

        // linearly interpolate values along the x axis
        float nx0 = std::lerp(c00, c10, sx);
        float nx1 = std::lerp(c01, c11, sx);

        // linearly interpolate the nx0/nx1 along they y axis
        return std::lerp(nx0, nx1, sy);
    }

    static const unsigned kMaxTableSize = 256;
    static const unsigned kMaxTableSizeMask = kMaxTableSize - 1;
    float r[kMaxTableSize]{};
    unsigned permutationTable[kMaxTableSize * 2]{};
};
#endif // VALUENOISE_HPP
