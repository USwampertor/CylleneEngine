#pragma once
#include "cyMathPrerequisites.h"

#include "cyVector4f.h"
#include "cySphere.h"
#include "cyLine.h"
#include "cyPlane.h"
#include "cyAABB.h"
#include "cyOBB.h"



namespace CYLLENE_SDK {

/**
* @brief Check if a sphere intersects with a plane
* @param sphere The sphere to check
* @param plane The plane to check against
* @return True if the sphere intersects with the plane, false otherwise
*/
bool
intersects(const Sphere& sphere, const Plane& plane);
/**
* @brief Check if a line intersects with a plane
* @param line The line to check
* @param plane The plane to check against
* @return True if the line intersects with the plane, false otherwise
*/
bool
intersects(const Line& line, const Plane& plane);
/**
* @brief Check if an AABB intersects with a plane
* @param aabb The AABB to check
* @param plane The plane to check against
* @return True if the AABB intersects with the plane, false otherwise
*/
bool
intersects(const AABB& aabb, const Plane& plane);
/**
* @brief Check if an OBB intersects with a plane
* @param obb The OBB to check
* @param plane The plane to check against
* @return True if the OBB intersects with the plane, false otherwise
*/
bool
intersects(const OBB& obb, const Plane& plane);
/**
* @brief Check if a sphere intersects with an AABB
* @param sphere The sphere to check
* @param aabb The AABB to check against
* @return True if the sphere intersects with the AABB, false otherwise
*/
bool
intersects(const Sphere& sphere, const AABB& aabb);
/**
* @brief Check if a sphere intersects with an OBB
* @param sphere The sphere to check
* @param obb The OBB to check against
* @return True if the sphere intersects with the OBB, false otherwise
*/
  
bool
intersects(const Sphere& sphere, const OBB& obb);
/**
* @brief Check if an AABB intersects with an OBB
* @param aabb The AABB to check
* @param obb The OBB to check against
* @return True if the AABB intersects with the OBB, false otherwise
*/
bool
intersects(const AABB& aabb, const OBB& obb);
/**
* @brief Check if a line intersects with an AABB
* @param line The line to check
* @param aabb The AABB to check against
* @return True if the line intersects with the AABB, false otherwise
*/
bool
intersects(const Line& line, const AABB& aabb);
/**
* @brief Check if a line intersects with an OBB
* @param line The line to check
* @param obb The OBB to check against
* @return True if the line intersects with the OBB, false otherwise
*/
bool
intersects(const Line& line, const OBB& obb);
/**
* @brief Check if an AABB intersects with a sphere
* @param aabb The AABB to check
* @param sphere The sphere to check against
* @return True if the AABB intersects with the sphere, false otherwise
*/
bool
intersects(const AABB& aabb, const Sphere& sphere);
/**
* @brief Check if an OBB intersects with a sphere
* @param obb The OBB to check
* @param sphere The sphere to check against
* @return True if the OBB intersects with the sphere, false otherwise
*/



}


