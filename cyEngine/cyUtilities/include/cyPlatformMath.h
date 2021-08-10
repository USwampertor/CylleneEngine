
#pragma once

#include "cyPlatformTypes.h"
#include <math.h>
#include <cmath>
#include <algorithm>

#undef min
#undef max

namespace CYLLENE_SDK {


  struct CY_UTILITY_EXPORT PlatformMath {
  public:
    /**
     * Transformation functions
     */

    /**
     * @brief Changes from degrees to rads
     * @param T the degree to translate
     * @return the degree in rads
     *
     */
    template<typename T>
    static T
      degToRad(T degree) {
      return (degree * RADIAN);
    }

    /**
     * @brief Changes from rads to degrees
     * @param T the rad to translate
     * @return the rads in degrees
     *
     */
    template<typename T>
    static T
      radToDeg(T rad) {
      return (rad * DEGREE);
    }

    /**
     * Trigonometric functions
     */

     /**
      * @brief sin function
      * @param radian to get sin
      * @return the sin of the radian
      *
      */
    template<typename T>
    static T
      sin(T rad) {
      return static_cast<T>(std::sin(rad));
    }

    /**
     * @brief cos function
     * @param radian to get cos
     * @return the cos of the radian
     *
     */
    template<typename T>
    static T
    cos(T rad) {
      return static_cast<T>(std::cos(rad));
    }

    /**
     * @brief tan function
     * @param radian to get tan
     * @return the tan of the radian
     *
     */
    template<typename T>
    static T
      tan(T rad) {
      return std::tan(rad);
    }

    /**
     * @brief asin function
     * @param radian to get asin
     * @return the asin of the radian
     *
     */
    template<typename T>
    static T
      asin(T rad) {
      return std::asin(rad);
    }

    /**
     * @brief acos function
     * @param radian to get acos
     * @return the acos of the radian
     *
     */
    template<typename T>
    static T
      acos(T rad) {
      return std::acos(rad);
    }

    /**
     * @brief atan function
     * @param radian to get atan
     * @return the atan of the radian
     *
     */
    template<typename T>
    static T
      atan(T rad) {
      return std::atan(rad);
    }

    /**
     * @brief atan function
     * @param radian to get atan
     * @return the atan of the radian
     *
     */
    template<typename T>
    static T
      atan2(T t1, T t2) {
      return std::atan2(t1, t2);
    }

    /**
     * @brief sinh function
     * @param radian to get sinh
     * @return the sinh of the radian
     *
     */
    template<typename T>
    static T
      sinh(T rad) {
      return std::sinh(rad);
    }

    /**
     * @brief cosh function
     * @param radian to get cosh
     * @return the cosh of the radian
     *
     */
    template<typename T>
    static T
      cosh(T rad) {
      return std::cosh(rad);
    }

    /**
     * @brief tanh function
     * @param radian to get tanh
     * @return the tanh of the radian
     *
     */
    template<typename T>
    static T
      tanh(T rad) {
      return std::tanh(rad);
    }

    /**
     * @brief asin function
     * @param radian to get asin
     * @return the asin of the radian
     *
     */
    template<typename T>
    static T
      asinh(T rad) {
      return std::asinh(rad);
    }

    /**
     * @brief acosh function
     * @param radian to get acosh
     * @return the acosh of the radian
     *
     */
    template<typename T>
    static T
      acosh(T rad) {
      return std::acosh(rad);
    }

    /**
     * @brief atanh function
     * @param radian to get atanh
     * @return the atanh of the radian
     *
     */
    template<typename T>
    static T
      atanh(T rad) {
      return std::atanh(rad);
    }


    /**
     * Basic math library
     */

     /**
      * @brief computes e to the value given
      * @param power of e
      * @return e^power
      *
      */
    template<typename T>
    static T
      exp(T power) {
      return std::exp(power);
    }

    /**
     * @brief computes value to the power given
     * @param value and power of the evaluation
     * @return value^power
     *
     */
    template<typename T>
    static T
      pow(T value, T power) {
      return static_cast<T>(std::pow(value, power));
    }

    /**
     * @brief computes T value to the power given
     * @param T value and float power of the evaluation
     * @return value^power
     *
     */
    template<typename T>
    static T
      powf(T value, float power) {
      return static_cast<float>(std::pow(value, power));
    }

    /**
     * @brief computes absolute of value
     * @param value to get absolute
     * @return value with no signs
     *
     */
    template<typename T>
    static T
      abs(T value) {
      return std::abs(value);
    }

