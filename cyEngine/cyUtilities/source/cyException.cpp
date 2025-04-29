#include "cyException.h"

namespace CYLLENE_SDK {
/*
  using std::rethrow_exception;
  using std::throw_with_nested;
  using std::current_exception;
  using std::rethrow_if_nested;

// Rethrow (creates a std::nested_exception) an exception, using the Exception class
// which contains file and line info. The original exception is preserved...
void rethrow(const String& message, const char* file, unsigned int32 line)
try
{
  rethrow_exception(current_exception());
}
catch (...)
{
  throw_with_nested(Exception(message, file, line));
}

// Backtrace an exception by recursively unwrapping the nested exceptions
void Backtrace(const StdException& ex)
try
{
  std::cerr << ex.what() << std::endl;
  rethrow_if_nested(ex);
}
catch (const StdException& nested_ex)
{
  Backtrace(nested_ex);
}

// General Exception handler
void Handle_Exception(const StdException& ex, const String& function)
  try
{
  if (function != "") {
    std::cerr << "Library API: Exception caught in function \'" << function << "\'" << std::endl;
  }
  std::cerr << "Backtrace:" << std::endl;
  Backtrace(ex);
}
catch (...)
{
  std::cerr << "Something went super-wrong! TERMINATING!" << std::endl;
  std::exit(EXIT_FAILURE);
}
*/
}