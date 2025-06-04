#pragma once
#include "cyGraphicsPrerequisites.h"

namespace CYLLENE_SDK
{

  namespace GPRIMITIVE_TOPOLOGY
  {
    BETTER_ENUM(E, uint32,
      eUNKNOWN = -1,
      ePOINTLIST = 0,
      eLINELIST = 1,
      eLINESTRIP = 2,
      eTRIANGLELIST = 3,
      eTRIANGLESTRIP = 4,
      eLINELIST_ADJ = 5,
      eLINESTRIP_ADJ = 6,
      eTRIANGLELIST_ADJ = 7,
      eTRIANGLESTRIP_ADJ = 8
    );
  }

  namespace GRAPHIC_TYPE
  {
    BETTER_ENUM(E, uint32,
      eUNKNOWN = -1,
      eNONE = 0,
      eMODEL = 1,
      eMATERIAL = 2,
      eTEXTURE = 3,
      eSHADER = 5,
      eRENDER_TARGET = 5,
      eVERTEX_BUFFER = 6,
      eINDEX_BUFFER = 7,
      eCONSTANT_BUFFER = 8,
      eINPUT_LAYOUT = 9
    );
  }

  namespace GRESOURCE_USAGE
  {
    BETTER_ENUM(E, uint32,
                eDEFAULT,
                eIMMUTABLE,
                eDYNAMIC,
                eSTAGING);
  }

  namespace GRESOURCE_BINDINGS
  {
    BETTER_ENUM(E, uint32,
                VERTEXBUFFER      = 1 << 0,
                INDEXBUFFER       = 1 << 1,
                CONSTANTBUFFER    = 1 << 2,
                SHADERRESOURCE    = 1 << 3,
                STREAMOUTPUT      = 1 << 4,
                RENDERTARGET      = 1 << 5,
                DEPTHSTENCIL      = 1 << 6,
                UNORDEREDACCESS   = 1 << 7,
                DECODER           = 1 << 8,
                ENCODER           = 1 << 9);
  }

  namespace GDSV_DIMENSION
  {
    BETTER_ENUM(E, uint32,
                UNKNOWN           = 0,
                TEXTURE1D         = 1,
                TEXTURE1DARRAY    = 2,
                TEXTURE2D         = 3,
                TEXTURE2DARRAY    = 4,
                TEXTURE2DMS       = 5,
                TEXTURE2DMSARRAY  = 6);
  }

  
  namespace GCLEAR_FLAGS
  {
    BETTER_ENUM(E, uint32,
                NONE    = 0,
                DEPTH   = 1 << 0,
                STENCIL = 1 << 1,
                COLOR   = 1 << 2,
                ALL     = 1 << 0 | 1 << 1 | 1 << 2);
  }

class CY_GRAPHICS_EXPORT GGraphic
{
public:
  /*
   * You shouldn't be doing this but hey, to each their own 
   */
  GGraphic() = default;
  
  virtual ~GGraphic() {}

  // static GRAPHIC_TYPE::E
  // staticType() {
  //   CY_ASSERT(true && "Graphic::staticType() should never be called");
  //   return GRAPHIC_TYPE::E::eNONE;
  // }

  virtual void*
  get() = 0;

  virtual void
  set(void*) = 0;

  // const GRAPHIC_TYPE::E& getType() { return m_type; }

protected:

  // GRAPHIC_TYPE::E m_type = GRAPHIC_TYPE::E::eUNKNOWN;


};

}