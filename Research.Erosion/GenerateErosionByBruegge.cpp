
#include "Erosion.hpp"
#include <algorithm>
#include"Support.hpp"

/**
* \brief
*  ### Generate Erosion by Bruegge
*
*  »сточник: https://github.com/bruegge/Realtime-Procedural-Terrain-Generation
*  —корость работы: быстро
*/
Erosion & Erosion::GenerateErosionByBruegge(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  ::std::vector<float> SourceHeightMap(m_SizeX * m_SizeY, 0.0f);

  const auto AtSourceHeightMap = [&](int x, int y) -> float &
  {
    // ¬ оригинале и дл€ чтени€ и дл€ записи используетс€ одна и та же карта
    // высот, но в этом случае получаетс€ нечто вроде сглаживани€ неровностей.
    //return At(x, y);
    x = ::std::clamp(x, 0, (int)m_SizeX - 1);
    y = ::std::clamp(y, 0, (int)m_SizeY - 1);

    return SourceHeightMap[x + y * m_SizeX];
  };

  for (int i = 0; i < m_SizeX; ++i)
  {
    for (int j = 0; j < m_SizeY; ++j)
    {
      AtSourceHeightMap(i, j) = At(i, j);
    }
  }

  float di[3][3] = { 0 };
  float hi[3][3] = { 0 };
  float c = 0.25f;
  float N = m_SizeX;//m_nWidth;
  float T = 0.015f;//8.0f * 4.0f / N;

  for (int step = 0; step < 100; ++step);
  {
    for (int i = 1; i < m_SizeX - 1; ++i)
    {
      for (int j = 1; j < m_SizeY - 1; ++j)
      {
        int indexXDmax = 0;
        int indexYDmax = 0;
        float dTotal = 0.02f;

        //calculate neighborhoodHeights
        float h = AtSourceHeightMap(i, j);
        for (int x = 0; x < 3; ++x)
        {
          for (int y = 0; y < 3; ++y)
          {
            hi[x][y] = AtSourceHeightMap(i + (x - 1), j + (y - 1));
            di[x][y] = h - AtSourceHeightMap(i + (x - 1), j + (y - 1));

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
            At((i + x - 1), (j + y - 1)) = hi[x][y];
          }
        }
      }
    }
  }

  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Blur(2.0f);

  return *this;
}
