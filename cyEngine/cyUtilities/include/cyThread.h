/**
 * @file cyThread.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Thread.
 */

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

/**
 * @struct Threading
 * @brief Static helpers for basic thread operations.
 */
struct Threading
{
  /**
   * @brief Sleeps current thread for the given duration.
   * @param milliseconds Sleep duration.
   */
  static void
  sleepFor(Miliseconds milliseconds) {
    std::this_thread::sleep_for(milliseconds);
    
  }

  /**
   * @brief Yields execution of current thread.
   */
  static void
  yield() {
    std::this_thread::yield();
  }

  /**
   * @brief Sleeps until the given time point.
   * @tparam T Clock type.
   * @param duration Absolute wake-up time.
   */
  template<typename T>
  static void
  sleepUntil(TimePoint<T> duration) {
    std::this_thread::sleep_until(duration);
  }

  /**
   * @brief Returns the current thread identifier.
   * @return Current thread id.
   */
  static ThreadID
  getID() {
    return std::this_thread::get_id();
  }

};

// TODO: Check for Thread generation to have a manager

/**
 * @class ThreadManager
 * @brief Thread-pool style module for queued background jobs.
 */
class CY_UTILITY_EXPORT ThreadManager : public Module<ThreadManager>
{
public:

  virtual void
  onStartUp() override;

  virtual void
  onShutDown() override;

  /**
   * @brief Enqueues a callable and its arguments as a job.
   * @tparam F Callable type.
   * @tparam Args Argument types.
   * @param f Callable.
   * @param args Callable arguments.
   */
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

  /**
   * @brief Enqueues an already wrapped void job.
   * @param job Job callback.
   */
  void
  enqueue(const Callback<void>& job) {
    MULock lock(m_queueMutex);
    m_jobs.push(job);
    m_mutexCondition.notify_one();
  }

  /**
   * @brief Enqueues a callable and returns a future for its result.
   * @tparam F Callable type.
   * @tparam Args Argument types.
   * @param f Callable.
   * @param args Callable arguments.
   * @return Future for callable result.
   */
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
  

  /**
   * @brief Stops worker threads and pending processing.
   */
  void
  stop();

  /**
   * @brief Checks whether workers are currently processing jobs.
   * @return True if any worker is busy.
   */
  bool
  busy();

  /**
   * @brief Runs one update iteration on thread manager state.
   */
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

  /**
   * @brief Indicates whether workers should terminate.
   */
  bool 
  m_shouldTerminate = false;

  /**
   * @brief Mutex protecting queued jobs.
   */
  Mutex 
  m_queueMutex;

  /**
   * @brief Condition variable for worker wake-up.
   */
  ConditionVariable
  m_mutexCondition;
  
  /**
   * @brief Pending job queue.
   */
  Queue<Callback<void>>
  m_jobs;

};

/**
 * @brief Enqueues a task into the global thread manager.
 * @tparam F Callable type.
 * @tparam Args Argument types.
 * @param f Callable.
 * @param args Callable arguments.
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

/**
 * @brief Enqueues a task and returns a future for its result.
 * @tparam F Callable type.
 * @tparam Args Argument types.
 * @param f Callable.
 * @param args Callable arguments.
 * @return Future with callable return type.
 */
template<typename F, typename ... Args>
auto 
enqueueFuture(F&& f, Args&& ... args) {
  // wrap the callable and its params into a void() job
  return ThreadManager::instance().enqueueFuture(f, std::forward<Args>(args)...);
}

}

