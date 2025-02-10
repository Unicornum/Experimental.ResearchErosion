
#pragma once
#include <functional>
#include <filesystem>

class Support final
{
public:
  using RGBA_t = uint32_t;
  using At_t = ::std::function<float & (int, int)>;
  using AtRGBA_t = ::std::function<RGBA_t & (int, int)>;

public:
  Support & SetSize(const size_t, const size_t);
  Support & Normalize(const float, float * = nullptr);
  Support & Blur(const float);
  Support & ToRGBA(const AtRGBA_t &);
  Support & Load(const ::std::filesystem::path &);
  Support & Save(const ::std::filesystem::path &);
  Support & Save(const ::std::filesystem::path &, const AtRGBA_t &);

private:
  const At_t At;
  size_t m_SizeX = 0, m_SizeY = 0;

public:
  Support(const At_t &);
};
