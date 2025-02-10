
#include "Support.hpp"
#include <iostream>

Support::Support(const At_t & _At) :
  At(_At)
{

}

Support & Support::SetSize(const size_t _SizeX, const size_t _SizeY)
{
  m_SizeX = _SizeX;
  m_SizeY = _SizeY;
  return *this;
}

Support & Support::Normalize(const float _WaterLevel)
{
  auto Min = At(0, 0);
  auto Max = At(0, 0);

  for (size_t y = 0; y < m_SizeY; y++)
  {
    for (size_t x = 0; x < m_SizeX; x++)
    {
      const auto Value = At(x, y);
      Min = ::std::min(Min, Value);
      Max = ::std::max(Max, Value);
    }
  }

  if (Max == Min) return *this; // пустая карта высот

  ::std::cout << "World values min/max: " << Min << "/" << Max << ::std::endl;

  for (size_t y = 0; y < m_SizeY; y++)
  {
    for (size_t x = 0; x < m_SizeX; x++)
    {
      auto & Value = At(x, y);
      Value -= Min; // 0...(Max - Min)
      Value /= (Max - Min); // 0...1
      Value *= 1.0f + _WaterLevel; // 0...(1 + WaterLevel)
      Value -= _WaterLevel; // -WaterLevel...1
    }
  }

  return *this;
}
