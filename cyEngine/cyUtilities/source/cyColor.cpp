#include "cyColor.h"

#include "cyUtilities.h"
#include "cyVector3f.h"
#include "cyVector4f.h"
#include "cyMath.h"

namespace CYLLENE_SDK {

  HSV::HSV(const HSV& other)
    : h(other.h),
      s(other.s),
      v(other.v) {}

  HSV::HSV(const Vector3f& other)
    : h(other.x),
      s(other.y),
      v(other.z) {}

  String
  HSV::toString() {
    return Utils::format("(H: %2.2f, S: %2.2f, V: %2.2f)", h, s, v);
  }


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
  Color::operator*(const Color& other) const {
    return Color(this->r * other.r,
                 this->g * other.g,
                 this->b * other.b,
                 this->a * other.a);
  }

  Color
  Color::operator*(const float& other) const {
    return Color(this->r * other,
                 this->g * other,
                 this->b * other,
                 this->a * other);
  }

  Color
  Color::operator/(const float& other) const {
    return Color(this->r / other,
                 this->g / other,
                 this->b / other,
                 this->a / other);
  }

  Color
  Color::operator+(const Color& other) const {
    return Color(this->r + other.r,
                 this->g + other.g,
                 this->b + other.b,
                 this->a + other.a);
  }

  Color
  Color::operator-(const Color& other) const {
    return Color(this->r - other.r,
                 this->g - other.g,
                 this->b - other.b,
                 this->a - other.a);
  }

  bool
  Color::operator==(const Color& other) const {
    return Math::isNearSame(r, other.r) &&
           Math::isNearSame(g, other.g) &&
           Math::isNearSame(b, other.b) &&
           Math::isNearSame(a, other.a);
  }

  void
  Color::saturate() {
    r = r > 1.0f ? 1.0f : r;
    g = g > 1.0f ? 1.0f : g;
    b = b > 1.0f ? 1.0f : b;
    a = a > 1.0f ? 1.0f : a;
  }

