
#pragma once
#include <functional>
#include <filesystem>

class Support final
{
public:
  using At_t = ::std::function<float & (int, int)>;

public:
  Support & SetSize(const size_t, const size_t);
  Support & Normalize(const float = 0.0f);
  Support & Load(const ::std::filesystem::path &);
  Support & Save(const ::std::filesystem::path &);

private:
  const At_t At;
  size_t m_SizeX = 0, m_SizeY = 0;

public:
  Support(const At_t &);
};
