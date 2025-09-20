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

  void
  addPass(SPtr<GraphicPass> newPass) {
    m_passes.push_back(newPass);
  }

  Vector<SPtr<GraphicPass>> 
  getPasses() { return m_passes; }

  SPtr<GraphicPass>
  getAt(int32 index) {
    if (index < m_passes.size() && index >= 0) {
      return m_passes[index];
    }
    return nullptr;
  }

private:

  Vector<SPtr<GraphicPass>> m_passes;
};

}

