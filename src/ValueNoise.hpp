#ifndef VALUENOISE_HPP
#define VALUENOISE_HPP
#include <cmath>
#include <functional>
#include <random>

class ValueNoise {
public:
    static ValueNoise &getInstance() {
        static ValueNoise instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

private:
    ValueNoise() = default;

    // C++ 03
    // ========
    // Don't forget to declare these two. You want to make sure they
    // are inaccessible(especially from outside), otherwise, you may accidentally get copies of
    // your singleton appearing.
    ValueNoise(ValueNoise const &) = delete; // Don't Implement
    void operator=(ValueNoise const &) = delete; // Don't implement

    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<float> distrib{0.0, 1.0};

    int const PERLIN_YWRAPB = 4;
    int const PERLIN_YWRAP = 1 << PERLIN_YWRAPB;
    int const PERLIN_ZWRAPB = 8;
    int const PERLIN_ZWRAP = 1 << PERLIN_ZWRAPB;
    int const PERLIN_SIZE = 4095;

    int const perlin_octaves = 4; // default to medium smooth
    float const perlin_amp_falloff = 0.5f; // 50% reduction/octave
    std::vector<float> perlin;

    static float scaled_cosine(const float t) {
        return 0.5f * (1.0f - std::cosf(t * M_PI));
    }

public:
    float noise(const float _x, const float _y = 0.0f, const float _z = 0.0f) {
        if (perlin.empty()) {
            for (int i = 0; i <= PERLIN_SIZE; i++) {
                perlin.push_back(distrib(gen));
            }
        }

        float x = _x;
        float y = _y;
        float z = _z;

        if (x < 0) {
            x = -x;
        }
        if (y < 0) {
            y = -y;
        }
        if (z < 0) {
            z = -z;
        }

        int xi = std::floor(x);
        int yi = std::floor(y);
        int zi = std::floor(z);
        float xf = x - xi;
        float yf = y - yi;
        float zf = z - zi;

        float r = 0.0f;
        float ampl = 0.5f;

        for (int o = 0; o < perlin_octaves; o++) {
            int of = xi + (yi << PERLIN_YWRAPB) + (zi << PERLIN_ZWRAPB);

            const float rxf = scaled_cosine(xf);
            const float ryf = scaled_cosine(yf);

            float n1 = perlin[of & PERLIN_SIZE];
            n1 += rxf * (perlin[(of + 1) & PERLIN_SIZE] - n1);
            float n2 = perlin[(of + PERLIN_YWRAP) & PERLIN_SIZE];
            n2 += rxf * (perlin[(of + PERLIN_YWRAP + 1) & PERLIN_SIZE] - n2);
            n1 += ryf * (n2 - n1);

            of += PERLIN_ZWRAP;
            n2 = perlin[of & PERLIN_SIZE];
            n2 += rxf * (perlin[(of + 1) & PERLIN_SIZE] - n2);
            float n3 = perlin[(of + PERLIN_YWRAP) & PERLIN_SIZE];
            n3 += rxf * (perlin[(of + PERLIN_YWRAP + 1) & PERLIN_SIZE] - n3);
            n2 += ryf * (n3 - n2);

            n1 += scaled_cosine(zf) * (n2 - n1);

            r += n1 * ampl;
            ampl *= perlin_amp_falloff;
            xi = xi << 1;
            xf *= 2;
            yi = yi << 1;
            yf *= 2;
            zi = zi << 1;

            zf *= 2;

            if (xf >= 1.0) {
                xi++;
                xf--;
            }
            if (yf >= 1.0) {
                yi++;
                yf--;
            }
            if (zf >= 1.0) {
                zi++;
                zf--;
            }
        }
        return r;
    }
};
#endif // VALUENOISE_HPP
