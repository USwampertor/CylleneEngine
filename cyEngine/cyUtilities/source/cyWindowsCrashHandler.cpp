
#if CY_PLATFORM == CY_PLATFORM_WIN32
# include "cyWindows.h"
# include <Psapi.h>

//Disable warning in VS2015 that's not under my control
// Thanks Prince
#pragma warning(disable : 4091)
#	include <DbgHelp.h>
#pragma warning(default : 4091)

#endif

#include "cyUtilitiesPrerequisites.h"

#include "cyCrashHandler.h"
#include "cyDLLLoader.h"
#include "cyException.h"
#include "cyFileSystem.h"
#include "cyLogger.h"
#include "cyMath.h"

namespace CYLLENE_SDK {

#if CY_PLATFORM == CY_PLATFORM_WIN32

  // This defines the name of the dump file
  static const char* s_MiniDumpName = "CylleneDump.dmp";

  uint32
  getRawStackTrace(CONTEXT context, uint64 stackTrace[CY_MAX_STACKTRACE_DEPTH]) {
    HANDLE hProcess = GetCurrentProcess();
    HANDLE hThread = GetCurrentThread();
    uint32 machineType;

    STACKFRAME64 stackFrame;
    memset(&stackFrame, 0, sizeof(stackFrame));

    stackFrame.AddrPC.Mode = AddrModeFlat;
    stackFrame.AddrStack.Mode = AddrModeFlat;
    stackFrame.AddrFrame.Mode = AddrModeFlat;

#if CY_ARCH_TYPE == CY_ARCHITECTURE_x86_64
    stackFrame.AddrPC.Offset = context.Rip;
    stackFrame.AddrStack.Offset = context.Rsp;
    stackFrame.AddrFrame.Offset = context.Rbp;
    machineType = IMAGE_FILE_MACHINE_AMD64;
#else
    stackFrame.AddrPC.Offset = context.Eip;
    stackFrame.AddrStack.Offset = context.Esp;
    stackFrame.AddrFrame.Offset = context.Ebp;
    machineType = IMAGE_FILE_MACHINE_I386;
#endif

    uint32 numEntries = 0;
    while (true) {
      if (!StackWalk64(machineType,
                       hProcess,
                       hThread,
                       &stackFrame,
                       &context,
                       nullptr,
                       SymFunctionTableAccess64,
                       SymGetModuleBase64,
                       nullptr)) {
        break;
      }

      if (CY_MAX_STACKTRACE_DEPTH > numEntries) {
        stackTrace[numEntries] = stackFrame.AddrPC.Offset;
      }

      ++numEntries;

      if (0 == stackFrame.AddrPC.Offset || 0 == stackFrame.AddrFrame.Offset) {
        break;
      }
    }

    return numEntries;
  }

  String
  getWindowsStackTrace(CONTEXT context, uint32 skip = 0) {
    uint64 rawStackTrace[CY_MAX_STACKTRACE_DEPTH];
    uint32 numEntries = getRawStackTrace(context, rawStackTrace);

    numEntries = Math::min(static_cast<uint32>(CY_MAX_STACKTRACE_DEPTH), numEntries);

    uint32 bufferSize = sizeof(PIMAGEHLP_SYMBOL64) + CY_MAX_STACKTRACE_NAME_BYTES;
    auto buffer = static_cast<uint8*>(cy_alloc(bufferSize));

    auto symbol = reinterpret_cast<PIMAGEHLP_SYMBOL64>(buffer);
    symbol->SizeOfStruct = bufferSize;
    symbol->MaxNameLength = CY_MAX_STACKTRACE_NAME_BYTES;

    HANDLE hProcess = GetCurrentProcess();

    StringStream outputStream;
    for (uint32 i = skip; i < numEntries; ++i) {
      if (i > skip) {
        outputStream << std::endl;
      }

      DWORD64 funcAddress = rawStackTrace[i];

      //Output function name
      DWORD64 dummy;
      if (SymGetSymFromAddr64(hProcess, funcAddress, &dummy, symbol)) {
        outputStream << Utils::format("%s() - ", symbol->Name);
      }

      //Output file name and line
      IMAGEHLP_LINE64	lineData;
      lineData.SizeOfStruct = sizeof(lineData);

      String addressString = Utils::toString(funcAddress);

      DWORD column;
      if (SymGetLineFromAddr64(hProcess, funcAddress, &column, &lineData)) {
        Path filePath = lineData.FileName;
        outputStream << Utils::format("0x%llx File[%s:%u (%u)]",
          addressString,
          filePath.fileName(),
          static_cast<uint32>(lineData.LineNumber),
          static_cast<uint32>(column));
      }
      else {
        outputStream << Utils::format("0x%llx", addressString);
      }

      //Output module name
      IMAGEHLP_MODULE64 moduleData;
      moduleData.SizeOfStruct = sizeof(moduleData);

      if (SymGetModuleInfo64(hProcess, funcAddress, &moduleData)) {
        Path filePath = moduleData.ImageName;
        outputStream << Utils::format(" Module[%s]", filePath.fileName());
      }
    }

    cy_free(buffer);

    return outputStream.str();
  }

