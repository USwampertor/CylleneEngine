#pragma once

#include "cyUtilitiesPrerequisites.h"

#include "cyVector2f.h"

namespace CYLLENE_SDK {


  class CY_UTILITY_EXPORT Matrix2x2
  {
  public:

    Matrix2x2() = default;

    ~Matrix2x2() = default;


    /**
     * ZERO filled Matrix
     */
    static const Matrix2x2 ZERO;

    /**
     * IDENTITY matrix
     */
    static const Matrix2x2 IDENTITY;

  private:

    union {
      /**
       * Row major based matrix struct
       */
      struct {
        float m00, m01;
        float m10, m11;
      }_m;
      float m[2][2];
      Vector2f vec[2];
      float fVec[4];
    };



  };

}
