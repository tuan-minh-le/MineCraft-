#pragma once
#include "cgp/cgp.hpp"

// Represents a 3D plane using the equation ax + by + cz + d = 0
struct Plane {
    cgp::vec3 normal;  // (a, b, c) - plane normal vector
    float distance;    // d - distance from origin
    
    // Calculate signed distance from point to plane
    float distanceToPoint(const cgp::vec3& point) const;
};

// Axis-Aligned Bounding Box for chunks
struct AABB {
    cgp::vec3 min, max;
    
    AABB(const cgp::vec3& center, const cgp::vec3& size);
};

class Frustum {
private:
    Plane planes[6];  // Left, Right, Bottom, Top, Near, Far
    
public:
    // Extract frustum planes from view-projection matrix
    void extractFromMatrix(const cgp::mat4& m);
    
    // Test if AABB is inside frustum (chunk culling)
    bool isAABBInside(const AABB& aabb) const;
};