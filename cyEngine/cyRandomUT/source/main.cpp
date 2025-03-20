
#include <cyCrashHandler.h>
#include <cyLogger.h>
#include <cyRandom.h>
#include <cyTime.h>

#include <cyUnitTesting.h>

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

  CrashHandler::startUp();
  Logger::startUp();
  Time::startUp();

  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int32 res = context.run();

  if (context.shouldExit()) {
    return res;
  }

  context.clearFilters();

  return res + EXIT_SUCCESS;
}

TEST_CASE("[random] testing random module") {
  Random::init();


  float value = 0.0f;

  Benchmark().epochs(1000).run("1000 random floats",
    [&] {
    DONOTOPTIMIZE(value = Random::get<float>());
  }
  );

  // Time::instance().update();
  // for (int i = 0; i < 1000000; ++i) {
  //   value = Random::get<float>();
  // }
  // Time::instance().update();


  BENCHMARKEPOCHS("1000 random normalized floats", 1000,
    [&] {
      DONOTOPTIMIZE(value = Random::getNormalized());
      CHECK(value <= 1.0f);
    }
  );

  // Time::instance().update();
  // for (int i = 0; i < 1000000; ++i) {
  //   value = Random::getNormalized();
  //   CHECK(value <= 1.0f);
  // }
  // Time::instance().update();


  Benchmark().epochs(1000).run("1000 random floats ranged [876-1000]",
    [&] {
      DONOTOPTIMIZE(value = Random::getRanged<float>(876.0f, 1000.0f));
      CHECK((value <= 1000.0f && value >= 876.0f));
    }
  );

  // Time::instance().update();
  // for (int i = 0; i < 1000000; ++i) {
  //   value = Random::getRanged<float>(876.0f, 1000.0f);
  //   CHECK((value <= 1000.0f && value >= 876.0f));
  // }
  // Time::instance().update();

  Benchmark().epochs(1000).run("1000 random uint32 ranged [0-10]",
    [&] {
      DONOTOPTIMIZE(value = static_cast<float>(Random::getRanged<uint32>(0, 10)));
      CHECK((value <= 10 && value >= 0));
    }
  );

  // Time::instance().update();
  // for (int i = 0; i < 1000000; ++i) {
  //   value = static_cast<float>(Random::getRanged<uint32>(0, 10));
  //   CHECK((value <= 10));
  // }
  // Time::instance().update();
  Vector2f v2f;
  Benchmark().epochs(1000).run("1000 random Vector", 
    [&] {
      v2f = Random::getVector2f(5);
    }
  );

}
