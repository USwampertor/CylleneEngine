#pragma once
#include <cyCorePrerequisites.h>

using namespace CYLLENE_SDK;

namespace ADND {

  namespace ABILITY_SCORES {
    BETTER_ENUM(E, uint32,
                STRENGTH = 0,
                INTELLIGENCE,
                WISDOM,
                DEXTERITY,
                CONSTITUTION,
                CHARISMA);
  } 

  namespace RACE {
    BETTER_ENUM(E, uint32,
      UNKNOWN,
      DWARF,
      ELF,
      GNOME,
      HALF_ELF,
      HALFLING,
      HALF_ORC,
      HUMAN);
  }

  namespace CLASS {
    BETTER_ENUM(E, uint32,
      UNKNOWN,
      BARBARIAN,
      BARD,
      CLERIC,
      DRUID,
      FIGHTER,
      MONK,
      PALADIN,
      RANGER,
      ROGUE,
      SORCERER,
      WARLOCK,
      WIZARD);
  }

  namespace ALIGNMENT {
    BETTER_ENUM(E, uint32,
      LAWFUL,
      NEUTRAL,
      CHAOTIC);
  }

  namespace MONEY_TYPE {
    BETTER_ENUM(E, uint32,
      COPPER,
      SILVER,
      GOLD,
      ELECTRUM,
      PLATINUM);
  }

  namespace SAVING_THROW {
    BETTER_ENUM(E, uint32,
      ACID,
      CRUSHING_BLOW,
      NORMAL_BLOW,
      DISINTEGRATE,
      FALL,
      FIREBALL,
      MAGICAL_FIRE,
      NORMAL_FIRE,
      FROST,
      LIGHTNING,
      ELECTRICITY);
  }

}