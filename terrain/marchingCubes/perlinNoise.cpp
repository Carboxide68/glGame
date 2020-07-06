#include "perlinNoise.h"
#include <cmath>
#include <algorithm>
#include <iterator>

PerlinNoise::PerlinNoise(float min, float max, uint32_t seed) {
    m_Min = min;
    m_Max = max;
    reseed(seed);
}

double PerlinNoise::noise(const glm::vec3 point) const {
    float x = point.x;
    float y = point.y;
    float z = point.z;
    const std::int32_t X = static_cast<std::int32_t>(std::floor(x)) & 255;
    const std::int32_t Y = static_cast<std::int32_t>(std::floor(y)) & 255;
    const std::int32_t Z = static_cast<std::int32_t>(std::floor(z)) & 255;
    
    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    const float u = fade(x);
    const float v = fade(y);
    const float w = fade(z);

    const std::int32_t A = m_Permutation[X  ] + Y, AA = m_Permutation[A] + Z, AB = m_Permutation[A+1] + Z;
    const std::int32_t B = m_Permutation[X+1] + Y, BA = m_Permutation[B] + Z, BB = m_Permutation[B+1] + Z;

    return lerp(w, lerp(v, lerp(u, grad(m_Permutation[AA], x, y, z),
				grad(m_Permutation[BA], x - 1, y, z)),
				lerp(u, grad(m_Permutation[AB], x, y - 1, z),
				grad(m_Permutation[BB], x - 1, y - 1, z))),
				lerp(v, lerp(u, grad(m_Permutation[AA + 1], x, y, z - 1),
				grad(m_Permutation[BA + 1], x - 1, y, z - 1)),
				lerp(u, grad(m_Permutation[AB + 1], x, y - 1, z - 1),
				grad(m_Permutation[BB + 1], x - 1, y - 1, z - 1))));
}

void PerlinNoise::reseed(std::uint32_t seed) {
    m_RandomEngine.seed(seed);
    m_RandomEngine();

    for (int i = 0; i < 256; i++) {
        m_Permutation[i] = static_cast<std::uint8_t>(i);
    }

    std::shuffle(std::begin(m_Permutation), std::begin(m_Permutation) + 256, m_RandomEngine);

    for (int i = 0; i < 256; i++) {
        m_Permutation[i + 256] = m_Permutation[i];
    }
}