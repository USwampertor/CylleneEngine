/**
 * @file main.cpp
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Unit tests for cyUtilities systems.
 */

#include <atomic>
#include <fstream>
#include <string>

#include <cyUnitTesting.h>

#include <cyArgumentParser.h>
#include <cyDLLLoader.h>
#include <cyEvent.h>
#include <cyException.h>
#include <cyFileSystem.h>
#include <cyJSON.h>
#include <cyLogger.h>
#include <cyMemoryAllocator.h>
#include <cyModule.h>
#include <cyPlatformTypes.h>
#include <cyRegex.h>
#include <cySmartPointers.h>
#include <cyThread.h>
#include <cyTime.h>
#include <cyUtilities.h>

using namespace CYLLENE_SDK;

namespace {

void
testFunction(int value) {
  (void)value;
}

int
fooFunction(int value) {
  Threading::sleepFor(Miliseconds(10));
  return value * 2;
}

class TestModule : public Module<TestModule>
{
public:
  int state = 0;

  void
  onStartUp() override {
    state = 1;
  }

  void
  onShutDown() override {
    state = 2;
  }
};

} // namespace

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

TEST_SUITE("Platform Types") {
  TEST_CASE("QWord basic conversion") {
    QWord q(static_cast<uint64>(42));
    CHECK(static_cast<int64>(q) == 42);
  }
}

TEST_SUITE("Platform Utilities") {
  TEST_CASE("String and formatting helpers") {
    CHECK(Utils::format("Value %d", 7) == "Value 7");
    CHECK(Utils::toString(123) == "123");
    CHECK(Utils::toLowerCase("AbC") == "abc");
    CHECK(Utils::toUpperCase("AbC") == "ABC");
    CHECK(Utils::isStringNumber("12345") == true);
    CHECK(Utils::isStringNumber("12a45") == false);
    CHECK(Utils::intToHex(255, true) == "0xFF");
  }
}

TEST_SUITE("Module System") {
  TEST_CASE("Basic module lifecycle") {
    CHECK(TestModule::isStarted() == false);
    TestModule::startUp();
    CHECK(TestModule::isStarted() == true);
    CHECK(TestModule::instance().state == 1);
    TestModule::shutDown();
    CHECK(TestModule::isStarted() == false);
  }
}

TEST_SUITE("Logger") {
  TEST_CASE("Log event callback and message data") {
    if (!Logger::isStarted()) {
      Logger::startUp();
    }

    std::atomic<int> callbackCount { 0 };
    Logger::instance().onLogAdded().addListener([&callbackCount](const Log& newLog) {
      CHECK(newLog.getMsg().empty() == false);
      ++callbackCount;
    });

    Logger::instance().log("Utility logger test", LOG_VERBOSITY::E::eDEBUG, LOG_CHANNEL::E::eSYSTEM, LOG_OUTPUT::E::eDEFAULT);
    CHECK(callbackCount.load() >= 1);
  }
}

TEST_SUITE("Time") {
  TEST_CASE("Date and delta time") {
    if (!Time::isStarted()) {
      Time::startUp();
    }

    Time::instance().update();
    Threading::sleepFor(Miliseconds(15));
    Time::instance().update();

    CHECK(Time::instance().deltaTime(DELTA_TYPE::E::eMILLISECOND) >= 0.0f);
    CHECK(Time::instance().deltaTime(DELTA_TYPE::E::eMICROSECOND) >= 0.0f);
    CHECK(Time::instance().deltaTime(DELTA_TYPE::E::eSECOND) >= 0.0f);

    Date local = Time::now();
    Date utc = Time::nowUTC();
    CHECK(local.toString("%Y").empty() == false);
    CHECK(utc.toString("%Y").empty() == false);
  }
}

TEST_SUITE("Argument Parser") {
  TEST_CASE("Parse from argv and string input") {
    ArgumentParser parser;

    const char* fakeArgv[] = { "app", "-name", "cyllene", "-mode", "test" };
    parser.parse(5, fakeArgv);
    CHECK(parser.hasFlag("name") == true);
    CHECK(parser.hasFlag("mode") == true);
    CHECK(parser.getFlagValues("name").size() == 1);
    CHECK(parser.getFlagValues("name")[0] == "cyllene");

    parser.parse("-path \"hello world\" 123");
    CHECK(parser.hasFlag("path") == true);
    CHECK(parser.getFlagValues("path").size() == 2);
    CHECK(parser.getFlagValues("path")[0] == "hello world");
    CHECK(parser.getFlagValues("path")[1] == "123");

    CHECK(parser.removeFlag("mode") == true);
    CHECK(parser.hasFlag("mode") == false);
  }
}

