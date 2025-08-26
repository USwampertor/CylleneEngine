#pragma once
#include "cyCorePrerequisites.h"

namespace CYLLENE_SDK
{

class GraphicPass
{
public:

  GraphicPass() = default;
  
  virtual ~GraphicPass() {}
};

class GraphicsPipeline
{
public:

  Vector<SPtr<GraphicPass>> 
  getPasses() { return m_passes; }

private:

  Vector<SPtr<GraphicPass>> m_passes;
};

}

