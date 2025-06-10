#include "frustum.hpp"

// Plane implementation
float Plane::distanceToPoint(const cgp::vec3& point) const {
    return cgp::dot(normal, point) + distance;
}

// AABB implementation
AABB::AABB(const cgp::vec3& center, const cgp::vec3& size) {
    cgp::vec3 halfSize = size * 0.5f;
    min = center - halfSize;
    max = center + halfSize;
}

// Replace the extractFromMatrix method in frustum.cpp with this corrected version:
void Frustum::extractFromMatrix(const cgp::mat4& m) {
    // CGP uses column-major order, so we need to access as m(row, col)
    // Standard frustum extraction assumes row-major, so we need to transpose our access
    
    // Left plane: m[3] + m[0] (in row-major terms)
    planes[0].normal.x = m(3,0) + m(0,0);  // m[3][0] + m[0][0]
    planes[0].normal.y = m(3,1) + m(0,1);  // m[3][1] + m[0][1] 
    planes[0].normal.z = m(3,2) + m(0,2);  // m[3][2] + m[0][2]
    planes[0].distance = m(3,3) + m(0,3);  // m[3][3] + m[0][3]
    
    // Right plane: m[3] - m[0]
    planes[1].normal.x = m(3,0) - m(0,0);
    planes[1].normal.y = m(3,1) - m(0,1);
    planes[1].normal.z = m(3,2) - m(0,2);
    planes[1].distance = m(3,3) - m(0,3);
    
    // Bottom plane: m[3] + m[1]
    planes[2].normal.x = m(3,0) + m(1,0);
    planes[2].normal.y = m(3,1) + m(1,1);
    planes[2].normal.z = m(3,2) + m(1,2);
    planes[2].distance = m(3,3) + m(1,3);
    
    // Top plane: m[3] - m[1]
    planes[3].normal.x = m(3,0) - m(1,0);
    planes[3].normal.y = m(3,1) - m(1,1);
    planes[3].normal.z = m(3,2) - m(1,2);
    planes[3].distance = m(3,3) - m(1,3);
    
    // Near plane: m[3] + m[2]
    planes[4].normal.x = m(3,0) + m(2,0);
    planes[4].normal.y = m(3,1) + m(2,1);
    planes[4].normal.z = m(3,2) + m(2,2);
    planes[4].distance = m(3,3) + m(2,3);
    
    // Far plane: m[3] - m[2]
    planes[5].normal.x = m(3,0) - m(2,0);
    planes[5].normal.y = m(3,1) - m(2,1);
    planes[5].normal.z = m(3,2) - m(2,2);
    planes[5].distance = m(3,3) - m(2,3);
    
    // Normalize all planes
    for (int i = 0; i < 6; i++) {
        float length = cgp::norm(planes[i].normal);
        if (length > 0.0f) {
            planes[i].normal /= length;
            planes[i].distance /= length;
        }
    }
    
    // Debug: Print the first plane to see if it looks reasonable
    static bool debugPrinted = false;
    if (!debugPrinted) {
        std::cout << "Left plane normal: (" << planes[0].normal.x << ", " << planes[0].normal.y << ", " << planes[0].normal.z << ")" << std::endl;
        std::cout << "Left plane distance: " << planes[0].distance << std::endl;
        debugPrinted = true;
    }
}

bool Frustum::isAABBInside(const AABB& aabb) const {
    for (int i = 0; i < 6; i++) {
        // Find the vertex of the AABB that's farthest in the direction of the plane normal
        // This is the "positive vertex" - if it's outside, the whole AABB is outside
        cgp::vec3 positiveVertex;
        positiveVertex.x = (planes[i].normal.x >= 0) ? aabb.max.x : aabb.min.x;
        positiveVertex.y = (planes[i].normal.y >= 0) ? aabb.max.y : aabb.min.y;
        positiveVertex.z = (planes[i].normal.z >= 0) ? aabb.max.z : aabb.min.z;
        
        // If the positive vertex is outside this plane, the entire AABB is outside the frustum
        if (planes[i].distanceToPoint(positiveVertex) > 0) {
            return false;  // Cull this chunk
        }
    }
    return true;  // Chunk is inside or intersecting frustum, render it
}