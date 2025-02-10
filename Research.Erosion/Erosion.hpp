
#pragma once
#include <functional>

class Erosion final
{
public:
  using At_t = ::std::function<float & (int, int)>;

public:
  Erosion & SetSize(const size_t _SizeX, const size_t _SizeY)
  {
    m_Count = 0;
    m_SizeX = _SizeX;
    m_SizeY = _SizeY;
    m_Water.resize(_SizeX * _SizeY, m_MinWaterLevel);

    return *this;
  }

  Erosion & DommainDistortion(void);
  Erosion & AxelParisThermalErosion(void);
  Erosion & TinyErodeRainfall(void);
  Erosion & Gavoronoise(void);
  Erosion & WaterErosionByEDog(void);

private:
  const float m_MinWaterLevel = 0.01f;
  const float m_MetersPerXY = 2.0f;
  const At_t At;
  ::std::vector<float> m_Water;
  size_t m_SizeX = 0, m_SizeY = 0;
  size_t m_Count = 0;

public:
  Erosion(const At_t & _At) :
    At(_At)
  {

  }
};