  Color
  Color::fromHSV(const HSV& hsv) {
    double      hh, p, q, t, ff;
    long        i;
    Color       out;

    if (hsv.s <= 0.0) {       // < is bogus, just shuts up warnings
      out.r = hsv.v;
      out.g = hsv.v;
      out.b = hsv.v;
      return out;
    }
    hh = hsv.h;
    if (hh >= 360.0) {
      hh = 0.0;
    }
    hh /= 60.0;
    i = static_cast<long>(hh);
    ff = hh - i;
    p = hsv.v * (1.0 -  hsv.s);
    q = hsv.v * (1.0 - (hsv.s * ff));
    t = hsv.v * (1.0 - (hsv.s * (1.0 - ff)));

    switch (i) {
      case 0:
        out.r = hsv.v;
        out.g = t;
        out.b = p;
        break;
      case 1:
        out.r = q;
        out.g = hsv.v;
        out.b = p;
        break;
      case 2:
        out.r = p;
        out.g = hsv.v;
        out.b = t;
        break;

      case 3:
        out.r = p;
        out.g = q;
        out.b = hsv.v;
        break;
      case 4:
        out.r = t;
        out.g = p;
        out.b = hsv.v;
        break;
      case 5:
      default:
        out.r = hsv.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
  }

  HSV
  Color::toHSV(const Color& color) {
    HSV         out;
    double      min, max, delta;

    min = color.r < color.g ? color.r : color.g;
    min = min < color.b ? min : color.b;

    max = color.r > color.g ? color.r : color.g;
    max = max > color.b ? max : color.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001) {
      out.s = 0;
      out.h = 0; // undefined, maybe nan?
      return out;
    }
    if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
      out.s = (delta / max);                  // s
    }
    else {
      // if max is 0, then r = g = b = 0              
      // s = 0, h is undefined
      out.s = 0.0;
      out.h = NAN;                            // its now undefined
      return out;
    }
    if (color.r >= max) { // > is bogus, just keeps compiler happy
      out.h = (color.g - color.b) / delta;        // between yellow & magenta
    } 
    else {
      if (color.g >= max) {
        out.h = 2.0 + (color.b - color.r) / delta;  // between cyan & yellow
      }
      else {
        out.h = 4.0 + (color.r - color.g) / delta;  // between magenta & cyan
      }
    }
    out.h *= 60.0;                              // degrees
    if (out.h < 0.0)
      out.h += 360.0;
    return out;
  }

  Color
  Color::blend(const Color& A, const Color& B) {
    float alphaSource = A.a;
    float alphaDest = B.a;

    // Compute the resulting alpha
    float outAlpha = alphaSource + alphaDest * (1.0f - alphaSource);

    // Avoid division by zero
    if (outAlpha == 0.0f) {
      return Color(0.0f, 0.0f, 0.0f, 0.0f); // Fully transparent black
    }

    // Compute each blended component (R, G, B)
    float outR = (A.r * alphaSource + B.r * alphaDest * (1.0f - alphaSource)) / outAlpha;
    float outG = (A.g * alphaSource + B.g * alphaDest * (1.0f - alphaSource)) / outAlpha;
    float outB = (A.b * alphaSource + B.b * alphaDest * (1.0f - alphaSource)) / outAlpha;

    return Color(outR, outG, outB, outAlpha);
  }

  Color
  Color::lerp(const Color& A, const Color& B, const float& t) {
    // Clamp t to the range [0, 1]
    float tt = Math::clamp(t, 0.0f, 1.0f);

    // Perform linear interpolation for each component
    float r = A.r + tt * (B.r - A.r);
    float g = A.g + tt * (B.g - A.g);
    float b = A.b + tt * (B.b - A.b);
    float a = A.a + tt * (B.a - A.a);

    return Color(r, g, b, a);
  }

  void
  Color::setFloat(const float& nr, 
                  const float& ng, 
                  const float& nb, 
                  const float& na /* = 1.0f */) {
    r = Math::clamp(nr, 0.0f, 1.0f);
    g = Math::clamp(ng, 0.0f, 1.0f);
    b = Math::clamp(nb, 0.0f, 1.0f);
    a = Math::clamp(na, 0.0f, 1.0f);
  }

  void
  Color::setUint(const uint32& nr, 
                 const uint32& ng, 
                 const uint32& nb, 
                 const uint32& na /* = 255 */) {
    r = Math::clamp(static_cast<uint32>(nr), 
                    static_cast<uint32>(0), 
                    static_cast<uint32>(255)) * Color::Int2Linear;
    g = Math::clamp(static_cast<uint32>(ng), 
                    static_cast<uint32>(0), 
                    static_cast<uint32>(255)) * Color::Int2Linear;
    b = Math::clamp(static_cast<uint32>(nb), 
                    static_cast<uint32>(0), 
                    static_cast<uint32>(255)) * Color::Int2Linear;
    a = Math::clamp(static_cast<uint32>(na), 
                    static_cast<uint32>(0), 
                    static_cast<uint32>(255)) * Color::Int2Linear;
  }

  void
  Color::setFromVector3(const Vector3f& v3) {
    *this = { v3 };
  }

  void
  Color::setFromVector4(const Vector4f& v4) {
    *this = { v4 };
  }

  Vector3f
  Color::toVector3() {
    return Vector3f(r, g, b);
  }

  Vector4f
  Color::toVector4() {
    return Vector4f(r, g, b, a);
  }

  String
  Color::toString() {
    return Utils::format("(R: %2.2f, G: %2.2f, B: %2.2f, A: %2.2f)", r, g, b, a);
  }

  uint32
  Color::toHexValue() {
    int32 nr = static_cast<int32>(r * 255.0f);
    int32 ng = static_cast<int32>(g * 255.0f);
    int32 nb = static_cast<int32>(b * 255.0f);
    int32 na = static_cast<int32>(a * 255.0f);

    return (nr << 24) | (ng << 16) | (nb << 8) | na;
  }

  void
  Color::fromHex(const uint32& hexValue) {
    r = ((hexValue >> 24) & 0xFF) * Color::Int2Linear; // Red
    g = ((hexValue >> 16) & 0xFF) * Color::Int2Linear; // Green
    b = ((hexValue >> 8) & 0xFF)  * Color::Int2Linear; // Blue
    a = (hexValue & 0xFF)         * Color::Int2Linear; // Alpha
  }

  String
  Color::toHexString() {
    uint32 hex = toHexValue();
    return Utils::format("#%x");
  }

  const Color Color::BLACK    = Color(0.00f, 0.00f, 0.00f, 1.00f);
  const Color Color::CLEAR    = Color(0.00f, 0.00f, 0.00f, 0.50f);
  const Color Color::AZURE    = Color(0.94f, 1.00f, 1.00f);
  const Color Color::BLUE     = Color(0.00f, 0.00f, 1.00f);
  const Color Color::CYAN     = Color(0.00f, 1.00f, 1.00f);
  const Color Color::GRAY     = Color(0.50f, 0.50f, 0.50f);
  const Color Color::GREEN    = Color(0.00f, 1.00f, 0.00f);
  const Color Color::GREY     = Color(0.39f, 0.39f, 0.39f);
  const Color Color::MAGENTA  = Color(1.00f, 0.00f, 1.00f);
  const Color Color::RED      = Color(1.00f, 0.00f, 0.00f);
  const Color Color::ORANGE   = Color(1.00f, 0.39f, 0.00f);
  const Color Color::VIOLET   = Color(0.55f, 0.00f, 0.82f);
  const Color Color::WHITE    = Color(1.00f, 1.00f, 1.00f);
  const Color Color::YELLOW   = Color(1.00f, 1.00f, 0.00f);
  const Color Color::MISSING  = Color(1.00f, 0.00f, 0.86f);
  // constexpr float Color::Int2Linear 
  // constexpr float Color::Linear2Int 
}