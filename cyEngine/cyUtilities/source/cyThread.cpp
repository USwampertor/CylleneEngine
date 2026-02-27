#include "cyThread.h"


namespace CYLLENE_SDK {


void
ThreadManager::onStartUp() {
  m_maxThreads = std::thread::hardware_concurrency();
  for (uint32 i = 0; i < m_maxThreads; ++i) {
    m_threads.emplace_back(makeSharedPtr<Thread>(&ThreadManager::iterate, this));
  }
}

void
ThreadManager::iterate() {
  while (true) {
    {
      Callback<void> job;
      {
        MULock lock(m_queueMutex);
        m_mutexCondition.wait(lock, [this]() {
          return m_shouldTerminate || !m_jobs.empty();
          });
        if (m_shouldTerminate && m_jobs.empty()) {
          return;
        }
        job = m_jobs.front();
        m_jobs.pop();
      }
      job();
    }

  }
}

void
ThreadManager::onShutDown() {
  for (auto& thread : m_threads) {
    if (thread->joinable()) {
      thread->join();
    }
  }
  m_threads.clear();
}

bool
ThreadManager::busy() {
  MULock lock(m_queueMutex);
  return !m_jobs.empty();
}

void
ThreadManager::stop() {
  {
    MULock lock(m_queueMutex);
    m_shouldTerminate = true;
  }
  m_mutexCondition.notify_all();
  for (auto& thread : m_threads) {
    if (thread->joinable()) {
      thread->join();
    }
  }
  m_threads.clear();
}


}