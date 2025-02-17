/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/
/**
 * @file   	main.cpp
 * @author 	Marco "Swampy" Millan
 * @date 	2024/11/21
 * @brief 	
 *
 * 
 */
/*0***0***0***0***0***0***0***0***0***0***0***0***0***0***0***0*/

#include <cyBeing.h>
#include <cyCoreUTPrerequisites.h>
#include <cyCrashHandler.h>
#include <cyFileSystem.h>
#include <cyGameMode.h>
#include <cyImage.h>
#include <cyLogger.h>
#include <cyMath.h>
#include <cyMesh.h>
#include <cyModel.h>
#include <cyResourceManager.h>
#include <cyTexture.h>
#include <cyTime.h>
#include <cyTriangle.h>
#include <cyTransform.h>
#include <cyUnitTesting.h>
#include <cyVector2f.h>
#include <cyVertex.h>
#include <cyWindow.h>

#include <iostream>


using namespace CYLLENE_SDK;


using ManualShader = std::function<Color(float, float)>;


Vertex rotateVertex(const Vertex& v,
  float angleX,
  float angleY,
  float angleZ,
  float cx,
  float cy,
  float cz) {
  Vertex result = v;

  result.m_position.x -= cx;
  result.m_position.y -= cy;
  result.m_position.z -= cz;

  float cosX = cos(angleX);
  float sinX = sin(angleX);
  float y = result.m_position.y * cosX - result.m_position.z * sinX;
  float z = result.m_position.y * sinX + result.m_position.z * cosX;
  result.m_position.y = y;
  result.m_position.z = z;

  float cosY = cos(angleY);
  float sinY = sin(angleY);
  float x = result.m_position.x * cosY + result.m_position.z * sinY;
  z = -result.m_position.x * sinY + result.m_position.z * cosY;
  result.m_position.x = x;
  result.m_position.z = z;

  float cosZ = cos(angleZ);
  float sinZ = sin(angleZ);
  x = result.m_position.x * cosZ - result.m_position.y * sinZ;
  y = result.m_position.x * sinZ + result.m_position.y * cosZ;
  result.m_position.x = x;
  result.m_position.y = y;

  // Translate back
  result.m_position.x += cx;
  result.m_position.y += cy;
  result.m_position.z += cz;

  return result;
}


void drawTopTriangle(Triangle t, SharedPointer<ImageResource>& img, const ManualShader& pShader) {
  Vertex p0 = t._m.v0;
  Vertex p1 = t._m.v1;
  Vertex p2 = t._m.v2;

  if (p1.m_position.x < p0.m_position.x) { std::swap(p0, p1); }

  int32_t height = p2.m_position.y - p0.m_position.y;
  if (height <= 0) {
    return;
  }

  float dxLeft = static_cast<float>(p2.m_position.x - p0.m_position.x) / height;
  float dxRight = static_cast<float>(p2.m_position.x - p1.m_position.x) / height;

  float duLeft = (p2.m_uv.x - p0.m_uv.x) / height;
  float dvLeft = (p2.m_uv.y - p0.m_uv.y) / height;
  float duRight = (p2.m_uv.x - p1.m_uv.x) / height;
  float dvRight = (p2.m_uv.y - p1.m_uv.y) / height;

  float xs = p0.m_position.x;
  float xe = p1.m_position.x;
  float us = p0.m_uv.x;
  float vs = p0.m_uv.y;
  float ue = p1.m_uv.x;
  float ve = p1.m_uv.y;

  for (int y = p0.m_position.y; y <= p2.m_position.y; ++y) {
    int32_t left = static_cast<int32_t>(xs);
    int32_t right = static_cast<int32_t>(xe);
    if (left > right) {
      std::swap(left, right);
    }

    float u = us;
    float v = vs;

    float du = (ue - us) / (right - left + 1);
    float dv = (ve - vs) / (right - left + 1);

    for (int32_t x = Math::max(0, left); 
         x <= Math::min(static_cast<int32_t>(img->m_metadata.m_width - 1), right); 
         ++x) {
      // TODO: CHECK CLIPPING

      if (y > 0 && y < img->m_metadata.m_height) {
        Color pColor = pShader(u, v);

        img->setPixel(x, y, pColor);
      }
      u += du;
      v += dv;
    }

    xs += dxLeft;
    xe += dxRight;
    us += duLeft;
    vs += dvLeft;
    ue += duRight;
    ve += dvRight;

  }
}

