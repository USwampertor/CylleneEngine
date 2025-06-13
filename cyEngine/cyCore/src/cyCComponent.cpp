#include "cyCComponent.h"
#include "cyBBeing.h"
namespace CYLLENE_SDK
{
void
CComponent::setOwner(WPtr<BBeing> owner) {
    m_owner = owner;
  }
}