  using EnumProcessModulesType = bool(WINAPI*)(HANDLE hProcess,
                                               HMODULE* lphModule,
                                               DWORD cb,
                                               LPDWORD lpcbNeeded);

  using GetModuleBaseNameType = DWORD(WINAPI*)(HANDLE hProcess,
                                               HMODULE hModule,
                                               LPSTR lpBaseName,
                                               DWORD nSize);

  using GetModuleFileNameExType = DWORD(WINAPI*)(HANDLE hProcess,
                                                 HMODULE hModule,
                                                 LPSTR lpFilename,
                                                 DWORD nSize);

  using GetModuleInformationType = bool(WINAPI*)(HANDLE hProcess,
                                                 HMODULE hModule,
                                                 LPMODULEINFO lpmodinfo,
                                                 DWORD cb);

  static EnumProcessModulesType g_enumProcessModules;
  static GetModuleBaseNameType g_getModuleBaseName;
  static GetModuleFileNameExType g_getModuleFileNameEx;
  static GetModuleInformationType g_getModuleInformation;
  static bool g_symbolsLoaded = false;

  void
  loadPSAPISymbols() {

    g_enumProcessModules = 
      reinterpret_cast<EnumProcessModulesType>(DLLLoader::load("psapi.dll",
                                                          "EnumProcessModules"));
    g_getModuleBaseName = 
      reinterpret_cast<GetModuleBaseNameType>(DLLLoader::load("psapi.dll",
                                                         "GetModuleBaseNameA"));
    g_getModuleFileNameEx = 
      reinterpret_cast<GetModuleFileNameExType>(DLLLoader::load("psapi.dll",
                                                           "GetModuleFileNameExA"));
    g_getModuleInformation = 
      reinterpret_cast<GetModuleInformationType>(DLLLoader::load("psapi.dll",
                                                            "GetModuleInformation"));
    if (g_enumProcessModules == nullptr   ||
        g_getModuleBaseName == nullptr    || 
        g_getModuleFileNameEx == nullptr  || 
        g_getModuleInformation == nullptr) {
      Logger::instance().logWarning("The PSAPI symbols were not loaded correctly");
    }
  }

