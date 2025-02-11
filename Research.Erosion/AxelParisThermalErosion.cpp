
#include <iostream>
#include "Erosion.hpp"
#include <glm/glm.hpp>

/**
* \brief
*  ### Thermal Erosion by Axel Paris
*
*  Источник: https://aparis69.github.io/public_html/posts/terrain_erosion_2.html
*  
*  Простой алгоритм, не совсем полноценная эрозия (автор сам об этом пишет),
*  дает нечто похожее на хребты гор, но склоны получаются неестественно ровными.
*/
Erosion & Erosion::AxelParisThermalErosion(void)
{
  const auto converseCellSize = 1.0f / m_MetersPerXY;

  const int nx = m_SizeX;
  const int ny = m_SizeY;
  ::std::vector<float> TempBuffer(nx * ny);

  At_t AtData = At;

  At_t AtOutData = [&](int x, int y) -> float &
  {
    return TempBuffer[x + y * nx];
  };

  const auto NoRaceConditionVersion = [&](int x, int y)
  {
    const auto tanThresholdAngle = 1.0f; // 45°
    const auto amplitude = 0.2f;

    // Check stability with all neighbours
    float z = AtData(x, y);

    bool IsWillReceiveMatter = false;
    bool IsWillDistributeMatter = false;

    // Sample a 3x3 grid around the pixel
    const ::std::vector<::glm::ivec2> Grid3x3 =
    {
      { 0, -1 }, { -1, 0 }, { 0, 1 }, { 1, 0 },
      { 1, -1 }, { -1, 1 }, { -1, 1 }, { 1, -1 },
    };

    for (const auto & Cell : Grid3x3)
    {
      const int cX = x + Cell.x;
      if (cX < 0 || cX >= nx) continue;

      const int cY = y + Cell.y;
      if (cY < 0 || cY >= nx) continue;

      float zd = converseCellSize * (z - AtData(cX, cY));
      if (-zd > tanThresholdAngle) IsWillReceiveMatter = true;
      if (zd > tanThresholdAngle) IsWillDistributeMatter = true;
    }

    // Add/Remove matter if necessary
    AtOutData(x, y) = z + (IsWillReceiveMatter ? amplitude : 0.0f) -
      (IsWillDistributeMatter ? amplitude : 0.0f);
  };

  const auto NoRaceConditionVersion2 = [&](int x, int y)
  {
    const auto tanThresholdAngle = 0.6f;
    const auto amplitude = 0.1f;

    // Check stability with all neighbours
    float z = AtData(x, y);

    // Sample a 3x3 grid around the pixel
    const ::std::vector<::glm::ivec2> Grid3x3 =
    {
      { 0, -1 }, { -1, 0 }, { 0, 1 }, { 1, 0 },
      { 1, -1 }, { -1, 1 }, { -1, 1 }, { 1, -1 },
    };

    int WillMatter = 0;

    for (const auto & Cell : Grid3x3)
    {
      const int cX = x + Cell.x;
      if (cX < 0 || cX >= nx) continue;

      const int cY = y + Cell.y;
      if (cY < 0 || cY >= nx) continue;

      const float zd = converseCellSize * (z - AtData(cX, cY));
      if (-zd > tanThresholdAngle) WillMatter++;
      if (zd > tanThresholdAngle) WillMatter--;
    }

    // Add/Remove matter if necessary
    AtOutData(x, y) = z + WillMatter * amplitude;
  };

  const auto Count = 1000;

  for (int i = 0; i < Count; i++)
  {
    if (i % (Count / 100) == 0)
    {
      ::std::cout << "Erosion " << i / (Count / 100) << " % " << ::std::endl;
    }

#     ifdef _OPENMP
#     pragma omp parallel for
#     endif
    for (int y = 0; y < ny; y++)
    {
      for (int x = 0; x < nx; x++)
      {
        NoRaceConditionVersion(x, y);
      }
    }

    ::std::swap(AtData, AtOutData);
  }

  *this;
}
