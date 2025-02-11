
#include "Erosion.hpp"
#include "Random.hpp"
#include "Support.hpp"

/**
* \brief
*  ### Water Hydraulic Erosion by Job Talle
*
*  Источник: https://jobtalle.com/simulating_hydraulic_erosion.html
*
*  Работает быстро, дает вполне убедительный результат.
*/
Erosion & Erosion::JobTalleHydraulicErosion(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  const auto maxIterations = 80;
  const auto radius = 0.8f;
  const auto iterationScale = 0.04f;
  const auto erosionRate = 0.04f;
  const auto depositionRate = 0.03f;
  const auto speed = 0.15f;
  const auto friction = 0.7f;

  ::std::vector<float> Erosion(m_SizeX * m_SizeY, 0.0f);

  const auto AtErosion = [&](int _X, int _Y) -> float &
  {
    const int x = (_X + m_SizeX) % m_SizeX;
    const int y = (_Y + m_SizeY) % m_SizeY;

    return Erosion[x + y * m_SizeX];
  };

  /**
 * Let a snowball erode the height map
 * @param {Number} x The X coordinate to start at
 * @param {Number} y The Y coordinate to start at
 */
  const auto trace = [&](float x, float y)
  {
    const auto ox = (Random(0.0f, 1.0f) * 2.0f - 1.0f) * radius; // The X offset
    const auto oy = (Random(0.0f, 1.0f) * 2.0f - 1.0f) * radius; // The Y offset
    auto sediment = 0.0f; // The amount of carried sediment
    auto xp = x; // The previous X position
    auto yp = y; // The previous Y position
    auto vx = 0.0f; // The horizontal velocity
    auto vy = 0.0f; // The vertical velocity

    for (auto i = 0; i < maxIterations; ++i)
    {
      // Get the surface normal of the terrain at the current location
      const auto surfaceNormal = GetNormal(x + ox, y + oy);

      // If the terrain is flat, stop simulating, the snowball cannot roll any further
      if (surfaceNormal.z >= 0.99f)
        break;

      // Calculate the deposition and erosion rate
      const auto deposit = sediment * depositionRate * surfaceNormal.z;
      const auto erosion = erosionRate * (1.0f - surfaceNormal.z) * ::std::min(1.0f, i * iterationScale);

      // Change the sediment on the place this snowball came from
      AtErosion(xp, yp) += deposit - erosion;
      sediment += erosion - deposit;

      vx = friction * vx + surfaceNormal.x * speed;
      vy = friction * vy + surfaceNormal.y * speed;
      xp = x;
      yp = y;
      x += vx;
      y += vy;
    }
  };

  const auto snowballs = 200000;

  for (auto i = 0; i < snowballs; ++i)
  {
    trace(
      //Random(0.0f, 1.0f) * m_SizeX,
      //Random(0.0f, 1.0f) * m_SizeY);

      // Дает более грубый - и более похожий на реальность результат.
      10 * (int)(Random(0.0f, 0.1f) * m_SizeX),
      10 * (int)(Random(0.0f, 0.1f) * m_SizeY));
  }

#   ifdef _OPENMP
#   pragma omp parallel for
#   endif
  for (int y = 0; y < m_SizeY; y++)
  {
    for (int x = 0; x < m_SizeX; x++)
    {
      // убираем мусор
      auto dh = AtErosion(x, y);
      if (dh > 0.0125f) continue;

      auto h = At(x, y) + dh;
      if (h < 0.3f) continue;

      At(x, y) = h;
    }
  }

  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Blur(4.0f);

  return *this;
}
