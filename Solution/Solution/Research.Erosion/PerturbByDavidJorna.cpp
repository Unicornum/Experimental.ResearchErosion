
#include "Erosion.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include "Support.hpp"
#include "..\SimplexNoise\src\SimplexNoise.h"

/**
* \brief
*  ### Pertur by David Jorna
*
*  Источник: https://github.com/djorna/terrain-generation
*  Скорость работы: быстро
*/
Erosion & Erosion::PerturbByDavidJorna(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  using Offset = ::glm::vec3;
  float mag = 0.1;
  Offset offset1 = { 0.0f, 0.0f, 0.0f };
  Offset offset2 = { 1.5f, 6.7f, 3.4f };

  ::std::vector<float> new_img(m_SizeX * m_SizeY);

# pragma omp parallel for
  for (int i = 0; i < m_SizeX; ++i)
  {
    for (int j = 0; j < m_SizeY; ++j)
    {
      float val = At(i, j);
      float s = static_cast<float>(i) / static_cast<float>(m_SizeX);
      float t = static_cast<float>(j) / static_cast<float>(m_SizeY);

      float ss = s + mag * SimplexNoise::noise(s + offset1.x, t + offset1.y, val + offset1.z);
      float tt = t + mag * SimplexNoise::noise(s + offset2.x, t + offset2.y, val + offset2.z);

      float y2 = std::min(static_cast<float>(m_SizeX), std::max(0.0f, ss * static_cast<float>(m_SizeX)));
      float x2 = std::min(static_cast<float>(m_SizeY), std::max(0.0f, tt * static_cast<float>(m_SizeY)));

      new_img[i + j * m_SizeY] = At(x2, y2);
    }
  }

# pragma omp parallel for
  for (int i = 0; i < m_SizeX; ++i)
  {
    for (int j = 0; j < m_SizeY; ++j)
    {
      At(i, j) = new_img[i + j * m_SizeY];
    }
  }

  return *this;
}
