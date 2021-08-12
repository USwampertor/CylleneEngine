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
    return Color(this->m_r * other.m_r,
                 this->m_g * other.m_g,
                 this->m_b * other.m_b,
                 this->m_a * other.m_a);
  }

  const Color Color::AZURE    = Color(240,255,255);
  const Color Color::BLACK    = Color(0);
  const Color Color::BLUE     = Color(0,0,255);
  const Color Color::CLEAR    = Color(0,0,0,127);
  const Color Color::CYAN     = Color(0,255,255);
  const Color Color::GRAY     = Color(128,128,128);
  const Color Color::GREEN    = Color(0,255,0);
  const Color Color::GREY     = Color(100,100,100);
  const Color Color::MAGENTA  = Color(255,0,255);
  const Color Color::RED      = Color(255,0,0);
  const Color Color::ORANGE   = Color(255,100,0);
  const Color Color::VIOLET   = Color(140,0,210);
  const Color Color::WHITE    = Color(255,255,255);
  const Color Color::YELLOW   = Color(255,255,0);

}