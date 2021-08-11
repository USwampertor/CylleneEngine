#pragma once
#include "cyUtilitiesPrerequisites.h"


namespace CYLLENE_SDK {
  class Vector3f;
  class Vector3i;
  class Vector4f;
  class Vector4i;
  
  class CY_UTILITY_EXPORT Color
  {
  public:

    /**
    * Default constructor
    */
    Color() = default;

    /**
     * Default constructor
     */
    ~Color() = default;

    /**
     * Constructor with a copy of a color
     */
    Color(const Color& copy);

    /**
     * Constructor with a Vector 4
     */
    Color(Vector4f vector);

    /**
     * Constructor with a Vector 3
     */
    Color(Vector3f vector);

    /**
     * @brief Constructor with a RGBA color based on uint32 from 0 to 255
     * @param uint32 R red color from 0 to 255
     * @param uint32 G green color from 0 to 255
     * @param uint32 B blue color from 0 to 255
     * @param uint32 A alpha channel from 0 to 255
     */
    Color(uint32 R, uint32 G = 0, uint32 B = 0, uint32 A = 255);

    /**
     * @brief () operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    const uint32&
    operator()(const uint32& index) const;
    
    /**
     * @brief () operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    uint32&
    operator()(const uint32& index);
    
    /**
     * @brief [] operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    const uint32
    operator[](const uint32& index) const;

    /**
     * @brief [] operator overload
     * @param index of the object we wanna get
     * @return value at index
     *
     */
    uint32&
    operator[](const uint32& index);

    /**
     * @brief Operator * overload to multiply two colors
     * @param Color& other color to combine
     * @return the combination of the two colors
     *
     */
    Color
    operator*(const Color& other);

    /**
     * @brief Operator + overload to sum two colors
     * @param Color& other color to sum
     * @return the combination of the two colors
     *
     */
    Color
    operator+(const Color& other);

    /**
     * @brief Operator - overload to get the difference of two colors
     * @param Color& other color to get the difference
     * @return the difference between two colors
     *
     */
    Color
    operator-(const Color& other);

    /**
     * @brief Operator / overload to get the division between two colors
     * @param Color& other color to get the division
     * @return the division between two colors
     *
     */
    Color
    operator/(const uint32& other);

    const uint32&
    r();

    const uint32&
    g();

    const uint32&
    b();

    const uint32&
    a();


    /**
     * @brief
     * @param Vector3 HSV value
     * @return
     *
     */
    static void
    HSVToRGB(Vector3f hsv);

    /**
     * @brief
     * @param Vector3 HSV value
     * @return
     *
     */
    static void
    RGBToHSV(Vector3f hsv);

    /**
     * @brief Blends two colors
     * @param Color& A
     * @param Color& B
     * @return the blending of the two colors
     *
     */
    static Color
    blend(Color& A, Color& B);

    static Color
    lerp(Color& A, Color& B);

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
    setLinear(float nr, float ng, float nb, float na = 1.0f);

    /**
     * @brief Sets a color based in (R, G, B, A)
     * @param uint32 R
     * @param uint32 G
     * @param uint32 B
     * @param uint32 A
     * @return
     *
     */
    void
    setColor(uint32 nr, uint32 ng, uint32 nb, uint32 na = 255);

    /**
     * @brief Sets a color based in (R, G, B, A)
     * @param uint32 R
     * @param uint32 G
     * @param uint32 B
     * @param uint32 A
     * @return
     *
     */
    void
    setColor(Vector3f);

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

    /**
     * @brief Returns the color as a Vector in RGBA format
     * @param
     * @return a Vector(R, G, B, A) in float (0.0f - 1.0f)
     *
     */
    Vector4f
    linear();

    /**
     * @brief Returns a string with the information of the color
     * @param
     * @return a String with (R: XXX, G: XXX, B: XXX, A: XXX)
     *
     */
    String
    toString();


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

  private:

    /**
     * Red channel of the color
     */
    uint32 m_r;

    /**
     * Green channel of the color
     */
    uint32 m_g;

    /**
     * Blue channel of the color
     */
    uint32 m_b;

    /**
     * Alpha channel of the color
     */
    uint32 m_a;


  };
}
