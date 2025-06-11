#include "block/craft_table.hpp"

void Craft_table::action (){
    opened = true;
}

void Craft_table::initialize(){
    opened = false;
}

bool Craft_table::get_opened() const{
    return opened;
}
bool& Craft_table::set_opened(){
    return opened;
}