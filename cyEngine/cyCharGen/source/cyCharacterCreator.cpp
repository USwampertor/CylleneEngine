#include "cyCharacterCreator.h"
#include <cyTime.h>

namespace ADND {
Character
CharacterCreator::saveCharacter() {
  return {};
}


Character
CharacterCreator::saveCharacters() {
  return {};
}

void
CharacterCreator::loadCharacters() {

}
  
void
CharacterCreator::loadCharacter() {

}

void
CharacterCreator::reset() {
  m_currentCharacter = {};
  m_currentCharacter.m_birthDate = Time::instance().now();
}

void
CharacterCreator::rollMoneyAll() {

}

void
CharacterCreator::rollMoney(MONEY_TYPE::E type/* = MONEY_TYPE::E::COPPER*/) {

}

void
CharacterCreator::rollSecondarySkillset() {

}

void
CharacterCreator::rollSpells() {

}

void
CharacterCreator::rollAbilityScores(const uint32 method) {

}

void
CharacterCreator::rollSavingThrows() {

}

}