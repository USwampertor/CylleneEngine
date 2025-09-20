#include "cyInputMapping.h"


namespace CYLLENE_SDK {

void
InputMapping::bindAction(const INPUTCODE::E& button, const InputCallback& c) {
  if (m_actions.find(button) == m_actions.end()) {
    SPtr<InputAction> newAction = makeSharedPtr<InputAction>();
    newAction->m_action = c;
    m_actions.try_emplace(button, newAction);
  }
}

void
InputMapping::removeAction(const INPUTCODE::E& button) {
  if (m_actions.find(button) != m_actions.end()) {
    m_actions.erase(button);
  }
}

void
InputMapping::onInputUpdated(const INPUTCODE::E& button, const SPtr<InputValue>& value) {
  if (m_actions.find(button) != m_actions.end()) {
    m_actions[button]->m_action(value);
  }
}

} // namespace CYLLENE_SDK