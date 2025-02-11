
#include <iostream>
#include "Support.hpp"

void TestErosion(void)
{
  const int SizeX = 2048;
  const int SizeY = 2048;

  ::std::vector<float> HeightMap(SizeX * SizeY, 0.0f);

  const auto At = [&](int x, int y) -> float &
  {
    x = ::std::clamp(x, 0, SizeX - 1);
    y = ::std::clamp(y, 0, SizeY - 1);

    return HeightMap[x + y * SizeX];
  };

  Support(At)
    .SetSize(SizeX, SizeY)
    .Load("Source.tiff");

  Erosion(At)
    .SetSize(SizeX, SizeY)
    //.SetSize(512, 512)
    //.DommainDistortion()
    //.AxelParisThermalErosion()
    //.TinyErodeRainfall()
    //.TinyErodeRainfall()
    //.TinyErodeRainfall()
    //.TinyErodeRainfall()
    //.Gavoronoise()
    //.WaterErosionByEDog()
    .JobTalleHydraulicErosion()
    ;

  Support(At)
    .SetSize(SizeX, SizeY)
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
