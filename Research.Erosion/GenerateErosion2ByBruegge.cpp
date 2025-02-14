
#include "Erosion.hpp"
#include <algorithm>
#include <iostream>
#include"Support.hpp"

/**
* \brief
*  ### Generate Erosion 2 by Bruegge
*
*  ��������: https://github.com/bruegge/Realtime-Procedural-Terrain-Generation
*  �������� ������: ��������
*  
*  ����� �����������, ��� ������� GenerateErosion2() ������ ������������ -
*  ��� ����� �� ������������, ��������� ���� �� ������ �������� ����� (1e38),
*  � ��� �������� �������� ���� ����� ������ (������������!):
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