  void
  loadSymbols() {
    if (g_symbolsLoaded) {
      return;
    }

    HANDLE hProcess = GetCurrentProcess();
    uint32 options = SymGetOptions();

    options |= SYMOPT_LOAD_LINES;
    options |= SYMOPT_EXACT_SYMBOLS;
    options |= SYMOPT_UNDNAME;
    options |= SYMOPT_FAIL_CRITICAL_ERRORS;
    options |= SYMOPT_NO_PROMPTS;

    SymSetOptions(options);
    if (!SymInitialize(hProcess, nullptr, false)) {

      CY_LOG("SymInitialize failed. Error code: %d", 
             LOG_VERBOSITY::E::eERROR, 
             LOG_CHANNEL::E::eSYSTEM,
             static_cast<uint32>(GetLastError()));
      return;
    }

    DWORD bufferSize;
    g_enumProcessModules(hProcess, nullptr, 0, &bufferSize);

    auto* modules = static_cast<HMODULE*>(cy_alloc(bufferSize));
    g_enumProcessModules(hProcess, modules, bufferSize, &bufferSize);

    uint32 numModules = bufferSize / sizeof(HMODULE);
    for (uint32 i = 0; i < numModules; ++i) {
      MODULEINFO moduleInfo;

      char moduleName[CY_MAX_STACKTRACE_NAME_BYTES];
      char imageName[CY_MAX_STACKTRACE_NAME_BYTES];

      g_getModuleInformation(hProcess, modules[i], &moduleInfo, sizeof(moduleInfo));
      g_getModuleFileNameEx(hProcess, modules[i], imageName, CY_MAX_STACKTRACE_NAME_BYTES);
      g_getModuleBaseName(hProcess, modules[i], moduleName, CY_MAX_STACKTRACE_NAME_BYTES);

      char pdbSearchPath[CY_MAX_STACKTRACE_NAME_BYTES];
      char* fileName = nullptr;
      GetFullPathName(moduleName, CY_MAX_STACKTRACE_NAME_BYTES, pdbSearchPath, &fileName);
      *fileName = '\0';

      SymSetSearchPath(GetCurrentProcess(), pdbSearchPath);

      DWORD64 moduleAddress = SymLoadModule64(hProcess,
        modules[i],
        imageName,
        moduleName,
        (DWORD64)moduleInfo.lpBaseOfDll,
        static_cast<DWORD>(moduleInfo.SizeOfImage));

      if (0 != moduleAddress) {
        IMAGEHLP_MODULE64 imageInfo;
        memset(&imageInfo, 0, sizeof(imageInfo));
        imageInfo.SizeOfStruct = sizeof(imageInfo);

        if (!SymGetModuleInfo64(GetCurrentProcess(), moduleAddress, &imageInfo)) {
          CY_LOG("Failed retrieving module info for module: %s. \n Error code %d", 
                 LOG_VERBOSITY::E::eWARNING, 
                 LOG_CHANNEL::E::eSYSTEM, 
                 moduleName, 
                 static_cast<uint32>(GetLastError()));
        }
        else {
#if CY_DEBUG_DETAILED_SYMBOLS
          if (SymNone == imageInfo.SymType) {

            CY_LOG("Failed loading symbols for module: %s", 
                   LOG_VERBOSITY::E::eWARNING, 
                   LOG_CHANNEL::E::eSYSTEM, 
                   moduleName, 
                   static_cast<uint32>(GetLastError()));
          }
#endif
        }
      }
      else {
        CY_LOG(("Failed loading module %s. " \
                "Error code: %d. Search path: %s. " \
                "Image name: %s"),
                LOG_VERBOSITY::E::eWARNING, 
                LOG_CHANNEL::E::eSYSTEM, 
                moduleName, 
                static_cast<uint32>(GetLastError()),
                pdbSearchPath, 
                imageName);
      }
    }

    cy_free(modules);
    g_symbolsLoaded = true;
  }

