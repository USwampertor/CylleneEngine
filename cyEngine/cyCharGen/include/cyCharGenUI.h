#pragma once
#include "cyCharGenPrerequisites.h"
#include "cyCharacterCreator.h"

using namespace CYLLENE_SDK;

namespace ADND
{

class CharGenUI
{
public:
  CharGenUI() = default;
  ~CharGenUI() = default;

  void
  render();

  CharacterCreator m_characterCreator;
  int raceIndex = 0;
  int subraceIndex = 0;
  int classIndex = 0;
  int subclassIndex = 0;
};

}


