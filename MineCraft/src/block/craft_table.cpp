#include "block/craft_table.hpp"

void Craft_table::action (int* entier){
    opened = true;
}

Craft_table::Craft_table(){
    opened = false;
}

bool Craft_table::get_opened() const{
    return opened;
}
bool& Craft_table::set_opened(){
    return opened;
}