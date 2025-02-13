
#include "Erosion.hpp"
#include <algorithm>
#include"Support.hpp"

/**
* \brief
*  ### Generate Erosion by Bruegge
*
*  Источник: https://github.com/bruegge/Realtime-Procedural-Terrain-Generation
*  Скорость работы: быстро
*/
Erosion & Erosion::GenerateErosionByBruegge(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  float di[3][3] = { 0 };
  float hi[3][3] = { 0 };
  float c = 0.5f;
  float N = m_SizeX;//m_nWidth;
  float T = 8.0f * 4.0f / N;

  for (int step = 0; step < 100; ++step);
  {
    int indexXDmax = 0;
    int indexYDmax = 0;
    float dTotal = 0;

    for (int i = 1; i < m_SizeX - 1; ++i)
    {
      for (int j = 1; j < m_SizeY - 1; ++j)
      {
        //calculate neighborhoodHeights
        float h = At(i, j);
        for (int x = 0; x < 3; ++x)
        {
          for (int y = 0; y < 3; ++y)
          {
            hi[x][y] = At(i + (x - 1), j + (y - 1));
            di[x][y] = h - At(i + (x - 1), j + (y - 1));

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

        if (dTotal == 0.0f) continue;

        for (int x = 0; x < 3; ++x)
        {
          for (int y = 0; y < 3; ++y)
          {
            hi[x][y] = hi[x][y] + c * (hi[indexXDmax][indexYDmax] - T) * di[x][y] / dTotal;
            At((i + x - 1), (j + y - 1)) = hi[x][y];
          }
        }
      }
    }
  }

  return *this;
}
