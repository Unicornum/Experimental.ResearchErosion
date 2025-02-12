
#include "Erosion.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <glm/glm.hpp>
#include "Support.hpp"

/**
* \brief
*  ### Hydraulic Erosion by David Jorna
*
*  Источник: https://github.com/djorna/terrain-generation
*  Скорость работы: медленно
*/
Erosion & Erosion::HydraulicErosionByDavidJorna(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  using Point = ::glm::vec2;
  enum KernelType { MOORE, VON_NEUMANN, VON_NEUMANN2 };
  const auto kernel_type = MOORE;

  const auto iterations = 50;
  //float k_rain = 0.01f, k_solubility = 0.01f, k_evaporation = 0.5f, k_capacity = 0.01f;
  float k_rain = 0.2f, k_solubility = 0.025f, k_evaporation = 0.2f, k_capacity = 0.1f;

  ::std::vector<float> watermap(m_SizeX * m_SizeY, 0.0f);
  ::std::vector<float> sedimentmap(m_SizeX * m_SizeY, 0.0f);

  const auto AtWater = [&](int x, int y) -> float &
  {
    x = ::std::clamp(x, 0, (int)m_SizeX - 1);
    y = ::std::clamp(y, 0, (int)m_SizeY - 1);

    return watermap[x + y * m_SizeX];
  };

  const auto AtSediment = [&](int x, int y) -> float &
  {
    x = ::std::clamp(x, 0, (int)m_SizeX - 1);
    y = ::std::clamp(y, 0, (int)m_SizeY - 1);

    return sedimentmap[x + y * m_SizeX];
  };

  const auto pointInRange = [&](const int r, const int c)
  {
    return (r >= 0) && (r < m_SizeX) && (c >= 0) && (c < m_SizeY);
  };

  const auto neighbours = [&](const Point & point)
  {
    int x = point.x;
    int y = point.y;
    std::vector<Point> points;

    switch (kernel_type)
    {
      case MOORE:
        points = { Point(x - 1, y - 1), Point(x, y - 1), Point(x + 1, y - 1),
                  Point(x - 1, y),                  Point(x + 1, y),
                  Point(x - 1, y + 1), Point(x, y + 1), Point(x + 1, y + 1) };
        break;

      case VON_NEUMANN:
        points = { Point(x, y - 1),
                  Point(x - 1, y),                 Point(x + 1, y),
                                 Point(x, y + 1) };
        break;

      case VON_NEUMANN2:
        points = { Point(x - 1, y - 1),              Point(x + 1, y - 1),

                 Point(x - 1, y + 1),               Point(x + 1, y + 1) };
        break;
    };

    // Cull outlier points (unnessesarily opaque code)
    points.erase(std::remove_if(points.begin(), points.end(),
      [&](Point p) { return !pointInRange(p.y, p.x); }), points.end());
    return points;
  };

  const auto moveMaterial = [&](
    ::std::vector<float> & map,
    const Point & move_from,
    const Point & move_to,
    float amount)
  {
    map[(int)move_from.x + (int)move_from.y * m_SizeX] -= amount;
    map[(int)move_to.x + (int)move_to.y * m_SizeX] += amount;
  };

  const auto sum = [](const std::vector<float> & vec)
  {
    return std::accumulate(vec.begin(), vec.end(), 0.0f);
  };

  const auto average = [](const std::vector<float> & vec)
  {
    return std::accumulate(vec.begin(), vec.end(), 0.0f) / static_cast<float>(vec.size());
  };

  const auto rain = [&](void)
  {
    //watermap += k_rain;
  };

  const auto erosion = [&](void)
  {
#   pragma omp parallel for
    for (int i = 0; i < m_SizeX; ++i)
    {
      for (int j = 0; j < m_SizeY; ++j)
      {
        AtWater(i, j) += k_rain; // вместо rain()

        At(i, j) -= k_solubility * AtWater(i, j);
        AtSediment(i, j) += k_solubility * AtWater(i, j);
      }
    }
  };

  const auto transfer = [&](void)
  {
    ::std::vector<float> delta_watermap(m_SizeX * m_SizeY, 0.0f);
    ::std::vector<float> delta_sedimentmap(m_SizeX * m_SizeY, 0.0f);

    for (int i = 0; i < m_SizeX; ++i)
    {
      for (int j = 0; j < m_SizeY; ++j)
      {
        Point center = Point(i, j);
        float w = AtWater(i, j);
        float h = At(i, j);
        float m = AtSediment(i, j);
        float a = h + w;
        std::vector<float> dlist;
        std::vector<float> alist;
        auto nbs = neighbours(center);
        for (auto it = nbs.cbegin(); it != nbs.cend();)
        {
          float ai = AtWater(it->x, it->y) + At(it->x, it->y);
          if (ai < a)
          {
            dlist.push_back(a - ai);
            alist.push_back(ai);
            ++it;
          }
          else
          {
            it = nbs.erase(it);
          }
        }

        float d_total = sum(dlist);
        float delta_a = a - average(alist);
        float min_val = std::min(w, delta_a);

        // std::cout << "minval: " << min_val << ", d_total: " << d_total << ", delta_a: " << delta_a << '\n';

        for (int i = 0; i < nbs.size(); ++i)
        {
          // std::cout << dlist[i] << '\n';
          float delta_wi = min_val * dlist[i] / d_total;
          // moveMaterial(watermap, center, nbs[i], delta_wi);
          moveMaterial(delta_watermap, center, nbs[i], delta_wi);
          float delta_mi = m * delta_wi / w;
          // moveMaterial(sedimentmap, center, nbs[i], delta_mi);
          moveMaterial(delta_sedimentmap, center, nbs[i], delta_mi);
        }

      }
    }

    // Apply difference operation
    //add(watermap, delta_watermap, watermap);
    //add(sedimentmap, delta_sedimentmap, sedimentmap);
    for (int i = 0; i < m_SizeX; ++i)
    {
      for (int j = 0; j < m_SizeY; ++j)
      {
        AtWater(i, j) += delta_watermap[i + j * m_SizeX];
        AtSediment(i, j) += delta_sedimentmap[i + j * m_SizeX];
      }
    }
  };

  const auto evaporate = [&](void)
  {
    //watermap *= (1.0f - k_evaporation);

    ::std::vector<float> delta_sedimentmap(m_SizeX * m_SizeY, 0.0f);

#   pragma omp parallel for
    for (int i = 0; i < m_SizeX; ++i)
    {
      for (int j = 0; j < m_SizeY; ++j)
      {
        AtWater(i, j) *= (1.0f - k_evaporation);

        float max_sediment = k_capacity * AtWater(i, j);
        float delta_sediment = std::max(0.0f, AtSediment(i, j) - max_sediment);
        // sedimentmap.at<float>(i, j) -= delta_sediment;
        delta_sedimentmap[i + j * m_SizeX] -= delta_sediment;
        At(i, j) += delta_sediment;
      }
    }

    //add(sedimentmap, delta_sedimentmap, sedimentmap);
    for (int i = 0; i < m_SizeX; ++i)
    {
      for (int j = 0; j < m_SizeY; ++j)
      {
        AtSediment(i, j) += delta_sedimentmap[i + j * m_SizeX];
      }
    }
  };

  for (int pass = 0; pass < iterations; ++pass)
  {
    ::std::cout << "Erosion pass " << pass << "/" << iterations << ::std::endl;

    rain();
    erosion();
    transfer();
    evaporate();
  }

  return *this;
}
