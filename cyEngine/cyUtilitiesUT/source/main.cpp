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
#include <cyCrashHandler.h>
#include <cyDinamicLibrary.h>
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
#include <cyUUID.h>
#include <cyUtilities.h>
#include <cyMemoryPool.h>
#include <cyMemoryPoolHandler.h>
#include <cyTypeTraits.h>

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

struct PoolTestType { int value; PoolTestType() : value(0) {} PoolTestType(int v) : value(v) {} };
struct AnotherPoolType { float f; AnotherPoolType() : f(0.0f) {} };
struct UndeclaredType { char c; };

} // namespace

DECLARE_TYPE_TRAITS(PoolTestType)
DECLARE_TYPE_TRAITS(AnotherPoolType)

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

  TEST_CASE("QWord default construction is zero") {
    QWord q;
    CHECK(static_cast<int64>(q) == 0);
  }

  TEST_CASE("QWord from int32") {
    QWord q(static_cast<int32>(-7));
    CHECK(static_cast<int64>(q) == static_cast<uint64>(static_cast<int64>(-7)));
  }

  TEST_CASE("QWord from float truncates") {
    QWord q(3.99f);
    CHECK(static_cast<int64>(q) == 3);
  }

  TEST_CASE("QWord from double truncates") {
    QWord q(3.99);
    CHECK(static_cast<int64>(q) == 3);
  }

  TEST_CASE("QWord from bool") {
    QWord t(true);
    QWord f(false);
    CHECK(static_cast<int64>(t) == 1);
    CHECK(static_cast<int64>(f) == 0);
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
    Logger::instance().onLogAdded().removeAllListeners();

    std::atomic<int> callbackCount { 0 };
    uint64 conn = Logger::instance().onLogAdded().addListener([&callbackCount](const Log& newLog) {
      CHECK(newLog.getMsg().empty() == false);
      ++callbackCount;
    });

    Logger::instance().log("Utility logger test", LOG_VERBOSITY::E::eDEBUG, LOG_CHANNEL::E::eSYSTEM, LOG_OUTPUT::E::eDEFAULT);
    CHECK(callbackCount.load() >= 1);

    Logger::instance().onLogAdded().removeListener(conn);
  }

  TEST_CASE("Log getters") {
    Log entry("test msg", LOG_VERBOSITY::E::eWARNING, LOG_CHANNEL::E::eSYSTEM, LOG_OUTPUT::E::eCONSOLE);
    CHECK(String(entry.getMsg()) == "test msg");
    CHECK(entry.getVerbosity() == LOG_VERBOSITY::E::eWARNING);
    CHECK(entry.getChannel() == LOG_CHANNEL::E::eSYSTEM);
  }

  TEST_CASE("Log toString with active Time") {
    if (!Time::isStarted()) {
      Time::startUp();
    }
    Log entry("msg", LOG_VERBOSITY::E::eDEBUG, LOG_CHANNEL::E::eSYSTEM);
    String str = entry.toString();
    CHECK(str.find("msg") != String::npos);
  }

  TEST_CASE("logDebug, logWarning, logError helpers") {
    if (!Logger::isStarted()) {
      Logger::startUp();
    }
    Logger::instance().onLogAdded().removeAllListeners();

    int count = 0;
    uint64 conn = Logger::instance().onLogAdded().addListener([&count](const Log&) {
      ++count;
    });

    Logger::instance().logDebug("debug msg", LOG_CHANNEL::E::eSYSTEM);
    Logger::instance().logWarning("warning msg", LOG_CHANNEL::E::eSYSTEM);
    Logger::instance().logError("error msg", LOG_CHANNEL::E::eSYSTEM);

    CHECK(count >= 3);

    Logger::instance().onLogAdded().removeListener(conn);
  }

  TEST_CASE("Logger init and clear") {
    if (!Logger::isStarted()) {
      Logger::startUp();
    }
    Logger::instance().clear();
    Logger::instance().log("after clear");
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

  TEST_CASE("addFlag and setFlagValue") {
    ArgumentParser parser;
    parser.addFlag("custom");
    CHECK(parser.hasFlag("custom") == true);
    CHECK(parser.getFlagValues("custom").size() == 0);

    parser.setFlagValue("custom", "value1");
    CHECK(parser.getFlagValues("custom").size() == 1);
    CHECK(parser.getFlagValues("custom")[0] == "value1");
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

  TEST_CASE("removeListener stops specific callback") {
    Event<void> e;
    int count1 = 0;
    int count2 = 0;
    Callback<void> cb1 = [&count1] { ++count1; };
    Callback<void> cb2 = [&count2] { ++count2; };

    uint64 id1 = e.addListener(cb1);
    e.addListener(cb2);
    e.invoke();
    CHECK(count1 == 1);
    CHECK(count2 == 1);

    e -= id1;
    e.invoke();
    CHECK(count1 == 1);
    CHECK(count2 == 2);
  }

  TEST_CASE("removeAllListeners clears all") {
    Event<void> e;
    int calls = 0;
    e += [&calls] { ++calls; };
    e += [&calls] { ++calls; };
    e.invoke();
    CHECK(calls == 2);

    e.removeAllListeners();
    e.invoke();
    CHECK(calls == 2);
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

  TEST_CASE("regexSearch no match returns false") {
    StringMatch match;
    String noMatchInput("no digits here!");
    Regex r(R"(\d+)");
    bool result = regexSearch(noMatchInput, match, r);
    CHECK(!result);
    CHECK(match.size() == 0);
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

    size_t fileSize = 0;
    unsigned char* data = FileSystem::openBinary(filePath, fileSize);
    CHECK(data != nullptr);
    CHECK(fileSize > 0);
    cy_deleteN(data, fileSize);

    FileSystem::deleteFolder(basePath);
    CHECK(FileSystem::exists(basePath) == false);
  }

  TEST_CASE("Directory paths return non-empty") {
    Path home = FileSystem::homeDir();
    CHECK(home.fullPath().empty() == false);

    Path tmp = FileSystem::tempDir();
    CHECK(tmp.fullPath().empty() == false);

    Path exe = FileSystem::getExecutablePath();
    CHECK(exe.fullPath().empty() == false);
  }

  TEST_CASE("Base64 encodes file content") {
    const String basePath = FileSystem::getWorkingDirectory().fullPath() + "/cyutilitiesut_b64";
    const String srcPath = basePath + "/source.txt";

    if (!FileSystem::exists(basePath)) {
      CHECK(FileSystem::createFolder(basePath) == true);
    }

    File src = FileSystem::createFile(srcPath);
    CHECK(src.writeFile("hello") == true);

    String encoded = FileSystem::toBase64(srcPath);
    CHECK(encoded.empty() == false);

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

  TEST_CASE("cy_alloc typed and array new/delete") {
    int* raw = cy_alloc<int>();
    CHECK(raw != nullptr);
    cy_free(raw);

    int* arr = cy_newN<int, GenAlloc>(5);
    CHECK(arr != nullptr);
    for (int i = 0; i < 5; ++i) {
      arr[i] = i * 2;
    }
    CHECK(arr[3] == 6);
    cy_deleteN<int, GenAlloc>(arr, 5);
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

  TEST_CASE("getFile and getLine") {
    Exception ex("Test", "err", "fn", "test.cpp", 42);
    CHECK(String(ex.getFile()) == "test.cpp");
    CHECK(ex.getLine() == 42);
  }

  TEST_CASE("Copy constructor preserves data") {
    Exception orig("OrigType", "original desc", "src", "file.cpp", 10);
    Exception copy(orig);
    CHECK(String(copy.getDescription()) == "original desc");
    CHECK(String(copy.getSource()) == "src");
    CHECK(copy.getLine() == 10);
  }

  TEST_CASE("Copy assignment preserves data") {
    Exception a("A", "first", "s1", "f1.cpp", 1);
    Exception b("B", "second", "s2", "f2.cpp", 2);
    b = a;
    CHECK(String(b.getDescription()) == "first");
    CHECK(String(b.getSource()) == "s1");
  }

  TEST_CASE("Named derived exception types") {
    NotImplementedException nie("not impl", "test", __FILE__, __LINE__);
    CHECK(String(nie.what()).find("NotImplementedException") != String::npos);

    IOException ioe("io fail", "test", __FILE__, __LINE__);
    CHECK(String(ioe.what()).find("IOException") != String::npos);

    InvalidStateException ise("bad state", "test", __FILE__, __LINE__);
    CHECK(String(ise.what()).find("InvalidStateException") != String::npos);

    InvalidParametersException ipe("bad param", "test", __FILE__, __LINE__);
    CHECK(String(ipe.what()).find("InvalidParametersException") != String::npos);
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

  TEST_CASE("SmartPtr construction, dereference and move") {
    SmartPtr<int> sp1(new int(25));
    CHECK(*sp1 == 25);

    SmartPtr<int> sp2(std::move(sp1));
    CHECK(*sp2 == 25);
    CHECK_FALSE(sp2.operator->() == nullptr);
  }

  TEST_CASE("SmartPtr reset") {
    SmartPtr<int> sp(new int(10));
    CHECK(*sp == 10);
    sp.reset(new int(20));
    CHECK(*sp == 20);
  }

  TEST_CASE("SmallPtr via SmartPtr::ptr") {
    SmartPtr<int> owner(new int(42));
    {
      SmallPtr<int> handle = owner.ptr();
      CHECK_FALSE(handle.expired());
      CHECK(*handle == 42);
    }
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

TEST_SUITE("UUID") {
  TEST_CASE("createRandom returns non-null UUID") {
    UUID u = UUID::createRandom();
    CHECK_FALSE(u.isNull());
  }

  TEST_CASE("toString format") {
    UUID u = UUID::createRandom();
    String str = u.toString();
    CHECK(str.size() == 36);
    CHECK(str.find('-') != String::npos);
  }

  TEST_CASE("null UUID properties") {
    const UUID& n = UUID::null();
    CHECK(n.isNull());
    CHECK(n.toString() == "00000000-0000-0000-0000-000000000000");
  }

  TEST_CASE("toBytes and fromBytes round-trip") {
    UUID original = UUID::createRandom();
    auto bytes = original.toBytes();
    UUID restored = UUID::fromBytes(bytes);
    CHECK(original == restored);
  }

  TEST_CASE("createFromName deterministic") {
    UUID u1 = UUID::createFromName("test-name");
    UUID u2 = UUID::createFromName("test-name");
    CHECK(u1 == u2);
    CHECK_FALSE(u1.isNull());
  }

  TEST_CASE("createFromName different inputs differ") {
    UUID u1 = UUID::createFromName("hello");
    UUID u2 = UUID::createFromName("world");
    CHECK(u1 != u2);
  }

  TEST_CASE("createFromName with custom namespace") {
    UUID ns = UUID::createRandom();
    UUID u1 = UUID::createFromName("data", ns);
    UUID u2 = UUID::createFromName("data", ns);
    CHECK(u1 == u2);
  }

  TEST_CASE("random UUIDs almost certainly differ") {
    UUID a = UUID::createRandom();
    UUID b = UUID::createRandom();
    CHECK_FALSE(a == b);
    CHECK(a != b);
  }

  TEST_CASE("std::hash is consistent") {
    UUID a = UUID::createRandom();
    UUID same = UUID::fromBytes(a.toBytes());
    std::hash<UUID> hasher;
    CHECK(hasher(a) == hasher(same));
  }

  TEST_CASE("ordering") {
    UUID null = UUID::null();
    UUID a = UUID::createRandom();
    bool checker = (null < a) || (a < null);
    CHECK(checker);
  }
}

TEST_SUITE("Dynamic Library") {
  TEST_CASE("Load system DLL and resolve symbol") {
#if CY_PLATFORM == CY_PLATFORM_WIN32
    DynamicLibrary lib("kernel32.dll");
    CHECK(lib.getName() == "kernel32.dll");

    void* symbol = lib.loadSymbol("GetCurrentProcessId");
    CHECK(symbol != nullptr);

    DYNLIBRESULT::E result = lib.unload();
    CHECK(result == DYNLIBRESULT::E::eSUCCESS);
#else
    CHECK(true);
#endif
  }
}

TEST_SUITE("Crash Handler") {
  TEST_CASE("lifecycle, stack trace, folder, dump and logging") {
    if (!Logger::isStarted()) {
      Logger::startUp();
    }
    if (!Time::isStarted()) {
      Time::startUp();
    }

    CHECK(CrashHandler::isStarted() == false);
    CrashHandler::startUp();
    CHECK(CrashHandler::isStarted() == true);

    String trace = CrashHandler::getStackTrace();
    CHECK(trace.empty() == false);

    Path folder = CrashHandler::instance().getCrashFolder();
    CHECK(folder.fullPath().empty() == false);
    CHECK(FileSystem::exists(folder.fullPath()) == true);

    Path dump = CrashHandler::instance().createDump("test message", "fake stack trace");
    CHECK(dump.fullPath().empty() == false);
    CHECK(FileSystem::exists(dump.fullPath()) == true);

    CrashHandler::instance().logErrorAndStackTrace("test type", "test desc", "func", "file.cpp", 42);
    CrashHandler::instance().logErrorAndStackTrace("simple message", "stack content");

    FileSystem::deleteFile(dump.fullPath());
    FileSystem::deleteFolder(folder.fullPath());

    CrashHandler::shutDown();
    CHECK(CrashHandler::isStarted() == false);
  }
}

TEST_SUITE("Memory Pool") {
  TEST_CASE("Default state") {
    MemoryPool<int> pool;
    CHECK(pool.getCapacity() == 0);
    CHECK(pool.getAllocatedCount() == 0);
    CHECK(pool.isFull());  // 0-capacity pool has 0 free slots
  }

  TEST_CASE("Initialize and allocate") {
    MemoryPool<int> pool;
    pool.initialize(5);
    CHECK(pool.getCapacity() == 5);
    CHECK(pool.getAllocatedCount() == 0);

    int* a = pool.allocate(42);
    CHECK(a != nullptr);
    CHECK(*a == 42);
    CHECK(pool.getAllocatedCount() == 1);
    CHECK_FALSE(pool.isFull());

    (void)pool.allocate(100);
    (void)pool.allocate(200);
    (void)pool.allocate(300);
    (void)pool.allocate(400);
    CHECK(pool.isFull());
    CHECK(pool.getAllocatedCount() == 5);

    // Full pool returns nullptr
    int* f = pool.allocate(500);
    CHECK(f == nullptr);

    // Deallocate and allocate again
    pool.deallocate(a);
    CHECK(pool.getAllocatedCount() == 4);
    CHECK_FALSE(pool.isFull());

    int* g = pool.allocate(999);
    CHECK(g != nullptr);
    CHECK(*g == 999);
    CHECK(pool.isFull());

    pool.freePool();
  }

  TEST_CASE("Safe deallocate edge cases") {
    MemoryPool<int> pool;
    pool.initialize(3);

    // nullptr deallocate is safe
    pool.deallocate(nullptr);

    // Out-of-pool pointer is ignored
    int standalone = 0;
    pool.deallocate(&standalone);

    // Double deallocate is safe
    int* p = pool.allocate(1);
    pool.deallocate(p);
    pool.deallocate(p);
    CHECK(pool.getAllocatedCount() == 0);

    pool.freePool();
  }

  TEST_CASE("Clear keeps capacity") {
    MemoryPool<int> pool;
    pool.initialize(5);

    (void)pool.allocate(1);
    (void)pool.allocate(2);
    CHECK(pool.getAllocatedCount() == 2);

    pool.clear();
    CHECK(pool.getAllocatedCount() == 0);
    CHECK(pool.getCapacity() == 5);

    // Can allocate after clear
    int* p = pool.allocate(3);
    CHECK(p != nullptr);
    CHECK(*p == 3);

    pool.freePool();
  }

  TEST_CASE("getNextElement") {
    MemoryPool<int> pool;
    pool.initialize(3);

    int* next = pool.getNextElement();
    CHECK(next != nullptr);

    (void)pool.allocate();
    CHECK(pool.getNextElement() != nullptr);

    (void)pool.allocate();
    (void)pool.allocate();
    CHECK(pool.getNextElement() == nullptr);  // full

    pool.freePool();
  }

  TEST_CASE("getMemoryUsage") {
    MemoryPool<int> pool;
    pool.initialize(10);
    CHECK(pool.getMemoryUsage() > 0);
    pool.freePool();
  }

  TEST_CASE("Initialize zero size") {
    MemoryPool<int> pool;
    pool.initialize(0);
    CHECK(pool.getCapacity() == 0);
    CHECK(pool.getAllocatedCount() == 0);
    CHECK(pool.isFull());
    CHECK(pool.getNextElement() == nullptr);
    pool.freePool();
  }
}

TEST_SUITE("Memory Pool Handler") {
  TEST_CASE("Full lifecycle with pools") {
    MemoryPoolHandler::startUp(32);

    MemoryPoolHandler::instance().registerPool<PoolTestType>(4);
    CHECK(MemoryPoolHandler::instance().hasPool<PoolTestType>());
    CHECK_FALSE(MemoryPoolHandler::instance().hasPool<AnotherPoolType>());

    MemoryPoolHandler::instance().registerPool<AnotherPoolType>(8);
    CHECK(MemoryPoolHandler::instance().hasPool<AnotherPoolType>());

    CHECK(MemoryPoolHandler::instance().getCapacity<PoolTestType>() == 4);
    CHECK(MemoryPoolHandler::instance().getCapacity<AnotherPoolType>() == 8);
    CHECK(MemoryPoolHandler::instance().getDefaultCapacity() == 32);

    MemoryPool<PoolTestType>& pool = MemoryPoolHandler::instance().pool<PoolTestType>();
    PoolTestType* p1 = pool.allocate(42);
    CHECK(p1 != nullptr);
    CHECK(p1->value == 42);
    CHECK(MemoryPoolHandler::instance().getAllocatedCount<PoolTestType>() == 1);

    PoolTestType* p2 = pool.allocate(100);
    CHECK(p2 != nullptr);
    CHECK(MemoryPoolHandler::instance().getAllocatedCount<PoolTestType>() == 2);

    CHECK(MemoryPoolHandler::instance().getMemoryUsage<PoolTestType>() > 0);
    CHECK(MemoryPoolHandler::instance().getTotalMemoryUsage() > 0);

    MemoryPoolHandler::instance().clear<PoolTestType>();
    CHECK(MemoryPoolHandler::instance().getAllocatedCount<PoolTestType>() == 0);

    PoolTestType* p3 = pool.allocate(77);
    UUID fooId = TypeTraits<PoolTestType>::getTypeId();
    MemoryPoolHandler::instance().deallocate(fooId, p3);
    CHECK(MemoryPoolHandler::instance().getAllocatedCount<PoolTestType>() == 0);

    MemoryPoolHandler::instance().reset();
    CHECK(MemoryPoolHandler::instance().getAllocatedCount<PoolTestType>() == 0);
    CHECK(MemoryPoolHandler::instance().getAllocatedCount<AnotherPoolType>() == 0);

    MemoryPoolHandler::shutDown();
  }
}

TEST_SUITE("Windows Utilities") {
  TEST_CASE("Platform type exists") {
#if CY_PLATFORM == CY_PLATFORM_WIN32
    CHECK(std::is_base_of<PlatformUtils, WindowsUtils>::value);
    CHECK(sizeof(WindowsUtils) > 0);
#endif
    CHECK(true);
  }
}

TEST_SUITE("Type Traits") {
  TEST_CASE("Default trait for undeclared type") {
    CHECK(String(TypeTraits<UndeclaredType>::getTypeName()) == "Unknown");
    CHECK(TypeTraits<UndeclaredType>::getTypeId().isNull());
  }

  TEST_CASE("Declared trait properties") {
    CHECK(String(TypeTraits<PoolTestType>::getTypeName()) == "PoolTestType");
    CHECK_FALSE(TypeTraits<PoolTestType>::getTypeId().isNull());
  }

  TEST_CASE("Different types get different UUIDs") {
    const UUID& id1 = TypeTraits<PoolTestType>::getTypeId();
    const UUID& id2 = TypeTraits<AnotherPoolType>::getTypeId();
    CHECK(id1 != id2);
  }

  TEST_CASE("UUID is deterministic") {
    const UUID& id1 = TypeTraits<PoolTestType>::getTypeId();
    const UUID& id2 = TypeTraits<PoolTestType>::getTypeId();
    CHECK(id1 == id2);
  }
}