void drawBottomTriangle(Triangle t,  SharedPointer<ImageResource>& img, const ManualShader& pShader) {
  Vertex p0 = t._m.v0;
  Vertex p1 = t._m.v1;
  Vertex p2 = t._m.v2;

  if (p2.m_position.x < p1.m_position.x) { std::swap(p1, p2); }

  int32_t height = p2.m_position.y - p0.m_position.y;
  if (height <= 0) {
    return;
  }

  float dxLeft = static_cast<float>(p1.m_position.x - p0.m_position.x) / height;
  float dxRight = static_cast<float>(p2.m_position.x - p0.m_position.x) / height;

  float duLeft = (p1.m_uv.x - p0.m_uv.x) / height;
  float dvLeft = (p1.m_uv.y - p0.m_uv.y) / height;
  float duRight = (p2.m_uv.x - p0.m_uv.x) / height;
  float dvRight = (p2.m_uv.y - p0.m_uv.y) / height;

  float xs = p0.m_position.x;
  float xe = p0.m_position.x;
  float us = p0.m_uv.x;
  float vs = p0.m_uv.y;
  float ue = p0.m_uv.x;
  float ve = p0.m_uv.y;

  for (int y = p0.m_position.y; y <= p2.m_position.y; ++y) {
    int32_t left = static_cast<int32_t>(xs);
    int32_t right = static_cast<int32_t>(xe);
    if (left > right) {
      std::swap(left, right);
    }

    float u = us;
    float v = vs;

    float du = (ue - us) / (right - left + 1);
    float dv = (ve - vs) / (right - left + 1);

    for (int32_t x = std::max(0, left); x <= 
         Math::min(static_cast<int32_t>(img->m_metadata.m_width - 1), right); 
         ++x) {

      // TODO: CHECK CLIPPING
      if (y > 0 && y < img->m_metadata.m_height) {
        Color pColor = pShader(u, v);

        img->setPixel(x, y, pColor);
      }
      u += du;
      v += dv;
    }

    xs += dxLeft;
    xe += dxRight;
    us += duLeft;
    vs += dvLeft;
    ue += duRight;
    ve += dvRight;

  }

}

void
fillTriangle(Triangle& t, 
  SharedPointer<ImageResource>& img, 
  const ManualShader& pShader) {
  // if all three triangle edges have different y, we split it into 2
  Vertex p0 = t._m.v0;
  Vertex p1 = t._m.v1;
  Vertex p2 = t._m.v2;

  if (p0.m_position.y > p1.m_position.y) { std::swap(p0, p1); }
  if (p0.m_position.y > p2.m_position.y) { std::swap(p0, p2); }
  if (p1.m_position.y > p2.m_position.y) { std::swap(p1, p2); }

  // Barycentric coordinate
  if (p1.m_position.y == p2.m_position.y) {
    drawBottomTriangle(Triangle(p0, p1, p2), img, pShader);
  }
  else if (p0.m_position.y == p1.m_position.y) {
    drawTopTriangle(Triangle(p0, p1, p2), img, pShader);
  }
  else {
    Vector<Triangle> vec = t.getTopAndBottom();
    drawBottomTriangle(vec[0], img, pShader);
    drawTopTriangle(vec[1], img, pShader);
  }
}



