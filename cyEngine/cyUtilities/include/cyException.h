/**
 * @file cyException.h
 * @author Cyllene Engine Team
 * @date 2026-02-26
 * @brief Contains declarations and definitions for Exception.
 */

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>
#include "cyAssert.h"

#include "cyUtilitiesPrerequisites.h"

#include "cyCrashHandler.h"
#include "cyUtilities.h"

namespace CYLLENE_SDK {

// Using namespaces
using std::is_base_of;

// Exception
using StdException = std::exception;

using RunTimeError = std::runtime_error;

#if CY_COMPILER == CY_COMPILER_MSVC
# pragma warning( push )
# pragma warning( disable : 4275 )
#endif

/**
 * @class Exception
 * @brief Base exception type for Cyllene utility systems.
 */
class Exception : public StdException
{
 public:
  /**
   * @brief Constructs an exception with type, description, and source.
   * @param type Exception type name.
   * @param description Error description.
   * @param source Source function or subsystem.
   */
  Exception(const char* type, 
            Stringview description, 
            Stringview source)
    : m_typeName(type),
      m_description(description),
      m_source(source) {}

  /**
   * @brief Constructs an exception with full source-location information.
   * @param type Exception type name.
   * @param description Error description.
   * @param source Source function or subsystem.
   * @param file Source file path.
   * @param line Source line number.
   */
  Exception(const char* type,
            Stringview description,
            Stringview source,
            const char* file,
            long line)
    : m_line(line),
      m_typeName(type),
      m_description(description),
      m_source(source),
      m_file(file),
      m_fullDesc("") {}

  /**
   * @brief Copy-constructs an exception.
   * @param rhs Exception to copy.
   */
  Exception(const Exception& rhs)
    : m_line(rhs.m_line),
      m_typeName(rhs.m_typeName),
      m_description(rhs.m_description),
      m_source(rhs.m_source),
      m_file(rhs.m_file),
      m_fullDesc(rhs.m_fullDesc) {}

  /**
   * @brief Default destructor.
   */
  ~Exception() _NOEXCEPT = default;

  /**
   * @brief Copy-assigns an exception.
   * @param rhs Exception to copy from.
   * @return Reference to this exception.
   */
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
   * @return the full description of the exception
   * @note  The description contains the error number, the description
   *        supplied by the thrower, what routine threw the exception, and
   *        will also supply extra platform-specific information where applicable.
   */
  virtual Stringview
  getFullDescription() const {
    if (m_fullDesc.empty()) {
      StringStream desc;
      desc << "CYLLENE ENGINE EXCEPTION(" << m_typeName << "): " 
           << m_description << " in " << m_source;

      if (m_line > 0) {
        desc << " at " << m_file << " (line " << m_line << ")";
      }

      m_fullDesc = desc.str();
    }

    return m_fullDesc;
  }

  /**
   * @brief Returns the source that produced the exception.
   * @return Source function or subsystem name.
   */
  virtual Stringview
    getSource() const {
    return m_source;
  }

  /**
   * @brief Returns source file path where exception was created.
   * @return Source file path.
   */
  virtual Stringview
    getFile() const {
    return m_file;
  }

  /**
   * @brief Returns source line where exception was created.
   * @return Source line number.
   */
  virtual long
    getLine() const {
    return m_line;
  }

  /**
   * @brief Returns short error description.
   * @return Error description string.
   */
  virtual Stringview
    getDescription(void) const {
    return m_description;
  }

