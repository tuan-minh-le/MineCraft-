#pragma once
#include "cgp/cgp.hpp"
#include "item.hpp"

class Block : public Item {
protected:
    int dimension = 1;
    
    static cgp::mesh shared_cube_mesh;
    static bool mesh_initialized;
    cgp::vec3 position;

public:
    Block(); 
    virtual void initialize() = 0;
    
    static void initialize_shared_mesh();
    
    virtual void draw_block_at(const cgp::environment_generic_structure& environment);
    
    virtual void setup_texture() = 0;
    
    // Getter Setter
    int getDimension() const;

    cgp::vec3 getPosition()const;
    cgp::vec3& setPosition();
};