    /**
     * @brief computes square of given value
     * @param value to get square of
     * @return value^2
     *
     */
    template<typename T>
    static T
      sqr(T value) {
      return value * value;
    }

    /**
     * @brief computes cubic of given value
     * @param value to get cubic of
     * @return value^3
     *
     */
    template<typename T>
    static T
      cbc(T value) {
      return value * value * value;
    }

    /**
     * @brief computes square root of given value
     * @param value to get square root of
     * @return value^(1/2)
     *
     */
    template <typename T>
    static T
      sqrt(T value) {
      return static_cast<T>(std::sqrt(value));
    }

    /**
     * @brief computes inverse square root of given value
     * @param value to get inverse square root of
     * @return 1/(value^(1/2))
     *
     */
    template<typename T>
    static T
      invSqrt(T value) {
      return 1.0f / std::sqrt(value);
    }

    /**
     * @brief computes inverse square root of given value in a quick and dirty way without 
     * losing important value (k thx Carmack)
     * @param value to get inverse square root of
     * @return the old and always useful carmack equation
     *
     */
    template<typename T>
    static T
      qInvSqrt(T value) {
      long i;
      float x2, y;
      const float threehalfs = 1.5F;

      x2 = value * 0.5F;
      y = value;
      i = *(long*)&y;                       // evil floating point bit level hacking
      i = 0x5f3759df - (i >> 1);            // what the fuck? 
      y = *(float*)&i;
      y = y * (threehalfs - (x2 * y * y));  // 1st iteration
      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

      return y;
    }

    /**
     * @brief computes cubic root of given value
     * @param value to get cubic root of
     * @return value^(1/3)
     *
     */
    template <typename T>
    static T
      cbrt(T value) {
      return std::cbrt(value);
    }

    /**
     * @brief computes hypotenuse of given CA and CO
     * @param Opposite leg
     * @param Adjacent leg
     * @return value^2
     *
     */
    template <typename T>
    static T
      hypot(T co, T ca) {
      return std::hypot(co, ca);
    }

    /**
     * @brief returns the value a with the sign of b
     * @param A magnitude value
     * @param B sign value
     * @return magnitude of A with sign of B
     *
     */
    template <typename T>
    static T
      copySign(T a, T b) {
      return std::copysignf(a, b);
    }

    /**
     * @brief returns true if two values are almost the same, given a threshold
     * @param A value
     * @param B value
     * @param threshold value to take as reference. Default is SMALLNUMBER
     * @return True if the absolute difference is lesser than the threshold
     *
     */
    template <typename T>
    static bool
      isNearSame(T a, T b, T threshold = SMALLNUMBER) {
      return abs(a - b) <= threshold;
    }

    /**
     * @brief gets the mod of value as float
     * @param float
     * @param denominator
     * @return a module
     *
     */
    template<typename T>
    static T
      mod(T number, T denom) {
      return number % denom;
    }

    /**
     * @brief gets the float mod of value as float
     * @param T number
     * @param T denominator
     * @return a module
     *
     */
    template<typename T>
    static T
      fmod(T number, T denom) {
      return std::fmod(number, denom);
    }

    /**
     * @brief floors the given value to the nearest integer
     * @param value to floor
     * @return floored value
     *
     */
    template<typename T>
    static T
      floor(T a) {
      return std::floor(a);
    }

    /**
     * @brief floors the given value to the nearest position given number
     * @param the number to floor
     * @param the decimal position to take (0) (0.0) (0.00) (0.000)
     * @return the number floored
     *
     */
    template<typename T>
    static T
      floorp(T a, uint32 position = 0) {
      T b = a * (10 ^ position);
      return std::floor(b) / (10 ^ position);
    }

    /**
     * @brief ceiling given value to the nearest integer
     * @param value to ceiling
     * @return ceiling value
     *
     */
    template<typename T>
    static T
      ceil(T a) {
      return std::ceil(a);
    }

    /**
     * @brief sends the value to the ceiling in a given position
     * @param the value to roof
     * @param the decimal position to take (0) (0.0) (0.00) (0.000)
     * @return the number roofed
     *
     */
    template<typename T>
    static T
      ceilp(T a, uint32 position = 0) {
      T b = a * (10 ^ position);
      return std::ceil(b) / (10 ^ position);
    }

