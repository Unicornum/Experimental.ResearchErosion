
#include <iostream>
#include "Erosion.hpp"
#include "Random.hpp"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Support.hpp"

/**
* \brief
*  ### Water Erosion by E-DOG
*
*  Источник: https://ranmantaru.com/blog/2011/10/08/water-erosion-on-heightmap-terrain/
*  
*  Работает быстро, дает вполне убедительный результат. При этом требует
*  дополнительной обработки и блюра, т.к. генерирует много мусора.
*/
Erosion & Erosion::WaterErosionByEDog(void)
{
  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  Random_t oRandom;

  const int HMAP_SIZE = ::std::min(m_SizeX, m_SizeY);
  const auto game_rnd = [&](void) { return (int)((HMAP_SIZE - 1) * oRandom.Get(0.0f, 1.0f)); };
  const auto frnd = [&](void) { return oRandom.Get(0.0f, 1.0f); };
  using vec4f = ::glm::vec4;
  const auto zero4f = [](void) { return ::glm::vec4{ 0.0f }; };
  const auto TWOPI = 2.0f * ::glm::pi<float>();
  const auto intfloorf = [&](float f) { return (int)(/*HMAP_SIZE * */floor(f)); };
  const auto maxval = [](auto v1, auto v2) { return ::std::max(v1, v2); };
  const auto minval = [](auto v1, auto v2) { return ::std::min(v1, v2); };
  using Point2 = ::glm::vec2;

  unsigned iterations = m_SizeX * m_SizeY / 125;

  // Kq=10; Kw=0.001f; Kr=0.9f; Kd=0.02f; Ki=0.1f; minSlope=0.05f; g=20;

  float 
    Kq = 10.0f, 
    Kw = 0.001f, 
    Kr = 0.9f, 
    Kd = 0.002f, 
    Ki = 0.01f, 
    minSlope = 0.05f, 
    g = 9.8f, 
    Kg = g * 1.0f;

  ::std::vector<::glm::vec2> erosion(m_SizeX * m_SizeY, { 0.0f, 0.0f });
  ::std::vector<float> hmap(m_SizeX * m_SizeY, 0.0f);

  static const unsigned MAX_PATH_LEN = HMAP_SIZE * 4;

  const auto HMAP_INDEX = [&](int x, int y)
  {
    int index = x + y * m_SizeX;
    if (index < 0) return 0;
    if (index >= m_SizeX * m_SizeY) return (int)(m_SizeX * m_SizeY);
    return index;
  };

  const auto HMAP = [&](int x, int y) { return At(x, y); };

#define DEPOSIT_AT(X, Z, W) \
  { \
    float delta=ds*(W); \
    erosion[HMAP_INDEX((X), (Z))].y+=delta; \
    hmap   [HMAP_INDEX((X), (Z))]  +=delta; \
    /*params.deposit(scolor, surface[HMAP_INDEX((X), (Z))], delta); */ \
  }

#if 1
#define DEPOSIT(H) \
      DEPOSIT_AT(xi  , zi  , (1-xf)*(1-zf)) \
      DEPOSIT_AT(xi+1, zi  ,    xf *(1-zf)) \
      DEPOSIT_AT(xi  , zi+1, (1-xf)*   zf ) \
      DEPOSIT_AT(xi+1, zi+1,    xf *   zf ) \
      (H)+=ds;
#else
#define DEPOSIT(H) \
      DEPOSIT_AT(xi  , zi  , 0.25f) \
      DEPOSIT_AT(xi+1, zi  , 0.25f) \
      DEPOSIT_AT(xi  , zi+1, 0.25f) \
      DEPOSIT_AT(xi+1, zi+1, 0.25f) \
      (H)+=ds;
#endif

  for (unsigned iter = 0; iter < iterations; ++iter)
  {
    int xi = game_rnd();
    int zi = game_rnd();

    float xp = xi, zp = zi;
    float xf = 0, zf = 0;

    float h = HMAP(xi, zi);
    float s = 0, v = 0, w = 1;

    float h00 = h;
    float h10 = HMAP(xi + 1, zi);
    float h01 = HMAP(xi, zi + 1);
    float h11 = HMAP(xi + 1, zi + 1);

    float dx = 0, dz = 0;

    unsigned numMoves = 0;
    for (; numMoves < MAX_PATH_LEN; ++numMoves)
    {
      // calc gradient
      float gx = h00 + h01 - h10 - h11;
      float gz = h00 + h10 - h01 - h11;
      //== better interpolated gradient?

      // calc next pos
      dx = (dx - gx) * Ki + gx;
      dz = (dz - gz) * Ki + gz;

      float dl = sqrtf(dx * dx + dz * dz);
      if (dl <= FLT_EPSILON)
      {
        // pick random dir
        float a = frnd() * TWOPI;
        dx = cosf(a);
        dz = sinf(a);
      }
      else
      {
        dx /= dl;
        dz /= dl;
      }

      float nxp = xp + dx;
      float nzp = zp + dz;

      // sample next height
      int nxi = intfloorf(nxp);
      int nzi = intfloorf(nzp);
      float nxf = nxp - nxi;
      float nzf = nzp - nzi;

      float nh00 = HMAP(nxi, nzi);
      float nh10 = HMAP(nxi + 1, nzi);
      float nh01 = HMAP(nxi, nzi + 1);
      float nh11 = HMAP(nxi + 1, nzi + 1);

      float nh = (nh00 * (1 - nxf) + nh10 * nxf) * (1 - nzf) + (nh01 * (1 - nxf) + nh11 * nxf) * nzf;

      // if higher than current, try to deposit sediment up to neighbour height
      if (nh >= h)
      {
        float ds = (nh - h) + 0.001f;

        if (ds >= s)
        {
          // deposit all sediment and stop
          ds = s;
          DEPOSIT(h)
            s = 0;
          break;
        }

        DEPOSIT(h)
          s -= ds;
        v = 0;
      }

      // compute transport capacity
      float dh = h - nh;
      float slope = dh;
      //float slope=dh/sqrtf(dh*dh+1);

      float q = maxval(slope, minSlope) * v * w * Kq;

      // deposit/erode (don't erode more than dh)
      float ds = s - q;
      if (ds >= 0)
      {
        // deposit
        ds *= Kd;
        //ds=minval(ds, 1.0f);

        DEPOSIT(dh)
          s -= ds;
      }
      else
      {
        // erode
        ds *= -Kr;
        ds = minval(ds, dh * 0.99f);

#define ERODE(X, Z, W) \
        { \
          float delta=ds*(W); \
          hmap             [HMAP_INDEX((X), (Z))]-=delta; \
          Point2 &e=erosion[HMAP_INDEX((X), (Z))]; \
          float r=e.x, d=e.y; \
          if (delta<=d) d-=delta; \
          else { r+=delta-d; d=0; } \
          e.x=r; e.y=d; \
        }

#if 1
        for (int z = zi - 1; z <= zi + 2; ++z)
        {
          float zo = z - zp;
          float zo2 = zo * zo;

          for (int x = xi - 1; x <= xi + 2; ++x)
          {
            float xo = x - xp;

            float w = 1 - (xo * xo + zo2) * 0.25f;
            if (w <= 0) continue;
            w *= 0.1591549430918953f;

            ERODE(x, z, w)
          }
        }
#else
        ERODE(xi, zi, (1 - xf) * (1 - zf))
          ERODE(xi + 1, zi, xf * (1 - zf))
          ERODE(xi, zi + 1, (1 - xf) * zf)
          ERODE(xi + 1, zi + 1, xf * zf)
#endif

          dh -= ds;

#undef ERODE

        s += ds;
      }

      // move to the neighbour
      v = sqrtf(v * v + Kg * dh);
      w *= 1 - Kw;

      xp = nxp; zp = nzp;
      xi = nxi; zi = nzi;
      xf = nxf; zf = nzf;

      h = nh;
      h00 = nh00;
      h10 = nh10;
      h01 = nh01;
      h11 = nh11;
    }
  }

#undef DEPOSIT
#undef DEPOSIT_AT

#   ifdef _OPENMP
#   pragma omp parallel for
#   endif
  for (int y = 0; y < m_SizeY; y++)
  {
    for (int x = 0; x < m_SizeX; x++)
    {
      // убираем мусор
      auto dh = 50.0f * hmap[HMAP_INDEX(x, y)];
      if (dh > 0.0125f) continue;

      auto h = At(x, y) + dh;
      if (h < 0.3f) continue;

      At(x, y) = h;
    }
  }

  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Blur(3.0f);

  return *this;
}
