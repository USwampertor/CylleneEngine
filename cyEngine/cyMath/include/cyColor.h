/**
 * @file cyColor.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Color.
 */

#pragma once

#include "cyMathPrerequisites.h"


namespace CYLLENE_SDK {

// Class forwarding
class Vector3f;
class Vector4f;

// Redefinitions
using sRGBA = Vector4f;
using RGBA = Vector4f;

/**
 * @struct HSV
 * @brief HSV color representation.
 */
struct CY_MATH_EXPORT HSV
{
 public:

  /**
   * @brief Default constructor
   */
  HSV() = default;

  /**
   * @brief Constructs HSV from a Vector3f.
   * @param other Source vector.
   */
  HSV(const Vector3f& other);

  /**
   * @brief Copy constructor.
   * @param other Source HSV value.
   */
  HSV(const HSV& other);


  /**
   * @brief Converts HSV to a string representation.
   * @return String with values "H: XXX S: XXX V: XXX".
   */
  String
  toString();

 public:
    
  /**
   * @brief H value corresponds to Hue
   */
  float h;

  /**
   * @brief S value corresponds to Saturation
   */
  float s;

  /**
   * @brief V value corresponds to, well, Value
   */
  float v;
};

/**
 * @class Color
 * @brief A RGBA color that has values normalized from 0.0f to 1.0f
 */
class CY_MATH_EXPORT Color
{
 public:

  /**
   * @brief Default constructor.
   */
  Color() = default;

  /**
   * @brief Destructor.
   */
  ~Color() = default;

  /**
   * @brief Constructs RGBA color from normalized floats.
   * @param nr Red channel in [0, 1].
   * @param ng Green channel in [0, 1].
   * @param nb Blue channel in [0, 1].
   * @param na Alpha channel in [0, 1].
   */
  Color(float nr, float ng = 0, float nb = 0, float na = 1.0f);

  /**
   * @brief Copy constructor.
   * @param copy Source color.
   */
  Color(const Color& copy);

  /**
   * @brief Constructs color from Vector4f.
   * @param vector Source vector.
   */
  Color(const Vector4f& vector);

  /**
   * @brief Constructs color from Vector3f.
   * @param vector Source vector.
   */
  Color(const Vector3f& vector);

  /**
   * @brief () operator overload
   * @param index of the object we wanna get
   * @return value at index
   *
   */
  const float&
  operator()(const uint32& index) const;
    
  /**
   * @brief () operator overload
   * @param index of the object we wanna get
   * @return value at index
   */
  float&
  operator()(const uint32& index);
    
  /**
   * @brief [] operator overload
   * @param index of the object we wanna get
   * @return value at index
   */
  const float
  operator[](const uint32& index) const;

  /**
   * @brief [] operator overload
   * @param index of the object we wanna get
   * @return value at index
   */
  float&
  operator[](const uint32& index);

  /**
   * @brief Multiplies two colors component-wise.
   * @param other Color to multiply.
   * @return Resulting color.
   */
  Color
  operator*(const Color& other) const;

  /**
   * @brief Multiplies color by scalar.
   * @param other Scalar multiplier.
   * @return Resulting color.
   */
  Color
  operator*(const float& other) const;

  /**
   * @brief Adds two colors component-wise.
   * @param other Color to add.
   * @return Resulting color.
   */
  Color
  operator+(const Color& other) const;

  /**
   * @brief Subtracts two colors component-wise.
   * @param other Color to subtract.
   * @return Resulting color.
   */
  Color
  operator-(const Color& other) const;

  /**
   * @brief Divides color by scalar.
   * @param other Scalar divisor.
   * @return Resulting color.
   */
  Color
  operator/(const float& other) const;

  /**
   * @brief Checks whether two colors are nearly equal.
   * @param other Color to compare.
   * @return True if both colors are equal within epsilon.
   */
  bool
  operator==(const Color& other) const;


  /**
   * @brief Clamps channels to valid range.
   */
  void
  saturate();

  /**
   * @brief Converts HSV to RGBA color.
   * @param hsv HSV value.
   * @return Converted color.
   */
  static Color
  fromHSV(const HSV& hsv);

