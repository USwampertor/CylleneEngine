#include "cyScene.h"

namespace CYLLENE_SDK {
void
Scene::init() {
  SPtr<BBeing> rootNode = makeSharedPtr<BBeing>("root");
  rootNode->m_self = std::static_pointer_cast<BBeing>(rootNode);
  rootNode->onCreate();
  rootNode->setParent({});
  m_rootNode = rootNode;
  m_rootNode->setActive(true);
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