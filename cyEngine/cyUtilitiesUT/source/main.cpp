/**
 * 
 */

#include <conio.h>

// Defining values for unit testing
#include <cyUnitTesting.h>
#include <cyLogger.h>
#include <cyDLLLoader.h>
// Using namespace for ease of use
using namespace CYLLENE_SDK;


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
  }
}


TEST_CASE("dllloading") {
}



