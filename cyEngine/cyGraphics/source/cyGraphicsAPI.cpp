#include "cyGraphicsAPI.h"
#include "cyWindow.h"

#include <cySceneManager.h>
#include <cyCCamera.h>

namespace CYLLENE_SDK
{
  
void
GraphicsAPI::createDefaultObjects() {
  Vector<GInputLayoutElement> inputDescs = {
    { "POSITION", 0, COLORFORMAT::E::RGB_32_FLOAT,  0, 0,   INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "NORMAL",   0, COLORFORMAT::E::RGB_32_FLOAT,  0, 12,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "TANGENT",  0, COLORFORMAT::E::RGB_32_FLOAT,  0, 24,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "BINORMAL", 0, COLORFORMAT::E::RGB_32_FLOAT,  0, 36,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "COLOR",    0, COLORFORMAT::E::RGBA_32_FLOAT, 0, 48,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "TEXCOORD", 0, COLORFORMAT::E::RG_32_FLOAT,   0, 64,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "BONES",    0, COLORFORMAT::E::RGBA_32_SINT,  0, 72,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "WEIGHTS",  0, COLORFORMAT::E::RGBA_32_FLOAT, 0, 88,  INPUTCLASSIFICATION::E::PERVERTEX, 0 },
    { "METADATA", 0, COLORFORMAT::E::RGBA_32_SINT,  0, 104, INPUTCLASSIFICATION::E::PERVERTEX, 0 }
  };

  // SPtr<GInputLayout> pInputLayout = createInputLayout(inputDescs, vShader);
  // 
  // if (!pInputLayout) {
  //   WindowManager::ShowErrorMessage("Error", "Error creating Input Layout");
  //   return;
  // }
}

void
GraphicsAPI::addToRenderPool(uint32 index) {

}

void
GraphicsAPI::removeFromRenderPool(uint32 index) {

}

void
GraphicsAPI::registerToRenderPool(uint32 index) {

}

bool
GraphicsAPI::registerGraphicPass(SPtr<GGraphicPass> newPass) {
  return true;
}

void
GraphicsAPI::draw() {
  auto cameras = SceneManager::instance().findBeingsWithComponent<CCamera>();
  if (cameras.empty()) {
    return;
  }

  for (auto& camera : cameras) {
    WPtr<CCamera> cameraPtr = camera.lock()->getComponent<CCamera>();
    if (cameraPtr.lock()) {
      WPtr<GraphicsPipeline> pipeline = cameraPtr.lock()->m_pipeline;

      for (WPtr<GGraphicPass> pass : pipeline.lock()->getPasses()) {
        pass.lock()->execute();
      }

    }



    // cameraPtr->executePasses(); // CCamera has no access to GraphicsAPI as this is a higher level dependency library
  }



}


}
