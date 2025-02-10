
#include <iostream>
#include "Support.hpp"

void TestErosion(void)
{
  const int SizeX = 2048;
  const int SizeY = 2048;

  ::std::vector<float> HeightMap(SizeX * SizeY, 0.0f);

  const auto At = [&](int x, int y) -> float &
  {
    x = ::std::max(0, ::std::min(SizeX - 1, x));
    y = ::std::max(0, ::std::min(SizeY - 1, y));

    return HeightMap[x + y * SizeX];
  };

  Support(At)
    .SetSize(SizeX, SizeY)
    .Load("Source.tiff")
    .Normalize(0.3f)
    .Save("Result.tiff");
}

int main()
{
  try
  {
    TestErosion();
  }
  catch (::std::exception & _Ex)
  {
    ::std::cout << "error: " << _Ex.what() << ::std::endl;
  }
}
