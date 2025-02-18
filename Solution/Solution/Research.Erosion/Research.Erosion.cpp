
#include <iostream>
#include "Support.hpp"
#include "Erosion.hpp"

#define SET_SIZE_2048
//#define SET_SIZE_1024
//#define SET_SIZE_512

#ifdef SET_SIZE_2048
const int SizeX = 2048;
const int SizeY = 2048;
const auto SourceFile = "..\\Pictures\\Source_2048x2048.tiff";
const auto ResultFile = "..\\Pictures\\Result_2048x2048.tiff";
const auto ColourFile = "..\\Pictures\\Colors_2048x2048.tiff";
#elif defined SET_SIZE_1024
const int SizeX = 1024;
const int SizeY = 1024;
const auto SourceFile = "..\\Pictures\\Source_1024x1024.tiff";
const auto ResultFile = "..\\Pictures\\Result_1024x1024.tiff";
const auto ColourFile = "..\\Pictures\\Colors_1024x1024.tiff";
#elif defined SET_SIZE_512
const int SizeX = 512;
const int SizeY = 512;
const auto SourceFile = "..\\Pictures\\Source_512x512.tiff";
const auto ResultFile = "..\\Pictures\\Result_512x512.tiff";
const auto ColourFile = "..\\Pictures\\Colors_512x512.tiff";
#else
#error "Macros SET_SIZE_xxxx not defined."
#endif

void TestErosion(void)
{
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
    .Load(SourceFile);

  Erosion(At)
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
    //.SimpleHydrologyByNickMcDonald(AtRGBA)
    //.MeanderingRiversByAxelParis(AtRGBA)
    ;

  Support(At)
    .SetSize(SizeX, SizeY)
    .Normalize(0.3f)
    //.Blur(2.0f)
    .Save(ResultFile)
    .ToRGBA(AtRGBA)
    .Save(ColourFile, AtRGBA);
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