int32
main(int32 argc, char* argv[]) {


  std::cout << argv[0] << std::endl;

  CrashHandler::startUp();
  SmartPointers::startUp();
  Logger::startUp();
  Time::startUp();
  ResourceManager::startUp();
  WindowManager::startUp();


  ImageResource sourceFile;
  ImageResource destinyFile;
  
  String destinyString;

  Path workingPath   = FileSystem::getWorkingDirectory();
  Path resourcesPath = workingPath.fullPath() + "/../resources";

  SharedPointer<ImageResource> imgScreen = 
    ResourceManager::instance().create<ImageResource>("imgScreen");
  
  imgScreen->create(1920, 1080, 32);
  imgScreen->clear();

  SharedPointer<ImageResource> imgTerrain = 
    ResourceManager::instance().loadFromPath<ImageResource>(Utils::format("%s/terrain2.bmp", 
                                                                          resourcesPath.fullPath().c_str()));


  SharedPointer<ImageResource> imgHorse = 
    ResourceManager::instance().loadFromPath<ImageResource>(Utils::format("%s/galina.bmp", 
                                                                          resourcesPath.fullPath().c_str()));

  imgScreen->bitBlt(imgTerrain,
                    Rect(0, 0, imgTerrain->m_metadata.m_width, imgTerrain->m_metadata.m_height),
                    Rect(0, 0, imgTerrain->m_metadata.m_width, imgTerrain->m_metadata.m_width));

  SharedPointer<TextureResource> horseTexture =
    ResourceManager::instance().create<TextureResource>("horseTexture");
  horseTexture->setImage(imgHorse);


  // Rotation center
  float cx = 0, cy = 0, cz = 0;

  // Rotation angles
  float angleX = 0, angleY = 0, angleZ = 0;
  
  // std::cout << "Starting Core Unit Test" << std::endl;
  // 
  // doctest::Context context;
  // 
  // context.applyCommandLine(argc, argv);
  // 
  // int32 res = context.run();
  // 
  // if (context.shouldExit()) {
  //   return res;
  // }
  // 
  // context.clearFilters();
  // 
  // return res + EXIT_SUCCESS;

  return 0;
}

TEST_CASE("[module] testing module startup") {
  CrashHandler::startUp();
  CHECK(CrashHandler::isStarted());
  SmartPointers::startUp();
  CHECK(CrashHandler::isStarted());
  Logger::startUp();
  CHECK(Logger::isStarted());
  Time::startUp();
  CHECK(Time::isStarted());
  ResourceManager::startUp();
  CHECK(ResourceManager::isStarted());
}

#define CLASSNAME(x) #x

TEST_CASE("[being] Creation of beings") {
  CHECK(Being::getClassName() == CLASSNAME(Being));
  CHECK(GameMode::getClassName() == CLASSNAME(GameMode));

  SharedPointer<Being> b1 = ClassRegister::createBeing("Being");
  SharedPointer<Being> b2 = ClassRegister::createBeing<Being>();
  b1->setName("b1");
  b1->createComponent<TransformComponent>();
}

TEST_CASE("[resource] Creation of textures") {

  Path workingPath = FileSystem::getWorkingDirectory();
  File testImage = FileSystem::open(workingPath.fullPath() + "/../resources/gizmo.png");
  if (testImage.isFile() && testImage.exists()) {
    std::cout << testImage.path() << std::endl;
    SharedPointer<ImageResource> r = ResourceManager::instance().loadFromPath<ImageResource>(testImage.path());
    CHECK(r->m_metadata.m_width == 32);
    CHECK(r->m_metadata.m_height == 32);
    CHECK(r->m_metadata.m_format == (+IMGEXT::E::PNG)._to_integral());
  }

}

TEST_CASE("[resource] Creation of models") {
  Path workingPath = FileSystem::getWorkingDirectory();
  File testModel = FileSystem::open(workingPath.fullPath() + "/../resources/cube.fbx");
  if (testModel.isFile() && testModel.exists()) {
    std::cout << testModel.path() << std::endl;
    SharedPointer<ModelResource> r = ResourceManager::instance().loadFromPath<ModelResource>(testModel.path());
    CHECK(r->m_meshes.size() == 1);
    CHECK((r->m_meshes[0])->m_vertexBuffer.size() == 24);
    CHECK((r->m_meshes[0])->m_indexBuffer.size() == 36);
  }
}