  String 
  getWindowsExceptionMessage(EXCEPTION_RECORD* record) {
    String exceptionAddress = Utils::toString((uint64)record->ExceptionAddress);
    String format;
    switch (record->ExceptionCode)
    {
    case EXCEPTION_ACCESS_VIOLATION:
    {
      DWORD_PTR violatedAddress = 0;
      if (2 == record->NumberParameters) {
        if (0 == record->ExceptionInformation[0]) {
          format = "Unhandled exception at 0x%llx."\
                   "Access violation reading 0x%llx.";
        }
        else if (8 == record->ExceptionInformation[0]) {
          format = "Unhandled exception at 0x%llx."\
                   "Access violation DEP 0x%x.";
        }
        else {
          format = "Unhandled exception at 0x%llx."\
                   "Access violation writing 0x%llx.";
        }

        violatedAddress = record->ExceptionInformation[1];
      }
      else {
        format = "Unhandled exception at 0x%llx."\
                 "Access violation.";
      }

      String violatedAddressStr = Utils::toString((uint64)violatedAddress);
      return Utils::format(format, exceptionAddress, violatedAddressStr);
    }
    case EXCEPTION_IN_PAGE_ERROR:
    {
      DWORD_PTR violatedAddress = 0;
      DWORD_PTR code = 0;
      if (3 == record->NumberParameters) {
        if (0 == record->ExceptionInformation[0]) {
          format = "Unhandled exception at 0x%llx."\
                   "Page fault reading 0x%llx with code 0x%llx.";
        }
        else if (8 == record->ExceptionInformation[0]) {
          format = "Unhandled exception at 0x%llx."\
                   "Page fault DEP 0x%llx with code 0x%llx.";
        }
        else {
          format = "Unhandled exception at 0x%llx."\
                   "Page fault writing 0x{1} with code 0x%llx.";
        }

        violatedAddress = record->ExceptionInformation[1];
        code = record->ExceptionInformation[3];
      }
      else {
        format = "Unhandled exception at 0x%llx."\
                 "Page fault.";
      }

      String violatedAddressStr = Utils::toString((uint64)violatedAddress);
      String codeStr = Utils::toString((uint64)code);
      return Utils::format(format, exceptionAddress, violatedAddressStr, codeStr);
    }
    case STATUS_ARRAY_BOUNDS_EXCEEDED:
    {
      format = "Unhandled exception at 0x%llx."\
        "Attempting to access an out of range array element.";
      return Utils::format(format, exceptionAddress);
    }
    case EXCEPTION_DATATYPE_MISALIGNMENT:
    {
      format = "Unhandled exception at 0x%llx."\
        "Attempting to access missaligned data.";
      return Utils::format(format, exceptionAddress);
    }
    case EXCEPTION_FLT_DENORMAL_OPERAND:
    {
      format = "Unhandled exception at 0x%llx."\
        "Floating point operand too small.";
      return Utils::format(format, exceptionAddress);
    }
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
    {
      format = "Unhandled exception at 0x%llx."\
        "Floating point operation attempted to divide by zero.";
      return Utils::format(format, exceptionAddress);
    }
    case EXCEPTION_FLT_INVALID_OPERATION:
    {
      format = "Unhandled exception at 0x%llx."\
        "Floating point invalid operation.";
      return Utils::format(format, exceptionAddress);
    }
    case EXCEPTION_FLT_OVERFLOW:
    {
      format = "Unhandled exception at 0x%llx."\
        "Floating point overflow.";
      return Utils::format(format, exceptionAddress);
    }
    case EXCEPTION_FLT_UNDERFLOW:
    {
      format = "Unhandled exception at 0x%llx."\
        "Floating point underflow.";
      return Utils::format(format, exceptionAddress);
    }
    case EXCEPTION_FLT_STACK_CHECK:
    {
      format = "Unhandled exception at 0x%llx."\
        "Floating point stack overflow/underflow.";
      return Utils::format(format, exceptionAddress);
    }
    case EXCEPTION_ILLEGAL_INSTRUCTION:
    {
      format = "Unhandled exception at 0x%llx."\
        "Attempting to execute an illegal instruction.";
      return Utils::format(format, exceptionAddress);
    }
    case EXCEPTION_PRIV_INSTRUCTION:
    {
      format = "Unhandled exception at 0x%llx."\
        "Attempting to execute a private instruction.";
      return Utils::format(format, exceptionAddress);
    }
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
    {
      format = "Unhandled exception at 0x%llx."\
        "Integer operation attempted to divide by zero.";
      return Utils::format(format, exceptionAddress);
    }
    case EXCEPTION_INT_OVERFLOW:
    {
      format = "Unhandled exception at 0x%llx."\
        "Integer operation result has overflown.";
      return Utils::format(format, exceptionAddress);
    }
    case EXCEPTION_STACK_OVERFLOW:
    {
      format = "Unhandled exception at 0x%llx."\
        "Stack overflow.";
      return Utils::format(format, exceptionAddress);
    }
    default:
    {
      format = "Unhandled exception at 0x%llx."\
        "Code 0x%llx.";

      String exceptionCode = Utils::toString((uint32)record->ExceptionCode);
      return Utils::format(format, exceptionAddress, exceptionCode);
    }

    } //switch
  }

  struct MiniDumpParams
  {
    Path filePath;
    EXCEPTION_POINTERS* exceptionData;
  };

