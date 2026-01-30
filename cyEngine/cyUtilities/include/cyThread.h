#pragma once

#include "cyUtilitiesPrerequisites.h"

#include "cyModule.h"
#include "cySmartPointers.h"
#include "cyEvent.h"


namespace CYLLENE_SDK
{

namespace THREAD_TYPE
{
  BETTER_ENUM(E, uint8,
    AUDIO,
    GRAPHICS,
    CORE,
    WORKER,
    GENERIC
  );
} // namespace THREAD_TYPE


struct Threading
{
  static void
  sleepFor(Miliseconds milliseconds) {
    std::this_thread::sleep_for(milliseconds);
    
  }

  static void
  yield() {
    std::this_thread::yield();
  }

  template<typename T>
  static void
  sleepUntil(TimePoint<T> duration) {
    std::this_thread::sleep_until(duration);
  }


  static ThreadID
  getID() {
    return std::this_thread::get_id();
  }

};

// TODO: Check for Thread generation to have a manager

/*
 *	@class	ThreadManager
 *	@brief	A module to create Threads and have a reference to them to safely manage
 *          and dispose them when not needed anymore
 *
 */
class CY_UTILITY_EXPORT ThreadManager : public Module<ThreadManager>
{
public:

  virtual void
  onStartUp() override;

  virtual void
  onShutDown() override;

  template<typename F, typename ... Args>
  void
  enqueue(F&& f, Args&& ... args) {
    MULock lock(m_queueMutex);
    auto task = Callback<void>([fn = std::bind(std::forward<F>(f), std::forward<Args>(args)...)]() mutable {
      fn();
    });
    // wrap the callable and its params into a void() job
    m_jobs.push(task);
    m_mutexCondition.notify_one();
  }

  // Accept an already-wrapped job
  void
  enqueue(const Callback<void>& job) {
    MULock lock(m_queueMutex);
    m_jobs.push(job);
    m_mutexCondition.notify_one();
  }

  template<class F, class... Args>
  auto enqueueFuture(F&& f, Args&&... args) -> Future<typename ResultOf<F(Args...)>::type> {
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = makeSharedPtr<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );

    Future<return_type> res = task->get_future(); {
      MULock lock(m_queueMutex);
      if (m_shouldTerminate) {
        throw std::runtime_error("enqueue on stopped ThreadPool");
      }

      m_jobs.emplace([task]() { (*task)(); });
    }

    m_mutexCondition.notify_one();
    return res;
  }
  

  void
  stop();

  bool
  busy();

  void
  iterate();



private:

  /**
   * The vector of threads
   */
  Vector<SPtr<Thread>> 
  m_threads;

  /**
   * Max number of threads we should allow
   */
  uint32 
  m_maxThreads = 1;

  bool 
  m_shouldTerminate = false;

  Mutex 
  m_queueMutex;

  ConditionVariable
  m_mutexCondition;
  
  Queue<Callback<void>>
  m_jobs;

};

/*
 *	@brief	Creates a thread that is stored in the thread manager. This function
 *          acts so user does not interact directly with the Manager
 *	@param	F&& f function to run in the thread
 *	@param	Args ... args the object to manage the function,
 *          After that should be parameters of that function
 *  @return	A raw thread pointer
 */
template<typename F,typename ... Args>
void 
enqueueToThread(F&& f, Args&& ... args) {
  // using Job = Callback<void>;
  // wrap the callable and its params into a void() job
  ThreadManager::instance().enqueue(f, std::forward<Args>(args)...);
//   ThreadManager::instance().enqueue(Job([fn = std::bind(std::forward<F>(f), std::forward<Args>(args)...)]() mutable {
//     fn();
//   }));
}

template<typename F, typename ... Args>
auto 
enqueueFuture(F&& f, Args&& ... args) {
  // wrap the callable and its params into a void() job
  return ThreadManager::instance().enqueueFuture(f, std::forward<Args>(args)...);
}


}

