/**
 * 
 */

#include <conio.h>

#include <cyJSON.h>

#include <cyCrashHandler.h>

using namespace CYLLENE_SDK;

int32
main() {
  CrashHandler::startUp();
  try
  {
    JSON json;
    json.Parse("{\"hello\":\"world\"}");
    std::cout << json["hello"].GetString() << std::endl;
    std::cout << json.stringify() << std::endl;
    std::cout << json.prettyString() << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
    CrashHandler::instance().createReport(e);
  }
  std::cout << "End of test..." << std::endl;
  return 0;
}