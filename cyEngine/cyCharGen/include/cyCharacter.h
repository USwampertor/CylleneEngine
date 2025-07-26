#pragma once

#include "cyCharGenPrerequisites.h"

#include <cyTime.h>
#include <cyJSON.h>

using namespace CYLLENE_SDK;

namespace ADND
{
  


class Character
{
public:
  Date m_birthDate;
  String m_name;
  String m_sex;
  int m_xp;
  int m_level;

  Map<ABILITY_SCORES::E, int> m_abilityScores;

  RACE::E m_race = RACE::E::UNKNOWN;
  RACE::E m_secondaryRace = RACE::E::UNKNOWN;

  CLASS::E m_class = CLASS::E::UNKNOWN;  
  CLASS::E m_secondaryClass = CLASS::E::UNKNOWN;

  int m_age;

  float m_xpMultiplier;

  int m_hitPoints;
  int m_maxHitPoints;

  float m_height;
  float m_weight;

  Map<SAVING_THROW::E, int> m_savingThrows = {};

  JSONDocument
  serialize();
};

}

