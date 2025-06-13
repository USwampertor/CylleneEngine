#include "cyScene.h"

namespace CYLLENE_SDK {
  void
  Scene::init() {
    m_rootNode = makeUniquePtr<SNode>();
  }

  void
  Scene::onSceneLoaded() {

  }

  void
  Scene::onSceneUnloaded() {

  }

  JSONDocument
  Scene::serialize() {
    JSONDocument document;

    return document;
  }

  void
  Scene::deserialize(const JSONValue& sceneData) {

  }

}