
#include "Erosion.hpp"
#include <algorithm>
#include <iostream>
#include"Support.hpp"

/**
* \brief
*  ### Generate Erosion by Bruegge
*
*  Источник: https://github.com/bruegge/Realtime-Procedural-Terrain-Generation
*  Скорость работы: медленно
*/
Erosion & Erosion::GenerateErosionByBruegge(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  ::std::vector<float> TempHeightMap(m_SizeX * m_SizeY, 0.0f);

  At_t GetTerrainHeight = At;

  At_t AtResult = [&](int x, int y) -> float &
  {
    x = ::std::clamp(x, 0, (int)m_SizeX - 1);
    y = ::std::clamp(y, 0, (int)m_SizeY - 1);

    return TempHeightMap[x + y * m_SizeX];
  };

  auto m_nWidth = ::std::max(m_SizeX, m_SizeY);

  float di[3][3];
  float hi[3][3];
  float c = 0.25f;
  float N = m_nWidth;
  float T = 4.0f / N;

  for (int step = 0; step < 100; ++step) //; - так было в оригинале, рукалицо
  {
    ::std::cout << "Erosion step: " << step << "/" << 100 << ::std::endl;

    for (int i = 1; i < m_nWidth - 1; ++i)
    {
      for (int j = 1; j < m_nWidth - 1; ++j)
      {
        int indexXDmax = 0;
        int indexYDmax = 0;
        float dTotal = 1.0f;

        //calculate neighborhoodHeights
        float h = GetTerrainHeight(i, j);
        for (int x = 0; x < 3; ++x)
        {
          for (int y = 0; y < 3; ++y)
          {
            hi[x][y] = GetTerrainHeight(i + (x - 1), j + (y - 1));
            di[x][y] = h - GetTerrainHeight(i + (x - 1), j + (y - 1));

            if (hi[x][y] > hi[indexXDmax][indexYDmax])
            {
              indexXDmax = x;
              indexYDmax = y;
            }
            if (di[x][y] > T)
            {
              dTotal += di[x][y];
            }
          }
        }

        for (int x = 0; x < 3; ++x)
        {
          for (int y = 0; y < 3; ++y)
          {
            hi[x][y] = hi[x][y] + c * (hi[indexXDmax][indexYDmax] - T) * di[x][y] / dTotal;
            AtResult((i + x - 1), (j + y - 1)) = hi[x][y];
          }
        }
      }
    }

    ::std::swap(GetTerrainHeight, AtResult);
  }

  return *this;
}
