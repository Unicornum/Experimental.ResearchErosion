
#include "Erosion.hpp"
#include <iostream>
#include <glm/glm.hpp>
//#include "Random.hpp"
#include "Support.hpp"

/**
* \brief
*  ### Multi-scale Erosion
*
*  Источник: https://github.com/H-Schott/MultiScaleErosion
*  Скорость работы: медленно
*  
*  Исходный вариант был оптимизирован для карты высот 512х512, поэтому он
*  идеально подходит для исходной карты высот, уменьшенной до этого размера
*  (обработка отдельно участка не помогает, т.е. дело именно в уклоне
*  поверхности), а для карты большего размера нужно в Slope() добавить
*  домножение на коэффициент > 1 bk увеличивать количество проходов.
*/
Erosion & Erosion::MultiScaleErosion(void)
{
  // Код адаптирован из https://www.shadertoy.com/view/XX2XWD

  using namespace ::glm;

  Support(At)
    .SetSize(m_SizeX, m_SizeY)
    .Normalize(0.0f);

  const auto Count = 600; const auto kSlope = 1.0f; // первоначальный вариант
  //const auto Count = 100; const auto kSlope = 5.0f; // тоже рабочие, но
  //const auto Count = 50; const auto kSlope = 6.0f;  // слабый эффект DEPOSITION

  float flow_routing_exp = 1.3f;

  vec2 neighbor[8] = 
  { 
    vec2(1, 0), vec2(1, 1), vec2(0, 1), vec2(-1, 1),
    vec2(-1, 0), vec2(-1, -1), vec2(0, -1), vec2(1, -1)
  };

  ::std::vector<vec4> bufferA(m_SizeX * m_SizeY);
  ::std::vector<vec4> bufferB(m_SizeX * m_SizeY);

  auto & source = bufferA;
  auto & result = bufferB;

  const auto Index = [&](vec2 p)
  {
    auto index = (int)p.x + (int)p.y * (int)m_SizeX;
    return ::std::clamp(index, 0, (int)m_SizeX * (int)m_SizeY - 1);
  };

  const auto Height = [&](vec2 p)
  {
    return source[Index(p)].x;
  };

  const auto Flow = [&](vec2 p)
  {
    return source[Index(p)].y;
  };

  const auto Sed = [&](vec2 p)
  {
    return source[Index(p)].z;
  };

  const auto Depo = [&](vec2 p)
  {
    return source[Index(p)].w;
  };

  const auto Slope = [&](vec2 p, vec2 q)
  {
    return kSlope * (Height(p) - Height(q)) / length(p - q);
  };

  const auto Steepest = [&](vec2 p, vec2 & q)
  {
    float max_slope = 0.;
    q = p;

    // look around p
    for (int i = 0; i < 8; i++)
    {
      vec2 pp = p + neighbor[i];
      if (Height(p) < Height(pp)) continue;
      float s = Slope(p, pp);
      if (s > max_slope)
      {
        max_slope = s;
        q = pp;
      }
    }

    return max_slope;
  };

  const auto Hash = [&](vec3 p)
  {
    p.x = float(int(p.x) % 20) + fract(p.x);
    p = fract(p * 0.3199f + 0.152f);
    p *= 17.0f;
    return fract(p.x * p.y * p.z * (p.x + p.y + p.z));
  };

  const auto Noise = [&](vec3 p)
  {
    vec3 i = floor(p);
    vec3 f = fract(p);

    f = f * f * (3.0f - 2.0f * f);

    return mix(mix(mix(Hash(i + vec3(0, 0, 0)),
      Hash(i + vec3(1, 0, 0)), f.x),
      mix(Hash(i + vec3(0, 1, 0)),
      Hash(i + vec3(1, 1, 0)), f.x), f.y),
      mix(mix(Hash(i + vec3(0, 0, 1)),
      Hash(i + vec3(1, 0, 1)), f.x),
      mix(Hash(i + vec3(0, 1, 1)),
      Hash(i + vec3(1, 1, 1)), f.x), f.y), f.z);
  };

  const auto Turbulence = [&](vec3 p)
  {
    float t = 2.f * Noise(0.5f * p) + Noise(p) + 0.5f * Noise(2.f * p) + 0.1f * Noise(10.f * p);
    return t / (2.f + 1.f + 0.5f + 0.1f);
  };

  const auto Softness = [&](vec2 p)
  {
    return Turbulence(0.1f * vec3(p, -12.365f));
  };

  const auto GetThermalDelta = [&](vec2 p, float s_max)
  {
    int up = 0;
    int down = 0;
    // look around p
    for (int i = 0; i < 8; i++)
    {
      vec2 q = p + neighbor[i];
      float s = Slope(p, q);
      if (s > s_max)
      {
        down++;
      }
      else if (s < -s_max)
      {
        up++;
      }
    }

    return up - down;
  };

  const auto GetFlowWeight = [&](vec2 p, vec2 q)
  {

    float weight = 0.;
    float total_weight = 0.;

    // look around p
    for (int i = 0; i < 8; i++)
    {
      vec2 pp = p + neighbor[i];
      if (Height(p) < Height(pp)) continue;
      float s = pow(Slope(p, pp), flow_routing_exp);
      total_weight += s;
      if (pp == q) weight = s;
    }

    if (total_weight == 0.f) return 0.f;
    return weight / total_weight;
  };

  const auto GetFlow = [&](vec2 p)
  {
    float flow = 0.f;
    // look around p
    for (int i = 0; i < 8; i++)
    {
      vec2 q = p + neighbor[i];
      if (Height(p) > Height(q)) continue;
      float weight = GetFlowWeight(q, p);
      flow += Flow(q) * weight;
    }

    return flow;
  };

  const auto GetSedFlow = [&](vec2 p)
  {
    float sed_flow = 0.f;
    // look around p
    for (int i = 0; i < 8; i++)
    {
      vec2 q = p + neighbor[i];
      if (Height(p) > Height(q)) continue;
      float weight = GetFlowWeight(q, p);
      sed_flow += Sed(q) * weight;
    }

    return sed_flow;
  };

  for (auto iFrame = 0; iFrame < Count; iFrame++)
  {
    ::std::cout << "Erosion pass " << iFrame << "/" << Count << ::std::endl;

#   pragma omp parallel for
    for (auto y = 0; y < m_SizeY; y++)
    {
      for (auto x = 0; x < m_SizeX; x++)
      {
        vec2 p = vec2(x, y);// / vec2(m_SizeX, m_SizeY);

        if (iFrame < 1)
        {
          // init Height
          //result.x = Terrain(p);
          result[Index(p)].x = At(x, y);

          // init Flow
          //result.y = 1.0f;
          result[Index(p)].y = 1.0f;

          // init Sed
          //result.z = 0.0f;
          result[Index(p)].z = 0.0f;

          // init Depo
          //result.w = 0.0f;
          result[Index(p)].w = 0.0f;

          continue;
        }
        else if (iFrame < 2 * Count / 5)
        {
          // SPE
          vec2 steepest_p;
          float s = Steepest(p, steepest_p);
          float steepest_h = Height(steepest_p);
          float spe = 0.4f * min(3.f, pow(Flow(p), 0.8f) * s * s) * Softness(p);
          result[Index(p)].x = max(steepest_h, Height(p) - spe);
          result[Index(p)].z = Sed(p);
          result[Index(p)].w = max(0.f, Depo(p) - (Height(p) - result[Index(p)].x));
        }
        else if (iFrame < 2 * Count / 3)
        {
          // THERMAL
          int thermal_delta = GetThermalDelta(p, (0.04f + 0.02f * Softness(p)));
          result[Index(p)].x = Height(p) + 0.003f * float(thermal_delta);
          result[Index(p)].z = Sed(p);
          result[Index(p)].w = Depo(p);
        }
        else
        {
          // DEPOSITION
          vec2 steepest_p;
          float s = Steepest(p, steepest_p);
          float spe = pow(Flow(p), 0.8f) * s * s;
          float new_sed = GetSedFlow(p);
          float depo_index = max(0.f, new_sed - 0.7f * spe);
          float depo = 2.0f * min(new_sed, 0.01f * depo_index);
          result[Index(p)].x = Height(p) + depo;
          result[Index(p)].z = new_sed + 0.1f * spe - depo;
          result[Index(p)].w = Depo(p) + depo;
        }

        // update Flow
        result[Index(p)].y = 1.f + GetFlow(p);
      }
    }

    ::std::swap(source, result);
  }

# pragma omp parallel for
  for (auto y = 0; y < m_SizeY; y++)
  {
    for (auto x = 0; x < m_SizeX; x++)
    {
      At(x, y) = result[x + y * m_SizeX].x;
    }
  }

  return *this;
}
