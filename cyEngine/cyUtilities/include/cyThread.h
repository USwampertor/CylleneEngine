#pragma once

#include "cyUtilitiesPrerequisites.h"

#include "cyModule.h"
#include "cySmartPointers.h"

namespace CYLLENE_SDK
{

// TODO: Check for Thread generation to have a manager

/*
 *	@class	ThreadManager
 *	@brief	A module to create Threads and have a reference to them to safely manage
 *          and dispose them when not needed anymore
 *
 */
class ThreadManager : Module<ThreadManager>
{
public:

  virtual void
  onStartUp() override {

  }

  /*
   *	@brief	Creates a thread and returns a shared pointer to it
   *	@param	F&& f function to run in the thread
   *	@param	Args ... args the object to manage the function,
   *          After that should be parameters of that function
   *  @return	a SharedPointer<Thread> that is already saved in the manager
   */
  template<typename F, typename ... Args>
  SharedPointer<Thread>
  create(F&& f, Args ... args) {
    m_threads.push_back(MakeSharedObject<Thread>(f, std::forward<Args>(args)...));
    return m_threads.back();
  }

private:

  /**
   * The vector of threads
   */
  Vector<SharedPointer<Thread>> m_threads;
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
Thread* MakeThread(F&& f, Args ... args) {
  return ThreadManager::instance().create(f, std::forward<Args>(args)...).get();
}


}

