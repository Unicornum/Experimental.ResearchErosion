
#pragma once
#include <functional>
#include <glm/glm.hpp>

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
  Erosion & JobTalleHydraulicErosion(void);

private:
  ::glm::vec3 GetNormal(const int x, const int y)
  {
    using namespace ::glm;

    auto right =  vec3(vec2(x + 1, y) / vec2(m_SizeX, m_SizeY), At(x + 1, y));
    auto left =   vec3(vec2(x - 1, y) / vec2(m_SizeX, m_SizeY), At(x - 1, y));
    auto bottom = vec3(vec2(x, y + 1) / vec2(m_SizeX, m_SizeY), At(x, y + 1));
    auto top =    vec3(vec2(x, y - 1) / vec2(m_SizeX, m_SizeY), At(x, y - 1));

    const auto Normal = ::glm::normalize(::glm::cross(right - left, bottom - top));
    return (Normal.z >= 0.0f) ? Normal : -Normal;
  }

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