  /**
   * @brief Standard exception message accessor.
   * @return C-string of full exception description.
   */
  const char*
    what() const _NOEXCEPT {
    return getFullDescription().data();
  }

protected:
  /**
   * @brief Runtime line number for the exception source.
   */
  long m_line = 0;
  /**
   * @brief Exception type name.
   */
  String m_typeName;
  /**
   * @brief Human-readable error description.
   */
  String m_description;
  /**
   * @brief Function or subsystem where exception originated.
   */
  String m_source;
  /**
   * @brief Source file path where exception originated.
   */
  String m_file;
  /**
   * @brief Cached full description string.
   */
  mutable String m_fullDesc;
};

/**
 * @class NotImplementedException
 * @brief Exception for unimplemented functionality.
 */
class NotImplementedException : public Exception
{
public:
  /**
   * @brief Constructs a not-implemented exception.
   * @param inDescription Error description.
   * @param inSource Source function or subsystem.
   * @param inFile Source file path.
   * @param inLine Source line number.
   */
  NotImplementedException(Stringview inDescription,
                          Stringview inSource,
                          const char* inFile,
                          long inLine)
    : Exception("NotImplementedException", inDescription, inSource, inFile, inLine) {}
};

/**
 * @class FileNotFoundException
 * @brief Exception for missing files.
 */
class FileNotFoundException : public Exception
{
public:
  /**
   * @brief Constructs a file-not-found exception.
   * @param inDescription Error description.
   * @param inSource Source function or subsystem.
   * @param inFile Source file path.
   * @param inLine Source line number.
   */
  FileNotFoundException(Stringview inDescription,
                        Stringview inSource,
                        const char* inFile,
                        long inLine)
    : Exception("FileNotFoundException", inDescription, inSource, inFile, inLine) {}
};

/**
 * @class IOException
 * @brief Exception for I/O failures.
 */
class IOException : public Exception
{
public:
  /**
   * @brief Constructs an I/O exception.
   * @param inDescription Error description.
   * @param inSource Source function or subsystem.
   * @param inFile Source file path.
   * @param inLine Source line number.
   */
  IOException(Stringview inDescription,
              Stringview inSource,
              const char* inFile,
              long inLine)
    : Exception("IOException", inDescription, inSource, inFile, inLine) {}
};

/**
 * @class InvalidStateException
 * @brief Exception for invalid runtime state.
 */
class InvalidStateException : public Exception
{
public:
  /**
   * @brief Constructs an invalid-state exception.
   * @param inDescription Error description.
   * @param inSource Source function or subsystem.
   * @param inFile Source file path.
   * @param inLine Source line number.
   */
  InvalidStateException(Stringview inDescription,
                        Stringview inSource,
                        const char* inFile,
                        long inLine)
    : Exception("InvalidStateException", inDescription, inSource, inFile, inLine) {}
};

/**
 * @class InvalidParametersException
 * @brief Exception for invalid input parameters.
 */
class InvalidParametersException : public Exception
{
public:
  /**
   * @brief Constructs an invalid-parameters exception.
   * @param inDescription Error description.
   * @param inSource Source function or subsystem.
   * @param inFile Source file path.
   * @param inLine Source line number.
   */
  InvalidParametersException(Stringview inDescription,
                             Stringview inSource,
                             const char* inFile,
                             long inLine)
    : Exception("InvalidParametersException", inDescription, inSource, inFile, inLine) {}
};

/**
 * @class InternalErrorException
 * @brief Exception for internal engine errors.
 */
class InternalErrorException : public Exception
{
public:
  /**
   * @brief Constructs an internal-error exception.
   * @param inDescription Error description.
   * @param inSource Source function or subsystem.
   * @param inFile Source file path.
   * @param inLine Source line number.
   */
  InternalErrorException(Stringview inDescription,
                         Stringview inSource,
                         const char* inFile,
                         long inLine)
    : Exception("InternalErrorException", inDescription, inSource, inFile, inLine) {}
};

/**
 * @class RenderingAPIException
 * @brief Exception for rendering API failures.
 */
class RenderingAPIException : public Exception
{
public:
  /**
   * @brief Constructs a rendering-API exception.
   * @param inDescription Error description.
   * @param inSource Source function or subsystem.
   * @param inFile Source file path.
   * @param inLine Source line number.
   */
  RenderingAPIException(Stringview inDescription,
                        Stringview inSource,
                        const char* inFile,
                        long inLine)
    : Exception("RenderingAPIException", inDescription, inSource, inFile, inLine) {}
};

/**
 * @class UnitTestException
 * @brief Exception used by unit testing utilities.
 */
class UnitTestException : public Exception
{
public:
  /**
   * @brief Constructs a unit-test exception.
   * @param inDescription Error description.
   * @param inSource Source function or subsystem.
   * @param inFile Source file path.
   * @param inLine Source line number.
   */
  UnitTestException(Stringview inDescription,
                    Stringview inSource,
                    const char* inFile,
                    long inLine)
    : Exception("UnitTestException", inDescription, inSource, inFile, inLine) {}
};

 //TODO: Add Crash handler functionality and the platform terminate data
#ifndef CY_EXCEPT
# define CY_EXCEPT(type, desc)                                                \
{                                                                             \
  static_assert((is_base_of<Exception, type>::value),                         \
                "Invalid exception type (" #type ") for CY_EXCEPT macro."     \
                "It needs to derive from CYLLEME_SDK::Exception.");           \
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

