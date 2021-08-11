#include "cyColor.h"

namespace CYLLENE_SDK {
  Color::Color(const Color& copy)
    : m_r(copy.m_r),
      m_g(copy.m_g),
      m_b(copy.m_b),
      m_a(copy.m_a) {}

  Color::Color(uint32 R, uint32 G, uint32 B, uint32 A)
    : m_r(R),
      m_g(G),
      m_b(B),
      m_a(A) {}

  uint32&
  Color::operator()(const uint32& index) {
    return (&m_r)[index];
  }

  const uint32&
  Color::operator()(const uint32& index) const {
    return (&m_r)[index];
  }

  uint32&
  Color::operator[](const uint32& index) {
    return (&m_r)[index];
  }

  const uint32
  Color::operator[](const uint32& index) const {
    return (&m_r)[index];
  }

  Color
  Color::operator*(const Color& other) {

  }

  const Color Color::Azure    = Color(240,255,255);
  const Color Color::Black    = Color(0);
  const Color Color::Blue     = Color(0,0,255);
  const Color Color::Clear    = Color(0,0,0,127);
  const Color Color::Cyan     = Color(0,255,255);
  const Color Color::Gray     = Color(128,128,128);
  const Color Color::Green    = Color(0,255,0);
  const Color Color::Grey     = Color(100,100,100);
  const Color Color::Magenta  = Color(255,0,255);
  const Color Color::Red      = Color(255,0,0);
  const Color Color::Orange   = Color(255,100,0);
  const Color Color::Violet   = Color(140,0,210);
  const Color Color::White    = Color(255,255,255);
  const Color Color::Yellow   = Color(255,255,0);

}