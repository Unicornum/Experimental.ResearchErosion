
#include "Erosion.hpp"
#include "Random.hpp"

#include "..\Solution\Solution\Experimental.MeanderingRivers\Code\Source\quadric.cpp"
#include "..\Solution\Solution\Experimental.MeanderingRivers\Code\Source\cubic.cpp"
#include "..\Solution\Solution\Experimental.MeanderingRivers\Code\Source\cubic-curve.cpp"
#include "..\Solution\Solution\Experimental.MeanderingRivers\Code\Source\cubic-curve-set.cpp"
#include "..\Solution\Solution\Experimental.MeanderingRivers\Code\Source\meanders-channel.cpp"
#include "..\Solution\Solution\Experimental.MeanderingRivers\Code\Source\meanders-simulation.cpp"

/**
* \brief
*  ### Meandering Rivers by Axel Paris
*
*  »сточник: https://github.com/aparis69/Meandering-rivers
*  —корость работы: ???
*/
Erosion & Erosion::MeanderingRiversByAxelParis(const AtRGBA_t & AtRGBA)
{
  const auto ExampleTerrainConstrained = [&](void)
  {
    const auto cellSizeXY = 20.0; // от этого зависит дистанци€ между точками русла реки

    ScalarField2D terrain(m_SizeX, m_SizeY,
      Box2D(m_SizeX * cellSizeXY, m_SizeY * cellSizeXY));

#   ifdef _OPENMP
#   pragma omp parallel for
#   endif
    for (int y = 0; y < m_SizeY; y++)
    {
      for (int x = 0; x < m_SizeX; x++)
      {
        terrain.Set(x, y, At(x, y));
      }
    }

    //  лючевые точки русла реки
    std::vector<Vector2> pts =
    {
      terrain.IntegerToVertex(1120, 1040),
      terrain.IntegerToVertex(1050, 1350),
      terrain.IntegerToVertex(600, 1700),
      terrain.IntegerToVertex(730, 1880),
    };

    std::vector<Vector2> pts2 =
    {
      terrain.IntegerToVertex(1500, 1300),
      terrain.IntegerToVertex(1280, 1300),
      terrain.IntegerToVertex(1050, 1350),
      //terrain.IntegerToVertex(600, 1700),
      //terrain.IntegerToVertex(730, 1880),
    };

    const auto SEED = 
      1234;
      //time(NULL);

    MeanderSimulation simulation(SEED, terrain);
    simulation.AddChannel(Channel(pts, 75.0)); // ширина, в которую должны уместитьс€ изгибы русла реки?
    simulation.AddChannel(Channel(pts2, 175.0));
    simulation.Step(275); // степень извилистости русла реки

    const auto & Rivers = simulation.GetChannels();
    for (const auto & River : Rivers)
    {
      int i = 0;
      auto dRiverWidth = 10.0f / River.Points().size();

      for (const auto & Point : River.Points())
      {
        int px = 0, py = 0;
        terrain.VertexToInteger(Point, py, px);

        auto RiverWidth = 3;//::std::max(3, (int)(dRiverWidth * ++i));

        for (int y = -RiverWidth; y <= RiverWidth; y++)
        {
          for (int x = -RiverWidth; x <= RiverWidth; x++)
          {
            auto & h = At(px + x, py + y);
            auto dh = -5.0f * (RiverWidth - ::std::max(abs(x), abs(y)));

            h = (h + dh < 80.0f) ? 80.0f : h + dh;
            AtRGBA(px + x, py + y) = 0x00000000;
          }
        }
      }
    }
  };

  ExampleTerrainConstrained();

  return *this;
}
