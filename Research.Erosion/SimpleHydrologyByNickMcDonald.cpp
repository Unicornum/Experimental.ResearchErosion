
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

#include "..\Solution\Solution\SimpleHydrology\source\vertexpool.h"
#include "..\Solution\Solution\SimpleHydrology\source\world.h"
#include "Support.hpp"

/**
* \brief
*  ### Simple Hydrology by Nick McDonald
*
*  Источник: https://github.com/weigert/SimpleHydrology.git
*  Скорость работы: ???
*/
Erosion & Erosion::SimpleHydrologyByNickMcDonald(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  World::SEED = time(NULL);
  srand(World::SEED);

  mappool::pool<quad::cell> cellpool;
  cellpool.reserve(quad::area);

  Vertexpool<Vertex> vertexpool;
  vertexpool.reserve(quad::tilearea, quad::maparea);

  World::map.init(vertexpool, cellpool, World::SEED);

  for (int y = 0; y < quad::tilesize; y++)
  {
    for (int x = 0; x < quad::tilesize; x++)
    {
      World::map.get({ x, y })->get({ x, y })->height = At(x, y);
    }
  }

  for (int i = 0; i < 500; i++)
  {
    World::erode(quad::tilesize);
  }

  for (int y = 0; y < quad::tilesize; y++)
  {
    for (int x = 0; x < quad::tilesize; x++)
    {
      At(x, y) = World::map.height({ x, y });
    }
  }

  return *this;
}
