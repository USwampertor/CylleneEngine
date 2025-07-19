#pragma once

#include "cyCharGenPrerequisites.h"
#include "cyCharacter.h"

namespace ADND
{

class CharacterCreator
{
public:

  CharacterCreator() = default;
  ~CharacterCreator() = default;

  Character
  saveCharacter();

  Character
  saveCharacters();

  void
  loadCharacters();
  
  void
  loadCharacter();

  void
  reset();

  void
  rollMoneyAll();

  void
  rollMoney(MONEY_TYPE::E type = MONEY_TYPE::E::COPPER);

  void
  rollSecondarySkillset();

  void
  rollSpells();

  void
  rollAbilityScores(const uint32 method);

  void
  rollSavingThrows();

  Character m_currentCharacter;

  Vector<Character> m_savedCharacters;
};

}

