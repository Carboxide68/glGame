#include <random>
#include <glm/glm.hpp>

class PerlinNoise {
public:

    PerlinNoise(float min, float max, uint32_t seed);

    inline double Noise1D(const float point) const {return Noise3D({point, 0.0f, 0.0f});}
    inline double Noise2D(const glm::vec2 point) const {return Noise3D({point[0], point[1], 0.0f});}
    inline double Noise3D(const glm::vec3 point) const {return (noise(point) * 0.5f + 0.5f) * (m_Max - m_Min) + m_Min;}

    void reseed(std::uint32_t seed);

private:

    double noise(const glm::vec3 point) const;

    static constexpr double fade(double t) {return t * t * t * (t * (t * 6 - 15) + 10);}
    static constexpr double lerp(double t, double a, double b) {return a + t * (b - a);}
    static constexpr double grad(int hash, double x, double y, double z) {
        const std::uint8_t h = hash & 15;
        const double u = h < 8 ? x : y;
        const double v = h < 4 ? y : (h==12||h==14 ? x : z);
        return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    }

    float m_Min;
    float m_Max;

    uint layerCount;

    std::uint32_t m_Seed;

    std::minstd_rand m_RandomEngine;

    std::uint8_t m_Permutation[512];

};