
#include "Erosion.hpp"
#include <iostream>
#include <queue>
#include <glm/glm.hpp>

/**
* \page ThermalErosionByDavidJornaPage
*
*  ### Thermal Erosion
*
*  Скорость обработки: десятки секунд.
*
* \htmlonly

<img class="PreviewImage" src="ThermalErosionByDavidJorna_grayscale_512x512.png">
<img class="PreviewImage" src="ThermalErosionByDavidJorna_colors_512x512.png">
<img class="PreviewImage" src="ThermalErosionByDavidJorna_grayscale_1024x1024.png">
<img class="PreviewImage" src="ThermalErosionByDavidJorna_colors_1024x1024.png">
<img class="PreviewImage" src="ThermalErosionByDavidJorna_grayscale_2048x2048.png">
<img class="PreviewImage" src="ThermalErosionByDavidJorna_colors_2048x2048.png">

* \endhtmlonly
*/
Erosion & Erosion::ThermalErosionByDavidJorna(void)
{
  // https://github.com/djorna/terrain-generation

  using Point = ::glm::vec2;
  const float talus_angle = 0.78f; // T
  const float magnitude = 0.25f; // c
  const auto iterations = 50;
  enum KernelType { MOORE, VON_NEUMANN, VON_NEUMANN2 };
  const auto kernel_type = VON_NEUMANN;

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
    const Point & move_from, 
    const Point & move_to,
    float amount)
  {
    At(move_from.x, move_from.y) -= amount;
    At(move_to.x, move_to.y) += amount;
  };

  const auto operation = [&](const Point & center)
  {
    const auto oNeighbours = neighbours(center);

    float diff_total = 0;
    float diff_max = 0;
    float center_height = At(center.x, center.y);
    std::vector<Point> lower_neighbours;
    std::queue<float> diffs;
    for (Point p : oNeighbours)
    {
      float diff = center_height - At(p.x, p.y);
      if (diff > talus_angle)
      {
        if (diff > diff_max) diff_max = diff;
        diff_total += diff;
        lower_neighbours.push_back(p);
        diffs.push(diff);
      }
    }

    for (Point p : lower_neighbours)
    {
      float diff = diffs.front();
      diffs.pop();
      float amount = magnitude * (diff_max - talus_angle) * diff / diff_total;
      moveMaterial(center, p, amount);
    }
  };

  for (int pass = 0; pass < iterations; ++pass)
  {
    ::std::cout << "Erosion pass " << pass << "/" << iterations << ::std::endl;

#   pragma omp parallel for
    for (int i = 0; i < m_SizeX; ++i)
    {
      for (int j = 0; j < m_SizeY; ++j)
      {
        operation({ i, j });
      }
    }
  }

  return *this;
}
