/**
 * 
 */

#include <conio.h>

// Defining values for unit testing
#include <cyUnitTesting.h>
#include <cyArgumentParser.h>
#include <cyLogger.h>
#include <cyEvent.h>
#include <cyTime.h> 
#include <cyThread.h>
#include <cyDLLLoader.h>
#include <cyJSON.h>
// Using namespace for ease of use
using namespace CYLLENE_SDK;

void
testFunction(int foo) {
  std::cout << foo << std::endl;
}

int 
fooFunction(int bar) {
  Threading::sleepFor(Miliseconds(2000));
  return bar * 2;
}

/*
 *	@brief  Unit Testing main for Utilities
 *	@param  int argc - amount of arguments
 *	@param  char* argv[] - arguments
 *  @return	int32 should return 0 if everything went a ok
 */
int32
main(int argc, char* argv[])
{
  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int32 res = context.run();

  if (context.shouldExit()) {
    return res;
  }

  context.clearFilters();

  return res + EXIT_SUCCESS;
}

TEST_SUITE("Modules") {
  TEST_CASE("Logger") {
    Logger::startUp();
    Logger::instance().onLogAdded().addListener([](const Log& newLog) {
      std::cout << "New log added: " << newLog.getMsg() << std::endl;
      });
    Logger::instance().log("Test log message", LOG_VERBOSITY::E::eDEFAULT, LOG_CHANNEL::E::eDEFAULT, LOG_OUTPUT::E::eCONSOLE);
    Logger::instance().log("Test log message 2", LOG_VERBOSITY::E::eDEFAULT, LOG_CHANNEL::E::eDEFAULT, LOG_OUTPUT::E::eCONSOLE);
    Logger::instance().log("Test log message 3", LOG_VERBOSITY::E::eDEFAULT, LOG_CHANNEL::E::eDEFAULT, LOG_OUTPUT::E::eCONSOLE);
  
  
    Logger::instance().dump();
  }

  TEST_CASE("Time") {
    Time::startUp();
  }
}

TEST_SUITE("Argument Parser") {
  ArgumentParser parser;
}

TEST_SUITE("Crash Handler") {

}

TEST_SUITE("DLL Loading") {
}

TEST_SUITE("Events") {

  TEST_CASE("Basic event operations") {

    Event<void, int> e;
    int callbackValue = 0;

    Callback<void, int> callback = testFunction;

    SUBCASE("Add and invoke listener") {
      e.addListener(testFunction);
      e.invoke(42);
    }
  }

}

TEST_SUITE("JSON") {
  JSONDocument doc;
}

TEST_SUITE("File System") {
  
}

TEST_SUITE("Regex") {

}

TEST_SUITE("Smart Pointers") {

}

TEST_SUITE("Threads") {
  TEST_CASE("Basic Thread operations") {
    ThreadManager::startUp();
    enqueueToThread(testFunction, 12);
    auto future = enqueueFuture(fooFunction, 123);
    Threading::sleepFor(Miliseconds(1000));

    std::cout << future.get() << std::endl;
  }
}

TEST_SUITE("Time") {

}









