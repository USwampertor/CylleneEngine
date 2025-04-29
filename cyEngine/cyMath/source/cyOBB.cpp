#include "cyOBB.h"
#include "cyMath.h"
#include "cyMatrix4.h"

#include <cyUtilities.h>

namespace CYLLENE_SDK {

String
OBB::toString() {
  return Utils::format("A: %s, B: %s, Height: %2.f");
}

bool
OBB::intersects(const Primitive& other) {
  const PRIMITIVE_TYPE::E type = other.getType();
  if (type == +PRIMITIVE_TYPE::E::OBB) {
    const OBB& otherOBB = static_cast<const OBB&>(other);

    // Axes to test: 3 face normals of this OBB, 3 face normals of the other OBB,
  // and 9 cross products of the edge directions of both OBBs.
    Vector3f axes[15];
    int axisCount = 0;

    // Add face normals of this OBB
    axes[axisCount++] = m_orientation.toMat4().getRightVector();    // Local X-axis
    axes[axisCount++] = m_orientation.toMat4().getUpVector();       // Local Y-axis
    axes[axisCount++] = m_orientation.toMat4().getForwardVector();  // Local Z-axis

    // Add face normals of the other OBB
    axes[axisCount++] = otherOBB.m_orientation.toMat4().getRightVector();    
    axes[axisCount++] = otherOBB.m_orientation.toMat4().getUpVector();       
    axes[axisCount++] = otherOBB.m_orientation.toMat4().getForwardVector();  

    // Add cross products of edge directions
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        axes[axisCount++] = Vector3f::cross(m_orientation.toMat4().columns[i], 
                                            otherOBB.m_orientation.toMat4().columns[j]);
      }
    }

    // Test each axis
    for (int i = 0; i < axisCount; ++i) {
      const Vector3f& axis = axes[i];

      // Skip degenerate axes (zero-length)
      if (axis.sqrMagnitude() < Math::EPSILONF) {
        continue;
      }

      // Normalize the axis
      Vector3f normalizedAxis = axis.normalized();

      // Project both OBBs onto the axis
      float projection1 = projectOntoAxis(normalizedAxis);
      float projection2 = otherOBB.projectOntoAxis(normalizedAxis);

      // Compute the distance between the centers along the axis
      float centerDistance = std::abs(Vector3f::dot(normalizedAxis, otherOBB.m_center - m_center));

      // Check for overlap
      if (centerDistance > projection1 + projection2) {
        return false; // Separating axis found, no intersection
      }
    }

    // No separating axis found, the OBBs intersect
    return true;

  }

  return false;
}

float OBB::projectOntoAxis(const Vector3f& axis) const {
  // Project the half-extents of the OBB onto the axis
  return std::abs(m_hExtents.x * Vector3f::dot(axis, m_orientation.toMat4().getRightVector())) +
    std::abs(m_hExtents.y * Vector3f::dot(axis, m_orientation.toMat4().getUpVector())) +
    std::abs(m_hExtents.z * Vector3f::dot(axis, m_orientation.toMat4().getForwardVector()));
}

}