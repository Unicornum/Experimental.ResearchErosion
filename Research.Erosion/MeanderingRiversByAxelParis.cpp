
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
*  Источник: https://github.com/aparis69/Meandering-rivers
*  Скорость работы: ???
*/
Erosion & Erosion::MeanderingRiversByAxelParis(const AtRGBA_t & AtRGBA)
{
  const auto ExampleTerrainConstrained = [&](void)
  {
    const auto cellSizeXY = 2.0;

    ScalarField2D terrain(m_SizeX, m_SizeY, Box2D(Vector2(0), 10000.0));
      //Box2D(m_SizeX * cellSizeXY, m_SizeY * cellSizeXY));

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

    std::vector<Vector2> pts =
    {
      Vector2(0, -8500),
      Vector2(0, -1750),
      Vector2(0, -1000),
      Vector2(0, 0),
      Vector2(0, 1000),
      Vector2(0, 1500),
      Vector2(0, 8500)
    };

    MeanderSimulation simulation(1234 /*time(NULL)*/, terrain);
    simulation.AddChannel(Channel(pts, 50.0));
    simulation.Step(1200);
    //simulation.OutputImage("../Results/meander_terrain.ppm", m_SizeX, m_SizeY);

    const auto & Rivers = simulation.GetChannels();
    for (const auto & River : Rivers)
    {
      for (const auto & Point : River.Points())
      {
        int px = 0, py = 0;
        terrain.VertexToInteger(Point, py, px);

        for (int y = -2; y < 3; y++)
        {
          for (int x = -2; x < 3; x++)
          {
            At(px + x, py + y) -= 10.0f * (3.0f - ::std::max(abs(x), abs(y)));
            AtRGBA(px + x, py + y) = 0x00000000;
          }
        }
      }
    }
  };

  ExampleTerrainConstrained();

  return *this;
}
