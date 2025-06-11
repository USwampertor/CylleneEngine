#include "cyCComponent.h"
#include "cyBBeing.h"
namespace CYLLENE_SDK
{
void
CComponent::setOwner(SPtr<BBeing> owner) {
    m_owner = owner;
  }
}