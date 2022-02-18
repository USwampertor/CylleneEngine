#pragma once
#include <iostream>
#include <exception>
#include <stdexcept>

#include "cyUtilitiesPrerequisites.h"
#include "cyStdHeaders.h"
#include "cyUtilities.h"

#include "cyCrashHandler.h"

namespace CYLLENE_SDK {
using std::runtime_error;
using std::is_base_of;

#if CY_COMPILER == CY_COMPILER_MSVC
# pragma warning( push )
# pragma warning( disable : 4275 )
#endif

class Exception : public StdException
{
 public:
  Exception(const char* type, 
            const String& description, 
            const String& source)
    : m_typeName(type),
      m_description(description),
      m_source(source) {}

  Exception(const char* type,
            const String& description,
            const String& source,
            const char* file,
            long line)
    : m_line(line),
      m_typeName(type),
      m_description(description),
      m_source(source),
      m_file(file),
      m_fullDesc("") {}

  Exception(const Exception& rhs)
    : m_line(rhs.m_line),
      m_typeName(rhs.m_typeName),
      m_description(rhs.m_description),
      m_source(rhs.m_source),
      m_file(rhs.m_file),
      m_fullDesc(rhs.m_fullDesc) {}

  ~Exception() _NOEXCEPT = default;

  Exception&
    operator=(const Exception& rhs) {
    m_description = rhs.m_description;
    m_source = rhs.m_source;
    m_file = rhs.m_file;
    m_line = rhs.m_line;
    m_typeName = rhs.m_typeName;
    m_fullDesc = rhs.m_fullDesc;
    return *this;
  }

  /**
   * @brief Returns a string with the full description of the exception.
   * @note  The description contains the error number, the description
   *        supplied by the thrower, what routine threw the exception, and
   *        will also supply extra platform-specific information where applicable.
   */
  virtual const String&
  getFullDescription() const {
    if (m_fullDesc.empty()) {
      StringStream desc;
      desc << "GEENGINE EXCEPTION(" << m_typeName << "): " 
           << m_description << " in " << m_source;

      if (m_line > 0) {
        desc << " at " << m_file << " (line " << m_line << ")";
      }

      m_fullDesc = desc.str();
    }

    return m_fullDesc;
  }

  virtual const String&
    getSource() const {
    return m_source;
  }

  virtual const String&
    getFile() const {
    return m_file;
  }

  virtual long
    getLine() const {
    return m_line;
  }

  virtual const String&
    getDescription(void) const {
    return m_description;
  }

  const char*
    what() const _NOEXCEPT {
    return getFullDescription().c_str();
  }

protected:
  long m_line = 0;
  String m_typeName;
  String m_description;
  String m_source;
  String m_file;
  mutable String m_fullDesc;
};

class NotImplementedException : public Exception
{
public:
  NotImplementedException(const String& inDescription,
    const String& inSource,
    const char* inFile,
    long inLine)
    : Exception("NotImplementedException", inDescription, inSource, inFile, inLine) {}
};

class FileNotFoundException : public Exception
{
public:
  FileNotFoundException(const String& inDescription,
    const String& inSource,
    const char* inFile,
    long inLine)
    : Exception("FileNotFoundException", inDescription, inSource, inFile, inLine) {}
};

class IOException : public Exception
{
public:
  IOException(const String& inDescription,
    const String& inSource,
    const char* inFile,
    long inLine)
    : Exception("IOException", inDescription, inSource, inFile, inLine) {}
};

class InvalidStateException : public Exception
{
public:
  InvalidStateException(const String& inDescription,
    const String& inSource,
    const char* inFile,
    long inLine)
    : Exception("InvalidStateException", inDescription, inSource, inFile, inLine) {}
};

class InvalidParametersException : public Exception
{
public:
  InvalidParametersException(const String& inDescription,
    const String& inSource,
    const char* inFile,
    long inLine)
    : Exception("InvalidParametersException", inDescription, inSource, inFile, inLine) {}
};

class InternalErrorException : public Exception
{
public:
  InternalErrorException(const String& inDescription,
    const String& inSource,
    const char* inFile,
    long inLine)
    : Exception("InternalErrorException", inDescription, inSource, inFile, inLine) {}
};

class RenderingAPIException : public Exception
{
public:
  RenderingAPIException(const String& inDescription,
    const String& inSource,
    const char* inFile,
    long inLine)
    : Exception("RenderingAPIException", inDescription, inSource, inFile, inLine) {}
};

class UnitTestException : public Exception
{
public:
  UnitTestException(const String& inDescription,
    const String& inSource,
    const char* inFile,
    long inLine)
    : Exception("UnitTestException", inDescription, inSource, inFile, inLine) {}
};

 //TODO: Add Crash handler functionality and the platform terminate data
#ifndef CY_EXCEPT
# define CY_EXCEPT(type, desc)                                                \
{                                                                             \
  static_assert((is_base_of<StdException, type>::value),                      \
                "Invalid exception type (" #type ") for CY_EXCEPT macro."     \
                "It needs to derive from geEngineSDK::Exception.");           \
  CrashHandler::instance().createReport(#type,                                \
                               desc,                                          \
                               __PRETTY_FUNCTION__,                           \
                               __FILE__,                                      \
                               __LINE__);                                     \
  Utils::terminate(true);                                                     \
}
#endif

#if CY_COMPILER == CY_COMPILER_MSVC
# pragma warning( pop )
#endif
}