  /**
   * @brief Converts RGBA color to HSV.
   * @param color Color value.
   * @return Converted HSV value.
   */
  static HSV
  toHSV(const Color& color);

  /**
   * @brief Blends two colors.
   * @param A First color.
   * @param B Second color.
   * @return Blended color.
   */
  static Color
  blend(const Color& A, const Color& B);

  /**
   * @brief Linearly interpolates between two colors.
   * @param A Start color.
   * @param B End color.
   * @param t Interpolation factor in [0, 1].
   * @return Interpolated color.
   */
  static Color
  lerp(const Color& A, const Color& B, const float& t);

  /**
   * @brief Sets normalized RGBA channels.
   * @param nr Red channel in [0, 1].
   * @param ng Green channel in [0, 1].
   * @param nb Blue channel in [0, 1].
   * @param na Alpha channel in [0, 1].
   */
  void
  setFloat(const float& nr, 
           const float& ng, 
           const float& nb, 
           const float& na = 1.0f);

  /**
   * @brief Sets RGBA channels from 8-bit values.
   * @param nr Red channel in [0, 255].
   * @param ng Green channel in [0, 255].
   * @param nb Blue channel in [0, 255].
   * @param na Alpha channel in [0, 255].
   */
  void
  setUint(const uint32& nr, 
          const uint32& ng, 
          const uint32& nb, 
          const uint32& na = 255);

  /**
   * @brief Sets color from Vector3f RGB values.
   * @param v3 Source RGB vector.
   */
  void
  setFromVector3(const Vector3f& v3);

  /**
   * @brief Sets color from Vector4f RGBA values.
   * @param v4 Source RGBA vector.
   */
  void
  setFromVector4(const Vector4f& v4);

  /**
   * @brief Returns color as Vector3f RGB.
   * @return RGB vector.
   */
  Vector3f
  toVector3();

  /**
   * @brief Returns color as Vector4f RGBA.
   * @return RGBA vector.
   */
  Vector4f
  toVector4();

  /**
   * @brief Returns color as grayscale value.
   * @return Grayscale value.
   */
  float
  toGrey();

  /**
   * @brief Returns a string with color information.
   * @return String with (R: XXX, G: XXX, B: XXX, A: XXX).
   */
  String
  toString();

  /**
   * @brief Converts this color to a packed hex value.
   * @return Packed color value.
   */
  uint32
  toHexValue();

  /**
   * @brief Sets color from packed hex value.
   * @param hexValue Packed color value.
   */
  void
  fromHex(const uint32& hexValue);

  /**
   * @brief Converts this color to a hex string.
   * @return Hex string representation.
   */
  String
  toHexString();

  /**
   * @brief Predefined azure color.
   */
  static const Color AZURE;
  /**
   * @brief Predefined black color.
   */
  static const Color BLACK;
  /**
   * @brief Predefined blue color.
   */
  static const Color BLUE;
  /**
   * @brief Predefined transparent color.
   */
  static const Color CLEAR;
  /**
   * @brief Predefined cyan color.
   */
  static const Color CYAN;
  /**
   * @brief Predefined gray color.
   */
  static const Color GRAY;
  /**
   * @brief Predefined green color.
   */
  static const Color GREEN;
  /**
   * @brief Predefined grey color alias.
   */
  static const Color GREY;
  /**
   * @brief Predefined magenta color.
   */
  static const Color MAGENTA;
  /**
   * @brief Predefined red color.
   */
  static const Color RED;
  /**
   * @brief Predefined orange color.
   */
  static const Color ORANGE;
  /**
   * @brief Predefined violet color.
   */
  static const Color VIOLET;
  /**
   * @brief Predefined white color.
   */
  static const Color WHITE;
  /**
   * @brief Predefined yellow color.
   */
  static const Color YELLOW;
  /**
   * @brief Fallback color used to represent missing values.
   */
  static const Color MISSING;

  static constexpr float Int2Linear = 1.0f / 255.0f;
  static constexpr float Linear2Int = 255.0f;


 public:

  /**
   * @brief Red channel of the color
   */
  float r;

  /**
   * @brief Green channel of the color.
   */
  float g;

  /**
   * @brief Blue channel of the color.
   */
  float b;

  /**
   * @brief Alpha channel of the color.
   */
  float a;


};
}

