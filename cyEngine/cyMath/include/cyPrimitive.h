/**
 * @file cyPrimitive.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Primitive.
 */

#pragma once
#include "cyMathPrerequisites.h"

#include "cyVector4f.h"

#ifdef RECT
#undef RECT
#endif

namespace CYLLENE_SDK
{

  namespace PRIMITIVE_TYPE
  {
  BETTER_ENUM(E, 
              uint32,
              eNONE = 0, 
              eAABB,
              eCAPSULE,
              eCONE,
              eCYLINDER,
              eFRUSTUM,
              eLINE,
              eOBB,
              ePLANE,
              ePOINT,
              eRAY,
              eRECT,  
              eSPHERE);
  }

/**
 * @class Primitive
 * @brief Abstract base type for all math primitives used in intersection tests.
 */
class CY_MATH_EXPORT Primitive
{
public:

  Primitive() = default;

  Primitive(const PRIMITIVE_TYPE::E& type) :m_type(type) {}

  virtual ~Primitive() = default;
  
  /**
   * @brief Gets the type of the primitive.
   * @return The type of the primitive as an enum value from PRIMITIVE_TYPE::E.
   */
  const PRIMITIVE_TYPE::E getType() const { return m_type; }

  /**
   * @brief Static method to get the type of the primitive class. 
   *        This should be overridden in derived classes to return the appropriate type.
   * @return The type of the primitive as an enum value from PRIMITIVE_TYPE::E.
   * @note This is a static method that should be overridden in 
   *        derived classes to return the appropriate type.
   */
  static PRIMITIVE_TYPE::E staticType() {
    CY_ASSERT(true && "IMPLEMENT THIS");
    return PRIMITIVE_TYPE::E::eNONE;
  }

  /**
   * @brief Checks if this primitive intersects with another primitive. 
   *        The actual intersection logic will depend on the types of the primitives involved.
   * @return True if this primitive intersects with the other primitive, false otherwise.
   *
   * @note This is a virtual function that should be overridden in derived classes to provide
   *       specific intersection logic based on the types of primitives being compared.
   */
  virtual bool
  intersects(const Primitive& other) = 0;

  /**
   * @brief Returns a string representation of the primitive for debugging purposes.
   * @return A string describing the primitive's properties and type.
   * 
   * @note This is a virtual function that should be overridden in derived classes to provide
   *       a type-specific printable representation.
   */
  virtual String
  toString() = 0;

protected:
  /**
   * @brief Runtime type identifier for the primitive.
   */
  PRIMITIVE_TYPE::E m_type = PRIMITIVE_TYPE::E::eNONE;
};

}

