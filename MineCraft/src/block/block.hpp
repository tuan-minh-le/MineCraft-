#pragma once
#include "cgp/cgp.hpp"
#include "item.hpp"

class Block : public Item {
protected:
    int dimension = 1;
    
    static cgp::mesh shared_cube_mesh;
    static bool mesh_initialized;

public:
    Block(); 
    virtual void initialize() = 0;
    
    static void initialize_shared_mesh();
    
    static void draw_cube_at(const cgp::vec3& position, const cgp::environment_generic_structure& environment);
    
    virtual void setup_texture() = 0;
    
    // Getter
    int getDimension() const;
};