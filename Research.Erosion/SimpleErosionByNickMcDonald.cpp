
#include "Erosion.hpp"
#include <iostream>
#include "Support.hpp"

/**
* \brief
*  ### Simple Erosion by Nick McDonald
*
*  Источник: https://github.com/weigert/SimpleErosion
*  Скорость работы: ???
*/
Erosion & Erosion::SimpleErosionByNickMcDonald(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  //Particle Properties
  float dt = 3.8f * 1.2;
  float density = 1.0;  //This gives varying amounts of inertia and stuff...
  float evapRate = 0.001;
  float depositionRate = 0.1;
  float minVol = 0.01;
  float friction = 0.05;

  double scale = 650.0;                  //"Physical" Height scaling of the map
  double heightmap[2049][2049] = { 0.0 };

  for (int y = 0; y < m_SizeY; y++)
  {
    for (int x = 0; x < m_SizeX; x++)
    {
      heightmap[x][y] = At(x, y);
    }
  }

  const auto dim = glm::vec2(m_SizeX, m_SizeY);

  srand(time(NULL));

  struct Particle
  {
    //Construct Particle at Position
    Particle(glm::vec2 _pos) { pos = _pos; }

    glm::vec2 pos;
    glm::vec2 speed = glm::vec2(0.0);

    float volume = 1.0;   //This will vary in time
    float sediment = 0.0; //Fraction of Volume that is Sediment!
  };

  const auto surfaceNormal = [&](int i, int j)
  {
    /*
      Note: Surface normal is computed in this way, because the square-grid surface is meshed using triangles.
      To avoid spatial artifacts, you need to weight properly with all neighbors.
    */

    glm::vec3 n = glm::vec3(0.15) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i + 1][j]), 1.0, 0.0));  //Positive X
    n += glm::vec3(0.15) * glm::normalize(glm::vec3(scale * (heightmap[i - 1][j] - heightmap[i][j]), 1.0, 0.0));  //Negative X
    n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale * (heightmap[i][j] - heightmap[i][j + 1])));    //Positive Y
    n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale * (heightmap[i][j - 1] - heightmap[i][j])));  //Negative Y

    //Diagonals! (This removes the last spatial artifacts)
    n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i + 1][j + 1]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i + 1][j + 1]) / sqrt(2)));    //Positive Y
    n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i + 1][j - 1]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i + 1][j - 1]) / sqrt(2)));    //Positive Y
    n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i - 1][j + 1]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i - 1][j + 1]) / sqrt(2)));    //Positive Y
    n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[i][j] - heightmap[i - 1][j - 1]) / sqrt(2), sqrt(2), scale * (heightmap[i][j] - heightmap[i - 1][j - 1]) / sqrt(2)));    //Positive Y

    return n;
  };

  for (int i = 0; i < 100000; i++)
  {
    ::std::cout << "Erosion pass " << i << "/" << 100000 << ::std::endl;

    //Spawn New Particle
    glm::vec2 newpos = glm::vec2(rand() % (int)m_SizeX, rand() % (int)m_SizeY);
    Particle drop(newpos);

    //As long as the droplet exists...
    while (drop.volume > minVol)
    {
      glm::ivec2 ipos = drop.pos;                   //Floored Droplet Initial Position
      glm::vec3 n = surfaceNormal(ipos.x, ipos.y);  //Surface Normal at Position

      //Accelerate particle using newtonian mechanics using the surface normal.
      drop.speed += dt * glm::vec2(n.x, n.z) / (drop.volume * density);//F = ma, so a = F/m
      drop.pos += dt * drop.speed;
      drop.speed *= (1.0 - dt * friction);       //Friction Factor

      /*
        Note: For multiplied factors (e.g. friction, evaporation)
        time-scaling is correctly implemented like above.
      */

      //Check if Particle is still in-bounds
      if (!glm::all(glm::greaterThanEqual(drop.pos, glm::vec2(0))) ||
        !glm::all(glm::lessThan(drop.pos, dim))) break;

     //Compute sediment capacity difference
      float maxsediment = drop.volume * glm::length(drop.speed) * (heightmap[ipos.x][ipos.y] - heightmap[(int)drop.pos.x][(int)drop.pos.y]);
      if (maxsediment < 0.0) maxsediment = 0.0;
      float sdiff = maxsediment - drop.sediment;

      //Act on the Heightmap and Droplet!
      drop.sediment += dt * depositionRate * sdiff;
      heightmap[ipos.x][ipos.y] -= dt * drop.volume * depositionRate * sdiff;

      //Evaporate the Droplet (Note: Proportional to Volume! Better: Use shape factor to make proportional to the area instead.)
      drop.volume *= (1.0 - dt * evapRate);
    }
  }

  for (int y = 0; y < m_SizeY; y++)
  {
    for (int x = 0; x < m_SizeX; x++)
    {
      At(x, y)= heightmap[x][y];
    }
  }

  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Blur(3.0f);

  return *this;
}