    /**
     * @brief rounds values to the nearest integer or half number x.0 <- -> x.5 <- -> y.0
     * @param value to round half
     * @return rounded value
     *
     */
    template<typename T>
    static T
      roundHalf(T a) {
      return std::round(a * 2.0f) / 2.0f;
    }

    /**
     * @brief rounds values to the nearest integer x.0 <- -> y.0
     * @param value to round
     * @return rounded value
     *
     */
    template<typename T>
    static T
      round(T a) {
      return std::round(a);
    }

    /**
     * @brief rounds values to the nearest value given a decimal position
     * @param the value to round
     * @param the decimal position to take in account
     * @return the value rounded to the decimal position
     *
     */
    template<typename T>
    static T
      roundp(T a, uint32 position = 0) {
      T b = a * (10 ^ position);
      return std::round(b) / (10 ^ position);
    }

    /**
     * @brief returns the max value between two numbers
     * @param A value, B value
     * @return highest value
     *
     */
    template<typename T>
    static T
      max(T a, T b) {
      return (a < b) ? a : b;

      //return std::max(a, b);
    }

    /**
     * @brief returns the min value between two numbers
     * @param A value, B value
     * @return lowest value
     *
     */
    template<typename T>
    static T
      min(T a, T b) {

      return (a > b) ? a : b;

      //return std::min(a, b);
    }

    /**
     * @brief returns the max value between 3 numbers
     * @param A value, B value, C value
     * @return highest value
     *
     */
    template<typename T>
    static T
      max3(T a, T b, T c) {
      return max(max(a, b), c);
    }

    /**
     * @brief returns the min value between 3 numbers
     * @param A value, B value, C value
     * @return lowest value
     *
     */
    template<typename T>
    static T
      min3(T a, T b, T c) {
      return min(min(a, b), c);
    }

    /**
     * @brief breaks A value into an integral and a fraction
     * @param fracpart the fraction
     * @param intPart where is going to store the integer
     * @return highest value
     *
     */
    template<typename T>
    static T
      frac(T fracpart, T* intpart) {
      return std::modf(fracpart, intpart);
    }

    /**
     * @brief clamps the value between two given values
     * @param value to clamp, top value, bottom value
     * @return highest value
     *
     */
    template<typename T>
    static T
      clamp(T number, T bot, T top) {
      return min(max(number, bot), top);
    }


    /**
    * Log functions
    */

    /**
     * @brief returns the power of given number ( base 10)
     * @param value to get exponent
     * @return exponent of log(10) value
     *
     */
    template <typename T>
    static T
      log10(T value) {
      return std::log10(value);
    }

    /**
     * @brief returns the power of given number ( base 2)
     * @param value to get exponent
     * @return exponent of log(2) value
     *
     */
    template <typename T>
    static T
      log2(T value) {
      return std::log2(value);
    }

    /**
     * @brief returns the power of given number ( base e)
     * @param value to get exponent
     * @return exponent of ln value
     *
     */
    template <typename T>
    static T
      logN(T value) {
      return std::log(value);
    }

    /**
     * @brief returns the power of given number by given base
     * @param value to get exponent, base of logarithm
     * @return exponent of log(base) value
     *
     */
    template <typename T>
    static T
      logX(T base, T value) {
      return std::log(value) / std::log(base);
    }


    /**
     * @brief creates a lerp between to given values and a scale
     * @param first point a, second point b, scale of lerp
     * @return lerp value
     *
     */
    template<typename T>
    static T
      lerp(T a, T b, float scale) {
      return static_cast<T>(static_cast<float>(a) +
        (static_cast<float>(b) - static_cast<float>(a))
        * scale);
    }

    /**
     * @brief returns the factorial of given number
     * @param top of the factorial list
     * @return the factorial of top
     *
     */
    template<typename T>
    static T
      factorial(T top) {
      return top == 0 ? 1 : (top * factorial(top - 1));
    }

    /**
     * @brief fast cosine of given radian
     * @param radian to get cos
     * @return cosine(rad) with Taylor
     *
     */
    template<typename T>
    static T
      fastcos(T rad) {
      return (1 -
        (pow(rad, 2) / 2) +
        (pow(rad, 4) / 24) -
        (pow(rad, 6) / 720) +
        (pow(rad, 8) / 40320));
    }

    /**
     * @brief fast sin of given radian
     * @param radian to get sin
     * @return sin(rad) with Taylor
     *
     */
    template<typename T>
    static T
      fastsin(T rad) {
      return (rad -
        (pow(rad, 3) / 6) +
        (pow(rad, 5) / 120) -
        (pow(rad, 7) / 5040) +
        (pow(rad, 9) / 362880));
    }

