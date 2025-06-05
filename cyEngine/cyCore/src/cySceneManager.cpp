#include "cySceneManager.h"

namespace CYLLENE_SDK {
SPtr<Scene> 
SceneManager::getActiveScene()
{
  return m_activeScene;
}

SPtr<Scene> SceneManager::createScene(const String& newSceneName)
{
  SPtr<Scene> newScene = makeSharedPtr<Scene>(newSceneName);
  newScene->init();
  m_scenes.push_back(newScene);
  return newScene;
}

bool SceneManager::changeScene(const String& sceneToLoad)
{
  if (m_activeScene)
  {
    m_activeScene->onSceneUnloaded();
  }
  for (const SPtr<Scene>& s : m_scenes)
  {
    if (s->getName() == sceneToLoad)
    {
      m_activeScene = s;
      m_activeScene->onSceneLoaded();
      return true;
    }
  }
  return false;
}

SPtr<Scene> SceneManager::loadScene(const String& newSceneName)
{
  SPtr<Scene> s = nullptr;

  return s;
}

void SceneManager::saveScene()
{

}

SPtr<Scene> SceneManager::findScene(const String& sceneToFind)
{
  for (int i = 0; i < m_scenes.size(); ++i)
  {
    if (m_scenes[i]->getName() == sceneToFind)
    {
      return m_scenes[i];
    }
  }
  return nullptr;
}

void SceneManager::update(const float& delta)
{
  if (!m_activeScene) return;

  // Process deletions
  auto& toRemove = m_activeScene->m_toRemove;
  auto& nodes = m_activeScene->m_nodes;

  for (auto it = nodes.begin(); it != nodes.end(); ) {
    if (auto being = std::static_pointer_cast<BBeing>(*it)) {
      if (being->m_markedToDestroy) {
      // Notify components
        being->onDestroy();
        being->removeAllComponents();
      }

      // Remove from scene
      it = nodes.erase(it);
    }
    else {
      ++it;
    }
  }
  toRemove.clear();


  // Update all active nodes
  for (auto& node : nodes) {
    if (node->isActive()) {
      if (auto being = std::static_pointer_cast<BBeing>(node)) {
        being->update(delta);
      }
    }
  }
  // rmt_ScopedCPUSample(Update, 0);
  // Delete Entities that are marked for delete
  // for (SPtr<BBeing> toDelete : m_activeScene->m_toRemove)
  // {
  //   int i = 0;
  // 
  //   for (SPtr<BBeing> e : m_activeScene->m_beings)
  //   {
  //     if (e.get() == toDelete)
  //     {
  //       //
  //       e->OnDestroy();
  //       e.reset();
  //       m_activeScene->m_entities.erase(m_activeScene->m_entities.begin() + i);
  //       break;
  //     }
  //     ++i;
  //   }
  //   m_activeScene->m_toRemove.clear();
  // }
  // 
  // for (int j = 0; j < m_activeScene->m_entities.size(); ++j)
  // {
  //   // TODO: Check a better way of doing this
  //   if (!App::Instance().m_parser.HasFlag("editor"))
  //   {
  //     m_activeScene->m_entities[j]->Update(delta);
  //   }
  // 
  // }
}

// void SceneManager::updateRender(RenderWindow& w)
// {
//   // rmt_ScopedCPUSample(UpdateRender, 0);
//   for (int i = 0; i < SceneManager::Instance().GetActiveScene()->m_entities.size(); ++i)
//   {
// 
//     if (m_activeScene->m_entities[i]->GetComponent<Sprite>())
//     {
//       w.draw(*m_activeScene->m_entities[i]->GetComponent<Sprite>());
//     }
//     if (m_activeScene->m_entities[i]->GetComponent<BoxCollider>())
//     {
//       w.draw(*m_activeScene->m_entities[i]->GetComponent<BoxCollider>());
//     }
//     if (App::Instance().m_parser.HasFlag("editor"))
//     {
//       w.draw(*m_activeScene->m_entities[i]->m_gizmoSprite);
//     }
//   }
// }

JSONDocument SceneManager::serialize(const Vector<String>& names)
{
  JSONDocument document;
  // document.SetArray();
  // JSONDocument::AllocatorType& allocator = document.GetAllocator();
  // 
  // for (const auto& name : names)
  // {
  //   JSONValue obj(rapidjson::kObjectType);
  // 
  //   SPtr<Scene> sceneToSave = FindScene(name);
  //   obj.AddMember("sceneName", sceneToSave->m_sceneName, allocator);
  //   obj.AddMember("gameSettings", sceneToSave->m_settings->m_gameSettings, allocator);
  // 
  //   JSONValue gravity(rapidjson::kArrayType);
  //   gravity.PushBack(sceneToSave->m_settings->m_levelGravity.x, allocator);
  //   gravity.PushBack(sceneToSave->m_settings->m_levelGravity.y, allocator);
  //   obj.AddMember("gravity", gravity, allocator);
  // 
  //   JSONValue entitiesDoc(rapidjson::kArrayType);
  //   for (int i = 0; i < sceneToSave->m_entities.size(); ++i)
  //   {
  //     JSONDocument entityDoc = sceneToSave->m_entities[i]->Serialize();
  //     entitiesDoc.PushBack(JSONValue(entityDoc, allocator), allocator);
  //   }
  //   obj.AddMember("entityData", entitiesDoc, allocator);
  // 
  //   document.PushBack(obj, allocator);
  // }
  return document;

}

void SceneManager::deserialize(const JSONValue& sceneArray)
{
  // for (auto& itr : sceneArray.GetArray())
  // {
  //   // String sceneName = itr.GetObject()["sceneName"].GetString();
  //   auto obj = itr.GetObject();
  //   String sceneName = obj["sceneName"].GetString();
  //   String gameSettings = obj["gameSettings"].GetString();
  // 
  //   Vector2f gravity;
  //   // JSONValue gravityObj = obj["gravity"].GetArray()[0];
  //   gravity.x = obj["gravity"].GetArray()[0].GetFloat();
  //   gravity.y = obj["gravity"].GetArray()[1].GetFloat();
  // 
  //   SPtr<Scene> newScene = CreateScene(sceneName);
  //   newScene->m_settings->m_levelGravity = gravity;
  //   newScene->m_settings->m_gameSettings = gameSettings;
  // 
  //   // JSONValue entityData = obj["entityData"].GetArray();
  // 
  //   for (auto& entity : obj["entityData"].GetArray())
  //   {
  //     SPtr<Entity> e = ClassRegisters::CreateEntity(entity["type"].GetString());
  //     newScene->m_entities.push_back(e);
  //   }
  // }
  // m_activeScene = m_scenes[0];
}
}