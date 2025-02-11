
#include <iostream>
#include "Erosion.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Support.hpp"

/**
* \brief
*  ### Eroded Terrain Noise
*
*  Источник: https://www.shadertoy.com/view/MtGcWh
*
*  Работает быстро (за один проход), по сути просто накладвает шум с учетом
*  нормалей. Обработка готовой карты высот дает вполне убедительный результат.
*/
Erosion & Erosion::Gavoronoise(void)
{
  using namespace ::glm;

  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.3f);

  // Нормали считаем сразу, т.к. они нужны для еще не измененной карты высот
  ::std::vector<vec3> Normals(m_SizeX * m_SizeY);

#   ifdef _OPENMP
#   pragma omp parallel for
#   endif
  for (int y = 0; y < m_SizeY; y++)
  {
    for (int x = 0; x < m_SizeX; x++)
    {
      Normals[x + y * m_SizeX] = GetNormal(x, y);
    }
  }

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
    float a = 0.7f * (smoothstep(0.4f, // нижняя высота обработки (0.2f даст изрезанную линию моря)
      0.8f, n.x * 0.5f + 0.5f)); //smooth the valleys
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
      auto uv = vec2(x, y) / vec2(m_SizeX, m_SizeY);
      auto & norm = Normals[x + y * m_SizeX];

      At(x, y) = mountain(
        uv * 48.0f, // степень изрезанности гор
        vec3(At(x, y), vec2(norm.x, norm.y)));
    }
  }

  return *this;
}
