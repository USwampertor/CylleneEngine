/**
 * 
 */

#include <conio.h>

#include <cyJSON.h>
#include <cyQuaternion.h>
#include <cyMatrix3x3.h>
#include <cyMatrix4x4.h>
#include <cyCrashHandler.h>
#include <cyMath.h>

using namespace CYLLENE_SDK;

int32
main() {
  CrashHandler::startUp();
  Quaternion q;

  try
  {
#if _DEBUG
    printf("DEBUG\n");
#else
    printf("RELEASE\n");
#endif

    float color[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
    Quaternion worldRotation;
    worldRotation.fromEuler(Euler(3.14159265f * 1.5f, 3.14159265f, 0.0f), 0);
    printf("Quaternion:\n%s\n", worldRotation.toString().c_str());

    Matrix3x3 world3x3 = worldRotation.getRotationMatrix();
    Matrix4x4 world(world3x3.m[0][0], world3x3.m[0][1], world3x3.m[0][2], 0.0f,
                    world3x3.m[1][0], world3x3.m[1][1], world3x3.m[1][2], 0.0f,
                    world3x3.m[2][0], world3x3.m[2][1], world3x3.m[2][2], 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f);
    printf("Matrix:\n%s\n", world3x3.toString().c_str());

  }
  catch (const std::exception& e)
  {
    std::cout << e.what();
    CrashHandler::instance().createReport(e);
  }
  std::cout << "End of test..." << std::endl;
  return 0;
}