    /**
     * @brief fast tangent of given radian
     * @param radian to get tan
     * @return tan(rad) with Taylor
     *
     */
    template<typename T>
    static T
      fasttan(T rad) {
      return (rad -
        (pow(rad, 3) * 2 / 6) +
        (pow(rad, 5) * 16 / 120) -
        (pow(rad, 7) * 272 / 5040) +
        (pow(rad, 9) * 7936 / 362880));
    }

    /**
     * @brief fast arcocosine of given radian
     * @param radian to get acos
     * @return arcocosine(rad) with Taylor
     *
     */
    template<typename T>
    static T
      fastacos(T rad) {
      float root = sqrt(abs(1.0f - rad));
      float result = -0.0187293f;
      result *= rad;
      result += 0.0742610f;
      result *= rad;
      result -= 0.2121144f;
      result *= rad;
      result += 1.5707288f;
      result *= root;
      return result;
    }

    /**
     * @brief fast arcsine of given radian
     * @param radian to get asin
     * @return arcsine(rad) with Taylor
     *
     */
    template<typename T>
    static T
      fastasin(T rad) {
      float root = sqrt(abs(1.0f - rad));
      float result = -0.0187293f;
      result *= rad;
      result += 0.0742610f;
      result *= rad;
      result -= 0.2121144f;
      result *= rad;
      result += 1.5707288f;
      result = HALF_PI - root * result;
      return result;
    }

    /**
     * @brief fast arctan of given radian
     * @param radian to get atan
     * @return arctan(rad) with Taylor
     *
     */
    template<typename T>
    static T
      fastatan(T rad) {
      return (
        rad -
        (pow(rad, 3) / 3) +
        (pow(rad, 3) / 5) -
        (pow(rad, 3) / 7) +
        (pow(rad, 3) / 9));
    }

    /**
     * @brief fast hyperbolic cosine of given radian
     * @param radian to get cosh
     * @return hyperbolic cosine(rad) with Taylor
     *
     */
    template<typename T>
    static T
    fastcosh(T rad) {
      return (1 +
        (pow(rad, 2) / 2) +
        (pow(rad, 4) / 24) +
        (pow(rad, 6) / 720) +
        (pow(rad, 8) / 40320));
    }

    /**
     * @brief fast hyperbolic cosine of given radian
     * @param radian to get cosh
     * @return hyperbolic cosine(rad) with Taylor
     *
     */
    template<typename T>
    static T
    fastsinh(T rad) {
      return (rad +
        ((rad * rad * rad) / 6) +
        ((rad * rad * rad * rad * rad) / 120) +
        ((rad * rad * rad * rad * rad * rad * rad) / 5040) +
        ((rad * rad * rad * rad * rad * rad * rad * rad * rad) / 362880));
    }

    /**
     * @brief fast hyperbolic tangent of given radian
     * @param radian to get tanh
     * @return hyperbolic tangent(rad) with Taylor
     *
     */
    template<typename T>
    static T
      fasttanh(T rad) {
      return (rad *
        (135135.0f + Math::sqr(rad) *
          (17325.0f + Math::sqr(rad) *
            (378.0f + Math::sqr(rad))))) /
        135135.0f +
        Math::sqr(rad) *
        (62370.0f + Math::sqr(rad) * (3150.0f + Math::sqr(rad) * 28.0f));
    }

    /**
     * Constants
     */

     /**
      * Constant PI 3.141592~
      */
    static const float PI;

    /**
     * HALF of Constant PI 3.141592~
     */
    static const float HALF_PI;

    /**
     * Constant EULER 2.71828182
     */
    static const float EULER;

    /**
     * Equivalent degrees of 1 rad
     */
    static const float DEGREE;

    /**
     * Equivalent radians of 1 Degree
     */
    static const float RADIAN;

    /**
     * A little number. Used for floating precision
     */
    static const float SMALLNUMBER;

    /**
     * A kinda small number. Used for floating precision, but not as precise as SMALLNUMBER
     */
    static const float KINDASMALLNUMBER;

    /**
     * An epsilon for floats, using the b / 2 system
     */
    static const float EPSILONF;

    /**
     * An epsilon for doubles, using the b / 2 system
     */
    static const float EPSILOND;

  };
}
