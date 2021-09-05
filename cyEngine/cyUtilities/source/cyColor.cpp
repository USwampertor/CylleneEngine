#include "cyColor.h"

#include "cyVector3f.h"
#include "cyVector4f.h"

namespace CYLLENE_SDK {

  HSV::HSV(const HSV& other)
    : h(other.h),
      s(other.s),
      v(other.v) {}

  HSV::HSV(const Vector3f& other)
    : h(other.x),
      s(other.y),
      v(other.z) {}



  Color::Color(float nr, float ng, float nb, float na) 
  : r(nr),
    g(ng),
    b(nb),
    a(na) {}

  Color::Color(const Color& copy)
    : r(copy.r),
      g(copy.g),
      b(copy.b),
      a(copy.a) {}

  Color::Color(const Vector3f& vector)
    : r(vector.x),
      g(vector.y),
      b(vector.z),
      a(1.0f) {}

  Color::Color(const Vector4f& vector)
    : r(vector.x),
      g(vector.y),
      b(vector.z),
      a(vector.w) {}

  float&
  Color::operator()(const uint32& index) {
    return (&r)[index];
  }

  const float&
  Color::operator()(const uint32& index) const {
    return (&r)[index];
  }

  float&
  Color::operator[](const uint32& index) {
    return (&r)[index];
  }

  const float
  Color::operator[](const uint32& index) const {
    return (&r)[index];
  }

  Color
  Color::operator*(const Color& other) {
    return Color(this->r * other.r,
                 this->g * other.g,
                 this->b * other.b,
                 this->a * other.a);
  }

  const Color Color::AZURE    = Color(240.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
  const Color Color::BLACK    = Color(0.0f, 0.0f, 0.0f, 1.0f);
  const Color Color::BLUE     = Color(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
  const Color Color::CLEAR    = Color(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 127.0f / 255.0f);
  const Color Color::CYAN     = Color(0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
  const Color Color::GRAY     = Color(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f);
  const Color Color::GREEN    = Color(0.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f);
  const Color Color::GREY     = Color(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f);
  const Color Color::MAGENTA  = Color(255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
  const Color Color::RED      = Color(255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f);
  const Color Color::ORANGE   = Color(255.0f / 255.0f, 100.0f / 255.0f, 0.0f / 255.0f);
  const Color Color::VIOLET   = Color(140.0f / 255.0f, 0.0f / 255.0f, 210.0f / 255.0f);
  const Color Color::WHITE    = Color(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
  const Color Color::YELLOW   = Color(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f);

}