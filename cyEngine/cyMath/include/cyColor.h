/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file 	cyColor.h
 * @author 	Marco "Swampy" Millan
 * @date 	2022/02/28
 * @brief A color object, you know, for coloring or holding color
 *        information
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
#pragma once

#include "cyMathPrerequisites.h"


namespace CYLLENE_SDK {

// Class forwarding
class Vector3f;
class Vector4f;

/*
 *	@class HSV
 *	@brief A HSV Color structure
 */
struct CY_MATH_EXPORT HSV
{
 public:

  /**
    * @brief Default constructor
    */
  HSV() = default;

  HSV(const Vector3f& other);

  /**
    * @brief Default destructor
    */
  HSV(const HSV& other);


  /**
   * @brief Returns the HSV object into a string
   * @return String with values "H: XXX S: XXX V: XXX"
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

using sRGBA = Vector4f;

using RGBA = Vector4f;
  
class CY_MATH_EXPORT Color
{
 public:

  /*
   * @brief Default constructor
   */
  Color() = default;

  /*
   * @brief Default constructor
   */
  ~Color() = default;

  /**
    * @brief Constructor with a RGBA color based on float from 0.0f to 1.0f
    * @param float R red color from 0.0f to 1.0f
    * @param float G green color from 0.0f to 1.0f
    * @param float B blue color from 0.0f to 1.0f
    * @param float A alpha channel from 0.0f to 1.0f
    */
  Color(float nr, float ng = 0, float nb = 0, float na = 1.0f);

  /**
    * @brief Constructor with a copy of a color
    */
  Color(const Color& copy);

  /**
    * @brief Constructor with a Vector 4
    */
  Color(const Vector4f& vector);

  /**
    * @brief Constructor with a Vector 3
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
    *
    */
  float&
  operator()(const uint32& index);
    
  /**
    * @brief [] operator overload
    * @param index of the object we wanna get
    * @return value at index
    *
    */
  const float
  operator[](const uint32& index) const;

  /**
    * @brief [] operator overload
    * @param index of the object we wanna get
    * @return value at index
    *
    */
  float&
  operator[](const uint32& index);

  /**
    * @brief Operator * overload to multiply two colors
    * @param Color& other color to combine
    * @return the combination of the two colors
    *
    */
  Color
  operator*(const Color& other) const;

  /**
    * @brief Operator * overload to multiply two colors
    * @param Color& other color to combine
    * @return the combination of the two colors
    *
    */
  Color
  operator*(const float& other) const;

  /**
    * @brief Operator + overload to sum two colors
    * @param Color& other color to sum
    * @return the combination of the two colors
    *
    */
  Color
  operator+(const Color& other) const;

  /**
    * @brief Operator - overload to get the difference of two colors
    * @param Color& other color to get the difference
    * @return the difference between two colors
    *
    */
  Color
  operator-(const Color& other) const;

  /**
    * @brief Operator / overload to get the division between two colors
    * @param Color& other color to get the division
    * @return the division between two colors
    *
    */
  Color
  operator/(const float& other) const;

  /*
   *	@brief  Operator == overload to check if two colors are the same
   *	@param	Color& other color to check
   *  @return	true if both colors rgba are each one less than epsilon
   */
  bool
  operator==(const Color& other) const;


  /*
   *	@brief	if the color is higher than 1, it clamps it down
   *	@param		
   *  @return	
   */
  void
  saturate();

  /**
    * @brief Converts a HSV to a color
    * @param Vector3 HSV value
    * @return
    *
    */
  static Color
  fromHSV(const HSV& hsv);

  /**
    * @brief
    * @param Vector3 HSV value
    * @return
    *
    */
  static HSV
  toHSV(const Color& color);

  /**
    * @brief Blends two colors
    * @param Color& A
    * @param Color& B
    * @return the blending of the two colors
    *
    */
  static Color
  blend(const Color& A, const Color& B);

  /*
    *	@brief	  Creates a lerp between two colors
    *	@param	  
    *	@return
    */
  static Color
  lerp(const Color& A, const Color& B, const float& t);

  /**
    * @brief Sets a color based in (R, G, B, A)
    * @param float R
    * @param float G
    * @param float B
    * @param float A
    * @return
    *
    */
  void
  setFloat(const float& nr, 
           const float& ng, 
           const float& nb, 
           const float& na = 1.0f);

  /**
    * @brief Sets a color based in (R, G, B, A) from 0 to 255
    * @param uint32 R red color
    * @param uint32 G green color
    * @param uint32 B blue color
    * @param uint32 A alpha transparency
    * @return
    *
    */
  void
  setUint(const uint32& nr, 
          const uint32& ng, 
          const uint32& nb, 
          const uint32& na = 255);

  /**
    * @brief Sets a color based in (R, G, B)
    * @param Vector3f the RGB
    * @return
    *
    */
  void
  setFromVector3(const Vector3f& v3);

  /**
    * @brief Sets a color based in (R, G, B, A)
    * @param Vector4f the RGBA
    * @return
    *
    */
  void
  setFromVector4(const Vector4f& v4);

  /**
    * @brief Returns the color as a vector3 in RGB format
    * @param
    * @return a Vector3(R, G, B) in uint32
    *
    */
  Vector3f
  toVector3();

  /**
    * @brief Returns the color as a Vector4 in RGBA format
    * @param
    * @return a Vector4(R, G, B, A) in uint32 (0-255)
    *
    */
  Vector4f
  toVector4();

  float
  toGrey();

  /**
    * @brief Returns a string with the information of the color
    * @param
    * @return a String with (R: XXX, G: XXX, B: XXX, A: XXX)
    *
    */
  String
  toString();

  uint32
  toHexValue();

  void
  fromHex(const uint32& hexValue);

  String
  toHexString();

  static const Color AZURE;
  static const Color BLACK;
  static const Color BLUE;
  static const Color CLEAR;
  static const Color CYAN;
  static const Color GRAY;
  static const Color GREEN;
  static const Color GREY;
  static const Color MAGENTA;
  static const Color RED;
  static const Color ORANGE;
  static const Color VIOLET;
  static const Color WHITE;
  static const Color YELLOW;
  static const Color MISSING;

  static constexpr float Int2Linear = 1.0f / 255.0f;
  static constexpr float Linear2Int = 255.0f;


 public:

  /**
    * Red channel of the color
    */
  float r;

  /**
    * Green channel of the color
    */
  float g;

  /**
    * Blue channel of the color
    */
  float b;

  /**
    * Alpha channel of the color
    */
  float a;


};
}
