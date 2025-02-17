
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

  ::std::vector<uint32_t> Colors(SizeX * SizeY, 0xFFFFFFFF);

  const auto AtRGBA = [&](int x, int y) -> uint32_t &
  {
    x = ::std::clamp(x, 0, SizeX - 1);
    y = ::std::clamp(y, 0, SizeY - 1);

    return Colors[x + y * SizeX];
  };

  Support(At)
    .SetSize(SizeX, SizeY)
    .Load("Source.tiff")
    //.Normalize(0.0f)
    ;

  Erosion(At)
    //.SetSize(512, 512)
    //.SetSize(1024, 1024)
    .SetSize(SizeX, SizeY)
    //.DommainDistortion()
    //.ThermalErosionByAxelParis()
    //.TinyErode()
    //.Gavoronoise()
    //.ThermalErosionByAxelParis().Gavoronoise() // *
    //.WaterErosionByEDog() // *
    //.HydraulicErosionByJobTalle() // *
    //.PerturbByDavidJorna()
    //.ThermalErosionByDavidJorna()
    //.FastErosionByDavidJorna()
    //.HydraulicErosionByDavidJorna()
    //.MultiScaleErosion() // *
    //.TerrainErosionByAdrianBlumer()
    //.SimpleErosionByNickMcDonald()
    .SimpleHydrologyByNickMcDonald(AtRGBA)
    //.MeanderingRivers()
    ;

  Support(At)
    .SetSize(SizeX, SizeY)
    .Normalize(0.3f)
    //.Blur(2.0f)
    .Save("Result.tiff")
    .ToRGBA(AtRGBA)
    .Save("Colors.tiff", AtRGBA);
    ;
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
