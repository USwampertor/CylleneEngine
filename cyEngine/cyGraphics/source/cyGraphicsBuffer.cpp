#include "cyGraphicsBuffer.h"

namespace CYLLENE_SDK
{

GraphicsBuffer::~GraphicsBuffer() {
  if (m_pBuffer) {
    delete m_pBuffer;
    m_pBuffer = nullptr;
  }
}

}
