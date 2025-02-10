
#include <iostream>
#include "Erosion.hpp"
#include "TinyErode.h"

/**
* \brief
*  ### TinyErode
*
*  Источник: https://github.com/tay10r/TinyErode
*
*  Готовый для использования вариант (хотя совсем без танцев с бубном обойтись
*  не удалось, в документации не прописанважный для работы библиотеки момент).
*  
*  Дает довольно грубый результат, много артефактов. Также подбор коэффициентов
*  для получения пригодного результата требует невероятных усилий.
*/
Erosion & Erosion::TinyErodeRainfall(void)
{
  m_Count++;

  auto getHeight = [&](int x, int y) -> float
  {
    return ::std::max(m_MinWaterLevel, At(x, y));
  };

  auto addHeight = [&](int x, int y, float deltaHeight)
  {
    if (At(x, y) < 200.0f) return;

    At(x, y) += deltaHeight;
    At(x, y) = ::std::min(650.0f, At(x, y));
    At(x, y) = ::std::max(-1.0f, At(x, y));
  };

  auto getWater = [&](int x, int y)
  {
    return m_Water[y * m_SizeX + x];
  };

  auto addWater = [&](int x, int y, float deltaWater) -> float
  {
    /* Note: 'deltaWater' can be negative. */

    /* The function returns the new water level. It shuold not
     * fall below zero. */

    auto & oWater = m_Water[y * m_SizeX + x];
    return oWater = ::std::max(m_MinWaterLevel, oWater + deltaWater);
  };

  auto carryCapacity = [](int x, int y) { return 0.01f; };
  auto deposition = [](int x, int y) { return 0.1f; };
  auto erosion = [](int x, int y) { return 0.1f; };
  auto evaporation = [](int x, int y) { return 0.01f; };

  ::TinyErode::Simulation simulation(m_SizeX, m_SizeX);
  simulation.SetTimeStep(0.1f);
  simulation.SetMetersPerX(m_MetersPerXY);
  simulation.SetMetersPerY(m_MetersPerXY);

  for (int i = 0; i < 1024; i++)
  {
    if (i % 10 == 0)
    {
      ::std::cout << "Erosion (" << m_Count << "): "
        << i / 10 << " % " << ::std::endl;
    }

    // Determines where the water will flow.
    simulation.ComputeFlowAndTilt(getHeight, getWater);

    // Moves the water around the terrain based on the previous computed values.
    simulation.TransportWater(addWater);

    // Where the magic happens. Soil is picked up from the terrain and height
    // values are subtracted based on how much was picked up. Then the sediment
    // moves along with the water and is later deposited.
    simulation.TransportSediment(carryCapacity, deposition, erosion, addHeight);

    // Due to heat, water is gradually evaported. This will also cause soil
    // deposition since there is less water to carry soil.
    simulation.Evaporate(addWater, evaporation);
  }

  // Drops all suspended sediment back into the terrain.
  simulation.TerminateRainfall(addHeight);

  return *this;
}
