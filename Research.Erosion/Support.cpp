
#include "Support.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <tiffio.h>
#include "fast_gaussian_blur_template.h"
#include "Erosion.hpp"

Support::Support(const At_t & _At) :
  At(_At)
{

}

Support & Support::SetSize(const size_t _SizeX, const size_t _SizeY)
{
  m_SizeX = _SizeX;
  m_SizeY = _SizeY;
  return *this;
}

Support & Support::Normalize(const float _WaterLevel, float * _pMaxValue)
{
  auto Min = At(0, 0);
  auto Max = At(0, 0);

  for (size_t y = 0; y < m_SizeY; y++)
  {
    for (size_t x = 0; x < m_SizeX; x++)
    {
      const auto Value = At(x, y);
      Min = ::std::min(Min, Value);
      Max = ::std::max(Max, Value);
    }
  }

  if (Max == Min) return *this; // пустая карта высот
  if (_pMaxValue != nullptr) *_pMaxValue = Max;

  ::std::cout << "World values min/max: " << Min << "/" << Max << ::std::endl;

  for (size_t y = 0; y < m_SizeY; y++)
  {
    for (size_t x = 0; x < m_SizeX; x++)
    {
      auto & Value = At(x, y);
      Value -= Min; // 0...(Max - Min)
      Value /= (Max - Min); // 0...1
      Value *= 1.0f + _WaterLevel; // 0...(1 + WaterLevel)
      Value -= _WaterLevel; // -WaterLevel...1
    }
  }

  return *this;
}

Support & Support::Blur(const float _Sigma)
{
  // https://github.com/bfraboni/FastGaussianBlur

  auto * old_image = &At(0, 0);
  ::std::vector<float> new_image(m_SizeX * m_SizeY);
  auto * new_image_data = new_image.data();

  fast_gaussian_blur(old_image, new_image_data, m_SizeX, m_SizeY, 1, _Sigma);

  memcpy(old_image, ::std::data(new_image), ::std::size(new_image) * sizeof(float));

  return *this;
}

Support & Support::ToRGBA(const AtRGBA_t & _AtRGBA)
{
  using namespace ::glm;

  //Color inputs matching https://github.com/dandrino/terrain-erosion-3-ways
  const ::std::vector<vec4> colors =
  {
    { 0.0f, 0.15f, 0.3f, 0.15f },
    { 0.3f, 0.3f, 0.45f, 0.3f  },
    { 0.65f, 0.5f, 0.5f, 0.35f  },
    { 0.7f, 0.4f, 0.36f, 0.33f },
    { 0.9f, 1.1f, 1.1f, 1.1f   },
    { -1.0f, 0.25f, 0.41f, 0.88f },
  };

  const auto ColorsYZW = [&](int i)
  {
    return vec3(colors[i].y, colors[i].z, colors[i].w);
  };

  //const auto lerp = [](float a, float b, float weight)
  //{
  //  return a * weight + b * (1.0f - weight);
  //};

  const auto ABGR = [](const uint8_t _A, const uint8_t _B, const uint8_t _G, const uint8_t _R)
  {
    return (_A << 24) + (_B << 16) + (_G << 8) + (_R << 0);
  };

  auto sundir = normalize(vec3(2.0f, 1.0f, 0.5f));

  Erosion oErosion(At);
  oErosion.SetSize(m_SizeX, m_SizeY);

#   ifdef _OPENMP
#   pragma omp parallel for
#   endif
  for (int y = 0; y < m_SizeY; y++)
  {
    for (int x = 0; x < m_SizeX; x++)
    {
      auto h = At(x, y);
      auto norm = oErosion.GetNormal(x, y);
      auto sunlight = 1.0f - dot(sundir, norm);

      auto outcol = ColorsYZW(4);

      if (h < colors[0].x)
      {
        auto dh = ::std::max(0.0f, (1.0f + 6.0f * h));
        outcol = ::glm::mix(0.5f * ColorsYZW(5), ColorsYZW(5), dh);
      }
      else
      {
        for (int i = 1; i < colors.size(); i++)
        {
          if (h < colors[i].x)
          {
            auto dh = (h - colors[i - 1].x) / (colors[i].x - colors[i - 1].x);
            outcol = ::glm::mix(ColorsYZW(i - 1), ColorsYZW(i), dh);
            break;
          }
        }

        outcol *= sunlight;
      }

      outcol.x = ::std::min(1.0f, outcol.x);
      outcol.y = ::std::min(1.0f, outcol.y);
      outcol.z = ::std::min(1.0f, outcol.z);

      _AtRGBA(x, y) = ABGR(
        0xFF,
        outcol.z * 0xFF,
        outcol.y * 0xFF,
        outcol.x * 0xFF);
    }
  }

  return *this;
}

