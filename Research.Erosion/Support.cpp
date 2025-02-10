
#include "Support.hpp"
#include <iostream>
#include <tiffio.h>

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

Support & Support::Normalize(const float _WaterLevel)
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
