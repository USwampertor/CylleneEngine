#pragma once
#include "cyInput.h"

namespace CYLLENE_SDK
{

/**
*
*/
Input::Input() {
}

/**
*
*/
Input::Input(SizeT _windowHandle)
  : m_windowHandle(_windowHandle),
    m_manager(nullptr) {
}

/**
*
*/
Input::~Input() {
}

/**
*
*/
void
Input::Initialize() {
  OIS::ParamList pl;
  pl.insert(std::make_pair(String("WINDOW"), std::to_string(m_windowHandle)));
  m_manager = OIS::InputManager::createInputSystem(pl);
}

}