static int GetTiffField(TIFF * _File, const uint32_t _TIFFTAG)
{
  int Value = 0;
  TIFFGetField(_File, _TIFFTAG, &Value);
  return Value;
};

Support & Support::Load(const ::std::filesystem::path & _PathToFile)
{
  TIFF * File = TIFFOpen(_PathToFile.string().c_str(), "r");
  if (File == nullptr)
  {
    throw ::std::exception(
      ("open file failed [" + _PathToFile.string() + "]").c_str());
  }

  const auto RowLenght = GetTiffField(File, TIFFTAG_IMAGEWIDTH);
  const auto Right = ::std::min((int)m_SizeX, RowLenght);
  const auto Bottom = ::std::min((int)m_SizeY, GetTiffField(File, TIFFTAG_IMAGELENGTH));
  ::std::vector<float> Line(RowLenght, 0.0f);

  for (int y = 0; y < Bottom; y++)
  {
    TIFFReadScanline(File, Line.data(), y);

    for (int x = 0; x < Right; x++)
    {
      At(x, y) = Line[x];
    }
  }

  TIFFClose(File);

  return *this;
}

Support & Support::Save(const ::std::filesystem::path & _PathToFile)
{
  TIFF * File = TIFFOpen(_PathToFile.string().c_str(), "r+");
  if (File == nullptr)
  {
    throw ::std::exception(
      ("open file failed [" + _PathToFile.string() + "]").c_str());
  }

  const auto RowLenght = GetTiffField(File, TIFFTAG_IMAGEWIDTH);
  const auto Right = ::std::min((int)m_SizeX, RowLenght);
  const auto Bottom = ::std::min((int)m_SizeY, GetTiffField(File, TIFFTAG_IMAGELENGTH));
  ::std::vector<float> Line(RowLenght, 0.0f);

  for (int y = 0; y < Bottom; y++)
  {
    for (int x = 0; x < Right; x++)
    {
      Line[x] = At(x, y);
    }

    TIFFWriteScanline(File, Line.data(), y);
  }

  TIFFClose(File);

  return *this;
}

Support & Support::Save(
  const ::std::filesystem::path & _PathToFile,
  const AtRGBA_t & _AtRGBA)
{
  TIFF * File = TIFFOpen(_PathToFile.string().c_str(), "r+");
  if (File == nullptr)
  {
    throw ::std::exception(
      ("open file failed [" + _PathToFile.string() + "]").c_str());
  }

  const auto RowLenght = GetTiffField(File, TIFFTAG_IMAGEWIDTH);
  const auto Right = ::std::min((int)m_SizeX, RowLenght);
  const auto Bottom = ::std::min((int)m_SizeY, GetTiffField(File, TIFFTAG_IMAGELENGTH));
  ::std::vector<uint32_t> Line(RowLenght, 0);

  for (int y = 0; y < Bottom; y++)
  {
    for (int x = 0; x < Right; x++)
    {
      Line[x] = _AtRGBA(x, y);
    }

    TIFFWriteScanline(File, Line.data(), y);
  }

  TIFFClose(File);

  return *this;
}
