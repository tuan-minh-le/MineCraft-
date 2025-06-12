
#include "block/craft_table.hpp"


Craft_table::Craft_table(){
    opened = false;
    item_name = "Crafting Table";
    type = BlockType::CRAFT_TABLE;
    craft_inventory.initialize(10,false);
}

void Craft_table::initialize() {
    initialize_shared_mesh();
    
}

void Craft_table::setup_texture(){

}

void Craft_table::draw_block_at(const cgp::environment_generic_structure& environment) {
    initialize_shared_mesh();

    static cgp::mesh_drawable ct_drawable;
    static bool drawable_initialized = false;
    
    if (!drawable_initialized) {
        ct_drawable.initialize_data_on_gpu(shared_cube_mesh);
        
        ct_drawable.texture.load_and_initialize_texture_2d_on_gpu("assets/crafting_table.png");
        drawable_initialized = true;    
    }
    
    ct_drawable.model.translation = position;
    cgp::draw(ct_drawable, environment);
}

void Craft_table::action (){
    opened = true;
}

bool Craft_table::get_opened() const{
    return opened;
}
bool& Craft_table::set_opened(){
    return opened;
}