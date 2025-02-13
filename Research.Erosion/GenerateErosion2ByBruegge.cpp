
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
*/
Erosion & Erosion::GenerateErosion2ByBruegge(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  // ����� �����������, ��� ������� GenerateErosion2() ������ ������������,
  // ��������� ���� �� ������ �������� ����� (1e38), � ��� �������� ��������
  // ���� ����� ������:
  //if (x == 2 && y == 2 && dTotal == 0.0f)
  //{
  //  int muh = 0;
  //  muh++;
  //}

  return *this;
}
