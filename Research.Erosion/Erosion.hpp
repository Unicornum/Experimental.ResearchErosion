
#pragma once
#include <functional>
#include <glm/glm.hpp>

class Erosion final
{
public:
  using RGBA_t = uint32_t;
  using At_t = ::std::function<float & (int, int)>;
  using AtRGBA_t = ::std::function<RGBA_t & (int, int)>;

public:
  Erosion & DommainDistortion(void);
  Erosion & ThermalErosionByAxelParis(void);
  Erosion & TinyErode(void);
  Erosion & Gavoronoise(void);
  Erosion & WaterErosionByEDog(void);
  Erosion & HydraulicErosionByJobTalle(void);
  Erosion & PerturbByDavidJorna(void);
  Erosion & ThermalErosionByDavidJorna(void);
  Erosion & FastErosionByDavidJorna(void);
  Erosion & HydraulicErosionByDavidJorna(void);
  Erosion & MultiScaleErosion(void);
  Erosion & TerrainErosionByAdrianBlumer(void);
  Erosion & SimpleErosionByNickMcDonald(void);
  Erosion & SimpleHydrologyByNickMcDonald(const AtRGBA_t &);
  Erosion & MeanderingRiversByAxelParis(const AtRGBA_t &);

public:
  Erosion & SetSize(const size_t _SizeX, const size_t _SizeY)
  {
    m_SizeX = _SizeX;
    m_SizeY = _SizeY;

    return *this;
  }

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
  const At_t At;
  size_t m_SizeX = 0, m_SizeY = 0;

public:
  Erosion(const At_t & _At) :
    At(_At)
  {

  }
};
