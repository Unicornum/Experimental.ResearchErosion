
#include "Erosion.hpp"
#include <algorithm>
#include <iostream>
#include"Support.hpp"

/**
* \brief
*  ### Generate Erosion 2 by Bruegge
*
*  »сточник: https://github.com/bruegge/Realtime-Procedural-Terrain-Generation
*  —корость работы: медленно
*  
*  “акое впечатление, что функци€ GenerateErosion2() просто недоработана -
*  она нигде не используетс€, обработка дает на выходе огромные числа (1e38),
*  а код содержит странные вещи вроде такого (неоднократно!):
*  if (...)
*  {
*    int muh = 0;
*    muh++;
*  }

*/
Erosion & Erosion::GenerateErosion2ByBruegge(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  return *this;
}
