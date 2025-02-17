
#include "Erosion.hpp"
#include <iostream>
#include <deque>

#define glEnableVertexAttribArray //
#define glVertexAttribFormat //
#define glVertexBindingDivisor //
#define glVertexAttribBinding //
#define glBindVertexBuffer //
#define glGenVertexArrays //
#define glBindVertexArray //
#define glGenBuffers //
#define glUnmapBuffer //
#define glDeleteBuffers //
#define glDeleteVertexArrays //
#define GL_MAP_WRITE_BIT 0
#define GL_MAP_PERSISTENT_BIT 0
#define GL_MAP_COHERENT_BIT 0
#define GL_ARRAY_BUFFER 0
#define glBindBuffer //
#define GL_ELEMENT_ARRAY_BUFFER 0
#define GL_STATIC_DRAW 0
#define glBufferData //
#define GL_DRAW_INDIRECT_BUFFER 0
#define glMultiDrawElementsIndirect //
#define GL_FLOAT 0
#define GL_FALSE 0
using GLuint = uint32_t;
using GLenum = int;
using GLbitfield = int;
#define GL_UNSIGNED_INT 0
#define GL_TRIANGLES 0
#define glBufferStorage //
#define glMapBufferRange //

#define emplace_front(a, b) \
  emplace_front(buf<T>{ a, b })

#define EXPERIMENTAL_TILE_SIZE 2048

#include "..\Solution\Solution\Experimental.SimpleHydrology\source\vertexpool.h"
#include "..\Solution\Solution\Experimental.SimpleHydrology\source\world.h"
#include "Support.hpp"

/**
* \brief
*  ### Simple Hydrology by Nick McDonald
*
*  »сточник: https://github.com/weigert/SimpleHydrology.git
*  —корость работы: медленно
*  
*   орректо работает только Release верси€, Debug выдает навыходе огромные
*  значени€.
*/
Erosion & Erosion::SimpleHydrologyByNickMcDonald(const AtRGBA_t & AtRGBA)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.3f);

  World::SEED = 12345;//time(NULL);
  srand(World::SEED);

  mappool::pool<quad::cell> cellpool;
  cellpool.reserve(quad::area);

  Vertexpool<Vertex> vertexpool;
  vertexpool.reserve(quad::tilearea, quad::maparea);

  World::map.init(vertexpool, cellpool, World::SEED);

  for (int y = 0; y < m_SizeY; y++)
  {
    for (int x = 0; x < m_SizeX; x++)
    {
      World::map.get({ x, y })->get({ x, y })->height = At(x, y);
    }
  }

  const int Count = 10;

  for (int i = 0; i < Count; i++)
  {
    if (i % ::std::max(1, Count / 10) == 0)
    {
      ::std::cout << "Erode " << i / ::std::max(1, Count / 10) << "0 % " << ::std::endl;
    }

    World::erode(m_SizeX * 10); // чем больше число, тем более густой будет речна€ сеть
  }

  for (int y = 0; y < m_SizeY; y++)
  {
    for (int x = 0; x < m_SizeX; x++)
    {
      auto h = World::map.height({ x, y }); // высота поверхности
      auto w = World::map.discharge({ x, y }); // уровень воды

      At(x, y) = h;

      if (w > h)
      {
        auto node = World::map.get({ x, y });
        auto cell = node->get({ x, y });
        float mx = cell->momentumx;
        float my = cell->momentumy;
        AtRGBA(x, y) =
          (0x00 << 24) +
          (0x7F << 16) +
          ((int)(127.0f * (1.0f + erf(my))) << 8) +
          ((int)(127.0f * (1.0f + erf(mx))) << 0);
      }
      else
      {
        //AtRGBA(x, y) = 0xFFFFFFFF;
      }
    }
  }

  return *this;
}