TEST_SUITE("Events") {
  TEST_CASE("Add invoke remove listeners") {
    Event<void, int> e;
    std::atomic<int> lastValue { 0 };

    Callback<void, int> callback = [&lastValue](int v) {
      lastValue = v;
    };

    e += callback;
    e.invoke(42);
    CHECK(lastValue.load() == 42);

    e.removeAllListeners();
  }
}

TEST_SUITE("Regex") {
  TEST_CASE("regexSearch helper") {
    String input = "value=123";
    StringMatch match;
    Regex r(R"(value=(\d+))");
    CHECK(regexSearch(input, match, r) == true);
    CHECK(match[1].str() == "123");
  }
}

TEST_SUITE("JSON") {
  TEST_CASE("Stringify and pretty stringify") {
    JSONDocument doc;
    doc.SetObject();
    auto& alloc = doc.GetAllocator();
    doc.AddMember("name", "cyllene", alloc);
    doc.AddMember("version", 1, alloc);

    String compact = doc.stringify();
    String pretty = doc.prettyString();
    CHECK(compact.find("\"name\"") != String::npos);
    CHECK(pretty.find("\n") != String::npos);
  }
}

TEST_SUITE("File System") {
  TEST_CASE("Create, write, read, delete temporary files") {
    const String basePath = FileSystem::getWorkingDirectory().fullPath() + "/cyutilitiesut_tmp";
    const String filePath = basePath + "/tmp.txt";

    if (!FileSystem::exists(basePath)) {
      CHECK(FileSystem::createFolder(basePath) == true);
    }

    CHECK(FileSystem::exists(basePath) == true);

    File file = FileSystem::createFile(filePath);
    CHECK(FileSystem::exists(file.path()) == true);
    CHECK(file.writeFile("hello utilities") == true);

    unsigned char* data = FileSystem::openBinary(filePath);
    CHECK(data != nullptr);
    delete[] data;

    FileSystem::deleteFolder(basePath);
    CHECK(FileSystem::exists(basePath) == false);
  }
}

TEST_SUITE("Memory Allocator") {
  TEST_CASE("Allocation helpers") {
    int* value = cy_new<int, GenAlloc>(7);
    CHECK(value != nullptr);
    CHECK(*value == 7);
    cy_delete<int, GenAlloc>(value);

    void* aligned = cy_alloc_aligned(64, 16);
    CHECK(aligned != nullptr);
    cy_free_aligned(aligned);

    void* aligned16 = cy_alloc_aligned16(64);
    CHECK(aligned16 != nullptr);
    cy_free_aligned16(aligned16);

    auto currentAllocs = MemoryCounter::getNumAllocs();
    auto currentFrees = MemoryCounter::getNumFrees();
    CHECK(currentAllocs >= 0);
    CHECK(currentFrees >= 0);
  }
}

TEST_SUITE("Exceptions") {
  TEST_CASE("Base and derived exception descriptions") {
    Exception ex("Exception", "desc", "source", __FILE__, __LINE__);
    CHECK(ex.getDescription() == "desc");
    CHECK(ex.getSource() == "source");
    CHECK(String(ex.what()).find("desc") != String::npos);

    FileNotFoundException fex("missing", "io", __FILE__, __LINE__);
    CHECK(fex.getDescription() == "missing");
    CHECK(String(fex.what()).find("FileNotFoundException") != String::npos);
  }
}

TEST_SUITE("Smart Pointers") {
  TEST_CASE("Standard smart pointer helpers") {
    auto uptr = makeUniquePtr<int>(42);
    CHECK(uptr != nullptr);
    CHECK(*uptr == 42);

    auto sptr = makeSharedPtr<int>(11);
    CHECK(sptr != nullptr);
    CHECK(*sptr == 11);
  }
}

TEST_SUITE("DLL Loading") {
  TEST_CASE("Load known OS symbol") {
#if CY_PLATFORM == CY_PLATFORM_WIN32
    void* symbol = DLLLoader::load("kernel32.dll", "GetCurrentProcessId", true);
    CHECK(symbol != nullptr);
#else
    CHECK(true);
#endif
  }
}

TEST_SUITE("Threads") {
  TEST_CASE("Queue jobs and futures") {
    if (!ThreadManager::isStarted()) {
      ThreadManager::startUp();
    }

    std::atomic<int> marker { 0 };
    ThreadManager::instance().enqueue([&marker]() {
      marker = 1;
    });

    auto future = enqueueFuture(fooFunction, 123);
    CHECK(future.get() == 246);

    for (int i = 0; i < 100 && marker.load() == 0; ++i) {
      Threading::sleepFor(Miliseconds(2));
    }
    CHECK(marker.load() == 1);

    ThreadManager::instance().stop();
    ThreadManager::shutDown();
  }
}
