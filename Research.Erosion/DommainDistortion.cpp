
#include <iostream>
#include "Erosion.hpp"
#include <glm/glm.hpp>
#include "Support.hpp"

/**
* \brief
*  ### Domain Distortion by Inigo Quilez
*  
*  Источник: https://iquilezles.org/articles/warp/
*  
*  Не вляется эрозией как таковой, дает красивый результат, но для реального
*  использования в качестве карты высот пригодно разве что для генерации
*  безумных инопланетных пейзажей.
*/
Erosion & Erosion::DommainDistortion(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  using namespace ::glm;

  ::std::vector<float> Result(m_SizeX * m_SizeY);

#   ifdef _OPENMP
#   pragma omp parallel for
#   endif
  for (int y = 0; y < m_SizeY; y++)
  {
    for (int x = 0; x < m_SizeX; x++)
    {
      Result[x + y * m_SizeX] = At(x, y);
    }
  }

  const auto fbm = [&](vec2 p)
  {
    const int X = (int)(p.x * m_SizeX) % m_SizeX;
    const int Y = (int)(p.y * m_SizeY) % m_SizeY;

    return Result[X + Y * m_SizeX];
  };

  const auto pattern = [&](vec2 p)
  {
    vec2 q = vec2(
      fbm(p + vec2(0.0f, 0.0f)),
      fbm(p + vec2(5.2f, 1.3f)));

    return fbm(p + 4.0f * q);
  };

  const auto pattern2 = [&](vec2 p)
  {
    vec2 q = vec2(
      fbm(p + vec2(0.0f, 0.0f)),
      fbm(p + vec2(5.2f, 1.3f)));

    vec2 r = vec2(
      fbm(p + 4.0f * q + vec2(1.7f, 9.2f)),
      fbm(p + 4.0f * q + vec2(8.3f, 2.8f)));

    return fbm(p + 4.0f * r);
  };

#   ifdef _OPENMP
#   pragma omp parallel for
#   endif
  for (int y = 0; y < m_SizeY; y++)
  {
    for (int x = 0; x < m_SizeX; x++)
    {
      At(x, y) =
        pattern({ (float)x / (m_SizeX - 1), (float)y / (m_SizeY - 1) });
        //pattern2({ (float)x / (m_SizeX - 1), (float)y / (m_SizeY - 1) });
    }
  }

  return *this;
}
