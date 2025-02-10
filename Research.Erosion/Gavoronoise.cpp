
#include <iostream>
#include "Erosion.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

Erosion & Erosion::Gavoronoise(void)
{
  // https://www.shadertoy.com/view/MtGcWh

  ::std::vector<float> Source(m_SizeX * m_SizeY);

  const auto AtSource = [&](int _X, int _Y) -> float &
  {
    const int x = (_X + m_SizeX) % m_SizeX;
    const int y = (_Y + m_SizeY) % m_SizeY;

    return Source[x + y * m_SizeX];
  };

#   ifdef _OPENMP
#   pragma omp parallel for
#   endif
  for (int y = 0; y < m_SizeY; y++)
  {
    for (int x = 0; x < m_SizeX; x++)
    {
      AtSource(x, y) = At(x, y);
    }
  }

  using namespace ::glm;

  const auto hash = [&](vec2 x)
  {
    const vec2 k = vec2(0.3183099f, 0.3678794f);
    x = x * k + vec2(k.y, k.x);
    return -1.0f + 2.0f * fract(16.0f * k * fract(x.x * x.y * (x.x + x.y)));
  };

  const auto erosion = [&](vec2 p, vec2 dir)
  {
    vec2 ip = floor(p);
    vec2 fp = fract(p);
    float f = 2.0f * ::glm::pi<float>();
    vec3 va = vec3(0.0f);
    float wt = 0.0f;
    for (int i = -2; i <= 1; i++)
    {
      for (int j = -2; j <= 1; j++)
      {
        vec2 o = vec2(i, j);
        vec2 h = hash(ip - o) * 0.5f;
        vec2 pp = fp + o - h;
        float d = dot(pp, pp);
        float w = exp(-d * 2.0f);
        wt += w;
        float mag = dot(pp, dir);
        va += vec3(cos(mag * f), -sin(mag * f) * (pp + dir)) * w;
      }
    }
    return va / wt;
  };

  const auto mountain = [&](vec2 p, vec3 n)
  {
    //take the curl of the normal to get the gradient facing down the slope
    vec2 dir = vec2(n.z, -n.y);
    //vec2 dir = vec2(n.y, n.z); // тоже интересный эффект

    //Now we compute another fbm type noise
    // erosion is a type of noise with a strong directionality
    //we pass in the direction based on the slope of the terrain
    //erosion also returns the slope. we add that to a running total
    //so that the direction of successive layers are based on the
    //past layers
    vec3 h = vec3(0.0f);
    //float a = 0.7f * (smoothstep(0.3f, 0.5f, n.x * 0.5f + 0.5f)); //smooth the valleys
    float a = 0.7f * (smoothstep(0.3f, 0.75f, n.x)); //smooth the valleys
    float f = 1.0f;
    for (int i = 0; i < 5; i++)
    {
      h += erosion(p * f, dir + vec2(h.z, -h.y)) * a * vec3(1.0f, f, f);
      a *= 0.4f;
      f *= 2.0f;
    }

    //remap height to [0,1] and add erosion
    //looks best when erosion amount is small
    //not sure about adding the normals together, but it looks okay
    return smoothstep(-1.0f, 1.0f, n.x) + 
      h.x * 0.1f; // глубина изрезанности гор
  };

#   ifdef _OPENMP
#   pragma omp parallel for
#   endif
  for (int y = 0; y < m_SizeY; y++)
  {
    for (int x = 0; x < m_SizeX; x++)
    {
      vec2 uv = vec2(x, y) / vec2(m_SizeX, m_SizeY);

      auto v1 = vec3(vec2(x + 1, y) / vec2(m_SizeX, m_SizeY), AtSource(x + 1, y));
      auto v2 = vec3(vec2(x - 1, y) / vec2(m_SizeX, m_SizeY), AtSource(x - 1, y));
      auto v3 = vec3(vec2(x, y + 1) / vec2(m_SizeX, m_SizeY), AtSource(x, y + 1));
      auto v4 = vec3(vec2(x, y - 1) / vec2(m_SizeX, m_SizeY), AtSource(x, y - 1));
      const auto Normal = ::glm::normalize(::glm::cross(v4 - v3, v2 - v1));
      const auto NormalXY = ::glm::normalize(vec2(Normal.x, Normal.y));

      At(x, y) = mountain(
        uv * 48.0f, // степень изрезанности гор
        vec3(AtSource(x, y), NormalXY));
    }
  }

  return *this;
}
