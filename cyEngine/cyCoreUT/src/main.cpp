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

#include <iostream>
#include <SDL3/SDL.h>

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


Vector<Triangle>
getTopAndBottom(Triangle& t) {
  Vertex p0 = t._m.v0;
  Vertex p1 = t._m.v1;
  Vertex p2 = t._m.v2;
  float tVal = static_cast<float>(p1.m_position.y - p0.m_position.y) /
    static_cast<float>(p2.m_position.y - p0.m_position.y);
  int newX = p0.m_position.x +
    static_cast<int32>(0.5f +
      static_cast<float>(p1.m_position.y - p0.m_position.y) *
      static_cast<float>(p2.m_position.x - p0.m_position.x) /
      static_cast<float>(p2.m_position.y - p0.m_position.y));

  float newU = p0.m_uv.x +
    ((p1.m_position.y - p0.m_position.y) * ((p2.m_uv.x - p0.m_uv.x) /
      (p2.m_position.y - p0.m_position.y)));
  float newV = p0.m_uv.y +
    ((p1.m_position.y - p0.m_position.y) * ((p2.m_uv.y - p0.m_uv.y) /
      (p2.m_position.y - p0.m_position.y)));

  Vertex newVtx(Vector3f(static_cast<float>(newX), p1.m_position.y, 0),
    Vector3f::ZERO,
    Vector3f::ZERO,
    Vector3f::ZERO,
    Vector2f(newU, newV));

  Color nC = t._m.v0.m_color +
    ((t._m.v2.m_color - t._m.v0.m_color) * (p1.m_position.y - p0.m_position.y) /
      (p2.m_position.y - p0.m_position.y));

  Triangle t1(p0, newVtx, p1);
  Triangle t2(p1, newVtx, p2);

  t1._m.v0.m_color = t._m.v0.m_color;
  t1._m.v1.m_color = t._m.v1.m_color;
  t1._m.v2.m_color = t._m.v1.m_color;

  t2._m.v0.m_color = t._m.v1.m_color;
  t2._m.v1.m_color = t._m.v2.m_color;
  t2._m.v2.m_color = t._m.v2.m_color;

  Vector<Triangle> vec;

  // Bottom
  vec.push_back(t1);
  // Top
  vec.push_back(t2);

  return vec;
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
    Vector<Triangle> vec  = getTopAndBottom(t);
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

  std::vector<Triangle> cubeFaces = {
    // Back
    {Vertex(Vector3f(800, 500, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 0)),
     Vertex(Vector3f(500, 500, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 0)),
     Vertex(Vector3f(800, 800, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 1))},
    {Vertex(Vector3f(500, 500, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 0)),
     Vertex(Vector3f(500, 800, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 1)),
     Vertex(Vector3f(800, 800, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 1))},
    {Vertex(Vector3f(500, 500,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 0)),
     Vertex(Vector3f(800, 500,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 0)),
     Vertex(Vector3f(500, 800,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 1))},
    {Vertex(Vector3f(800, 500,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 0)),
     Vertex(Vector3f(800, 800,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 1)),
     Vertex(Vector3f(500, 800,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 1))},
    {Vertex(Vector3f(500, 500, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 0)),
     Vertex(Vector3f(800, 500, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 0)),
     Vertex(Vector3f(500, 500,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 1))},
    {Vertex(Vector3f(800, 500, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 0)),
     Vertex(Vector3f(800, 500,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 1)),
     Vertex(Vector3f(500, 500,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 1))},
    {Vertex(Vector3f(500, 800,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 0)),
     Vertex(Vector3f(800, 800,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 0)),
     Vertex(Vector3f(500, 800, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 1))},
    {Vertex(Vector3f(800, 800,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 0)),
     Vertex(Vector3f(800, 800, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 1)),
     Vertex(Vector3f(500, 800, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 1))},
    {Vertex(Vector3f(500, 500, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 0)),
     Vertex(Vector3f(500, 500,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 0)),
     Vertex(Vector3f(500, 800, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 1))},
    {Vertex(Vector3f(500, 500,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 0)),
     Vertex(Vector3f(500, 800,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 1)),
     Vertex(Vector3f(500, 800, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 1))},
    {Vertex(Vector3f(800, 500, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 0)),
     Vertex(Vector3f(800, 800, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 1)),
     Vertex(Vector3f(800, 800,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 1))},
    {Vertex(Vector3f(800, 500,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 0)),
     Vertex(Vector3f(800, 500, -300), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(1, 0)),
     Vertex(Vector3f(800, 800,    0), Vector3f::ZERO, Vector3f::ZERO, Vector3f::ZERO, Vector2f(0, 1))}

  };

  ManualShader uvPShader = [&horseTexture](float u, float v) {
    Color c(u, v, 0.0f, 1.0f); 
    return c;
  };

  ManualShader texPShader = [&horseTexture](float u, float v) {
    Color c/*(u, v, 0.0f, 1.0f);*/ = horseTexture->sample(u, 
                                                          v, 
                                                          TEXTUREMODE::E::eCLAMP, 
                                                          SAMPLERFILTER::E::eLINEAR);
    return c;
  };

  ManualShader greyPShader = [&horseTexture](float u, float v) {
    Color c = horseTexture->sample(u, 
                                   v, 
                                   TEXTUREMODE::E::eCLAMP, 
                                   SAMPLERFILTER::E::eLINEAR);
    Vector3f grey(0.299f, 0.587f, 0.114f);
    Vector3f colorV(c.r, c.g, c.b);
    float  intensity = grey | colorV;

    // unsigned char rV = static_cast<unsigned char>(intensity * 255);

    c.setFloat(intensity, intensity, intensity);
    return c;
  };

  ManualShader kernelPShader = [&horseTexture](float u, float v) {

    float kernelBlur[] = {
      0.0625,0.125,0.0625,
      0.125, 0.25, 0.125,
      0.0625,0.125,0.0625
    };

    float kernelBSobel[] = {
      -1,-2,-1,
      0, 0, 0,
      1,2,1
    };


    float kernelEmboss[] = {
      -2,-1, 0,
      -1, 1, 1,
      0,1,2
    };

    float kernelIdentity[] = {
      0,0, 0,
      0, 1, 0,
      0,0,0
    };

    float kernelLSoble[] = {
      1,0, -1,
      2,0, -2,
      1,0, -1
    };

    float kernelOutline[] = {
      -1, -1, -1,
      -1, 8, -1,
      -1, -1, -1
    };

    float kernelRSoble[] = {
      -1,0,1  ,
      -2,0,2  ,
      -1,0,1
    };

    float kernelSharpen[] = {
      0,-1,0  ,
      -1,5,-1  ,
      0,-1,0
    };

    
    float kernelTSobel[] = {
      1,2,1,
      0, 0, 0,
      -1,-2,-1
    };

    float* kernel = kernelTSobel;

    float du = 1.0f / horseTexture->m_img->m_metadata.m_width;
    float dv = 1.0f / horseTexture->m_img->m_metadata.m_height;

    Color c00 = horseTexture->sample(u - du, v - dv, TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c01 = horseTexture->sample(u - du, v     , TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c02 = horseTexture->sample(u - du, v + dv, TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c10 = horseTexture->sample(u     , v - dv, TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c11 = horseTexture->sample(u     , v     , TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c12 = horseTexture->sample(u     , v + dv, TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c20 = horseTexture->sample(u + du, v - dv, TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c21 = horseTexture->sample(u + du, v     , TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c22 = horseTexture->sample(u + du, v + dv, TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);

    // Color avg = (c00 + c01 + c02 + c10 + c11 + c12 + c20 + c21 + c22) / 9.0f;

    Color finalVal = c00 * kernel[0] + c10 * kernel[1] + c20 * kernel[2] +
                     c01 * kernel[3] + c11 * kernel[4] + c21 * kernel[5] +
                     c02 * kernel[6] + c12 * kernel[7] + c22 * kernel[8];
    finalVal.saturate();
    float grey = finalVal.toGrey();

    return Color(grey, grey, grey);
  };


  ManualShader normalPShader = [&horseTexture](float u, float v) {

    float kernelLSoble[] = {
      1,0, -1,
      2,0, -2,
      1,0, -1
    };

    float kernelTSobel[] = {
      1,2,1,
      0, 0, 0,
      -1,-2,-1
    };


    float du = 1.0f / horseTexture->m_img->m_metadata.m_width;
    float dv = 1.0f / horseTexture->m_img->m_metadata.m_height;

    Color c00 = horseTexture->sample(u - du, v - dv, TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c01 = horseTexture->sample(u - du, v     , TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c02 = horseTexture->sample(u - du, v + dv, TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c10 = horseTexture->sample(u     , v - dv, TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c11 = horseTexture->sample(u     , v     , TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c12 = horseTexture->sample(u     , v + dv, TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c20 = horseTexture->sample(u + du, v - dv, TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c21 = horseTexture->sample(u + du, v     , TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);
    Color c22 = horseTexture->sample(u + du, v + dv, TEXTUREMODE::E::eCLAMP, SAMPLERFILTER::E::ePOINT);

    // Color avg = (c00 + c01 + c02 + c10 + c11 + c12 + c20 + c21 + c22) / 9.0f;

    float* kernel = kernelLSoble;
    Color finalValX = c00 * kernel[0] + c10 * kernel[1] + c20 * kernel[2] +
                      c01 * kernel[3] + c11 * kernel[4] + c21 * kernel[5] +
                      c02 * kernel[6] + c12 * kernel[7] + c22 * kernel[8];

    kernel = kernelTSobel;
    Color finalValY = c00 * kernel[0] + c10 * kernel[1] + c20 * kernel[2] +
                      c01 * kernel[3] + c11 * kernel[4] + c21 * kernel[5] +
                      c02 * kernel[6] + c12 * kernel[7] + c22 * kernel[8];

    finalValX.saturate();
    finalValY.saturate();

    float sx = finalValX.toGrey();
    float sy = finalValY.toGrey();

    Color normal(sx, sy, 1, 1);

    return normal;
  };

  // Initialize SDL3
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return -1;
  }

  // Create an SDL3 window
  SDL_Window* window = SDL_CreateWindow("SDL3 Window", 1920, 1080, SDL_WINDOW_OPENGL);
  if (!window) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return -1;
  }

  // Create an SDL3 renderer
  SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer) {
    std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

  SDL_Texture* sdlTexture = nullptr;
  // Main loop flag
  int quit = 0;

  // Main loop
  bool running = true;
  while (running) {
    imgScreen->clear();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    // Rotate the cube
    angleX += 0.1f;
    angleY += 0.1f;
    angleZ += 0.1f;

    for (const auto& tri : cubeFaces) {
      Triangle rotatedTri(
        rotateVertex(tri._m.v0, angleX, angleY, angleZ, 650, 650, -150),
        rotateVertex(tri._m.v1, angleX, angleY, angleZ, 650, 650, -150),
        rotateVertex(tri._m.v2, angleX, angleY, angleZ, 650, 650, -150)
      );

      Vector3f v0v1 = rotatedTri._m.v1.m_position - rotatedTri._m.v0.m_position;
      Vector3f v0v2 = rotatedTri._m.v2.m_position - rotatedTri._m.v0.m_position;

      Vector3f normal = Vector3f::cross(v0v1, v0v2);
      Vector3f view(0, 0, -1);
      normal = normal.normalized();
      float intensity = normal | view;
      //triangle is facing forwards
      if (intensity > 0) {
        // Draw the triangle
        fillTriangle(rotatedTri, imgScreen, texPShader);
      }

    }

    sdlTexture = SDL_CreateTexture(renderer,
                                   SDL_PIXELFORMAT_RGBA128_FLOAT,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   imgScreen->m_metadata.m_width,
                                   imgScreen->m_metadata.m_height);

    // Clear screen with a color
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_Surface* surface = nullptr;

    // Add something to create pixels randomly

    SDL_Surface* imgSurface = SDL_CreateSurfaceFrom(imgScreen->m_metadata.m_width,
      imgScreen->m_metadata.m_height,
      SDL_PIXELFORMAT_RGBA128_FLOAT,
      imgScreen->m_pixels.data(),
      imgScreen->m_metadata.m_width * 16); // pitch is width * bytes per pixel

    if (SDL_LockTextureToSurface(sdlTexture, nullptr, &surface)) {
      SDL_BlitSurface(imgSurface, nullptr, surface, nullptr);
      // SDL_Rect r;
      // SDL_FillSurfaceRect(surface,
      //                     nullptr,
      //                     SDL_MapRGB(SDL_GetPixelFormatDetails(surface->format), 
      //                                nullptr, 0, 0, 0));
      // 
      // r.w = sdlTexture->w;
      // r.h = sdlTexture->h;
      // r.x = 0;
      // r.y = static_cast<int>((static_cast<float>(sdlTexture->)));

      // Copy to GPU
      SDL_UnlockTexture(sdlTexture);
    }
    SDL_DestroySurface(imgSurface);

    SDL_FRect dstRect = { 0, 0, imgScreen->m_metadata.m_width, imgScreen->m_metadata.m_height };
    SDL_RenderTexture(renderer, sdlTexture, nullptr, &dstRect);

    // Present the frame
    SDL_RenderPresent(renderer);
  }

  // Cleanup
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

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

