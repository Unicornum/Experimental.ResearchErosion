
#include "Erosion.hpp"
#include <algorithm>
#include <iostream>
#include"Support.hpp"

/**
* \brief
*  ### Generate Erosion 2 by Bruegge
*
*  Источник: https://github.com/bruegge/Realtime-Procedural-Terrain-Generation
*  Скорость работы: медленно
*/
Erosion & Erosion::GenerateErosion2ByBruegge(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  // Такое впечатление, что функция GenerateErosion2() просто недоработана,
  // обработка дает на выходе огромные числа (1e38), а код содержит странные
  // вещи вроде такого:
  //if (x == 2 && y == 2 && dTotal == 0.0f)
  //{
  //  int muh = 0;
  //  muh++;
  //}

  return *this;
}
