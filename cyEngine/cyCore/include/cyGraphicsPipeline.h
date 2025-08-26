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

private:

  Vector<SPtr<GraphicPass>>& 
  getPasses() { return m_passes; }

  Vector<SPtr<GraphicPass>> m_passes;
};

}

