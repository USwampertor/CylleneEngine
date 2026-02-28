#include "cyGViewport.h"

namespace CYLLENE_SDK
{

void
GViewport::setViewport(int32 x, int32 y, int32 width, int32 height) {
  m_rect.setDimensions(x, y, width, height);
}

}