  DWORD
  CALLBACK createPlatformDumpWorker(void* data) {
    MiniDumpParams* params = static_cast<MiniDumpParams*>(data);

    const String cStr = params->filePath.fullPath();
    HANDLE hFile = CreateFile(cStr.c_str(),
                              GENERIC_WRITE,
                              0,
                              nullptr,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              nullptr);

    if (INVALID_HANDLE_VALUE != hFile) {
      MINIDUMP_EXCEPTION_INFORMATION DumpExceptionInfo;
      DumpExceptionInfo.ThreadId = GetCurrentThreadId();
      DumpExceptionInfo.ExceptionPointers = params->exceptionData;
      DumpExceptionInfo.ClientPointers = false;

      MiniDumpWriteDump(GetCurrentProcess(),
                        GetCurrentProcessId(),
                        hFile,
                        MiniDumpNormal,
                        &DumpExceptionInfo,
                        nullptr,
                        nullptr);
      CloseHandle(hFile);
    }

    return 0;
  }

  void
  createPlatformDump(const Path& filePath, void* exceptionData) {
    MiniDumpParams param = { filePath, static_cast<EXCEPTION_POINTERS*>(exceptionData) };

    //Write mini dump on a second thread in order to preserve the current thread's call stack
    DWORD threadId = 0;
    HANDLE hThread = CreateThread(nullptr,
                                  0,
                                  &createPlatformDumpWorker,
                                  &param,
                                  0,
                                  &threadId);

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
  }

  void
  popupErrorMessage(const String& msg, const Path& folder, const Path& dumpFile) {
    const String errorMessage = msg +
      "\n\nFor more information check the crash report located at:\n " +
      folder.fullPath();
    auto response = MessageBox(nullptr, errorMessage.c_str(), "Cyllene Engine Error!", MB_YESNO);
    if (response == IDYES) {
      CrashHandler::instance().openCrashHandlerApp(Utils::format("-p %s", dumpFile.fullPath()));
    }
  }

  struct CrashHandler::Data
  {
    Mutex mutex;
  };

  CrashHandler::CrashHandler() {
    m_data = cy_new<Data>();
  }

  CrashHandler::~CrashHandler() {
    cy_delete(m_data);
  }

  void
  CrashHandler::createReport(const String& type,
                             const String& strDescription,
                             const String& strFunction,
                             const String& strFile,
                             uint32 nLine) const {
    //Win32 debug methods are not thread safe
    MutexLock lock(m_data->mutex);

    logErrorAndStackTrace(type, strDescription, strFunction, strFile, nLine);
    Logger::instance().dump();
    Path folderPath = CrashHandler::instance().getCrashFolder();

    StringStream errorMessage;
    errorMessage << "  - Error: " << type << std::endl;
    errorMessage << "  - Description: " << strDescription << std::endl;
    errorMessage << "  - In function: " << strFunction << std::endl;
    errorMessage << "  - In file: " << strFile << ":" << nLine;

    createPlatformDump(folderPath.path() + "/" + String(s_MiniDumpName), nullptr);
    Path p = CrashHandler::instance().createDump(errorMessage.str(), getStackTrace());

    popupErrorMessage(m_errorMessage, folderPath, p);
  }

  int32
  CrashHandler::createReport(void* exception) const {
    EXCEPTION_POINTERS* exceptionData = static_cast<EXCEPTION_POINTERS*>(exception);

    //Win32 debug methods are not thread safe
    MutexLock lock(m_data->mutex);

    loadPSAPISymbols();
    loadSymbols();

    logErrorAndStackTrace(getWindowsExceptionMessage(exceptionData->ExceptionRecord),
                          getWindowsStackTrace(*exceptionData->ContextRecord, 0));

    Logger::instance().dump();
    Path folderPath = CrashHandler::instance().getCrashFolder();
    createPlatformDump(folderPath.path() + "/" + String(s_MiniDumpName), nullptr);
    Path p = CrashHandler::instance().createDump(getWindowsExceptionMessage(exceptionData->ExceptionRecord),
                                                 getWindowsStackTrace(*exceptionData->ContextRecord, 0));

    popupErrorMessage(m_errorMessage, CrashHandler::instance().getCrashFolder(), p);
    return EXCEPTION_EXECUTE_HANDLER;
  }

  String
  CrashHandler::getStackTrace() {
    CONTEXT context;
    RtlCaptureContext(&context);
    loadPSAPISymbols();
    loadSymbols();

    return getWindowsStackTrace(context);

  }

#endif
}