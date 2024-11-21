/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	cyBeing.h
 * @author 	Marco "Swampy" Millan
 * @date 	2024/11/20
 * @brief 	The smallest unit object that can exist, not exist, 
 *          be spawned, used, etc.
 *
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#pragma once
#include "cyCorePrerequisites.h"

#include "cyClassRegister.h"
#include "cyComponent.h"

namespace CYLLENE_SDK
{
/*
 *	@class	Being
 *	@brief	All objects that exist and are updated or not in the game engine
 *          are Being child classes
 *
 */
class CY_CORE_EXPORT Being
{

  BODY();

public:
  /**
   * Default Constructor
   */
  Being() = default;

private:

  String m_name;

  Map<COMPONENT_TYPE::E, SharedPointer<Component>> m_components;

};

REGISTER_CLASS(Being);

}

