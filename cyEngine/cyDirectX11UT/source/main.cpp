#include <cyUnitTesting.h>

#include <cyGraphicsDX11API.h>
#include <cyDLLLoader.h>
#include <cyGraphicsAPI.h>
#include <cyWindow.h> 


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

  GraphicsDX11API::startUp<GraphicsDX11API>();
  GraphicsDX11API::instance().initialize(nullptr);



  doctest::Context context;

  context.applyCommandLine(argc, argv);

  int32 res = context.run();

  if (context.shouldExit()) {
    return res;
  }

  context.clearFilters();

  return res + EXIT_SUCCESS;
}


