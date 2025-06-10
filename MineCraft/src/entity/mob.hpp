#pragma once

#include "cgp/cgp.hpp"
#include "entity/entity.hpp"


class Mob : public Entity{
    protected:

    cgp::mesh_drawable mesh_drawable;
    cgp::mesh mesh;
    bool mesh_initialized;
    bool mesh_drawable_initialized;
    std::string filename_mesh_obj;
    std::string filename_mesh_texture;

    public:

    void initialize();
    void initialize_mesh();
    void initialize_drawable_mesh();

    cgp::mesh get_mesh() const;
    cgp::mesh_drawable get_mesh_drawable() const;

};