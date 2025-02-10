
#pragma once
#include <random>
#include <iostream>

static const auto Random = [](const auto _From, const auto _To)
{
  static ::std::mt19937_64 Generator{ ::std::random_device{}() };

  if constexpr (::std::is_integral_v<decltype(_From)>)
  {
    return ::std::uniform_int_distribution<size_t>{ _From, _To }(Generator);
  }
  else if constexpr (::std::is_floating_point_v <decltype(_From)>)
  {
    return ::std::uniform_real_distribution<float>{ _From, _To }(Generator);
  }
  else
  {
    throw ::std::runtime_error{ "Random: unexpected type." };
  }
};

class Random_t final
{
public:
  template<class T>
  T Get(const T _From, const T _To)
  {
    if constexpr (::std::is_integral_v<decltype(_From)>)
    {
      return ::std::uniform_int_distribution<size_t>{ _From, _To }(m_Generator);
    }
    else if constexpr (::std::is_floating_point_v <decltype(_From)>)
    {
      return ::std::uniform_real_distribution<float>{ _From, _To }(m_Generator);
    }
    else
    {
      throw ::std::runtime_error{ "Random: unexpected type." };
    }
  }

private:
  const uint32_t m_Seed;
  ::std::mt19937_64 m_Generator;

public:
  explicit Random_t(const uint32_t _Seed = 0) :
    m_Seed((_Seed == 0) ? ::std::random_device{}() : _Seed),
    m_Generator(m_Seed)
  {
  }
};
