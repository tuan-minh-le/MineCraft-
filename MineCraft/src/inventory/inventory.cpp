#include "inventory.hpp"

Inventory::Inventory(){

}
Inventory::~Inventory(){

}

void Inventory::draw_inventory(){
    glDisable(GL_DEPTH_TEST); // Pour HUD
    // setupOrtho(); // Matrice ortho

    // for (int i = 0; i < inventory_size; i++) {
    //     float x = col * slotWidth;
    //     float y = row * slotHeight;

    //     drawSlotBackground(x, y);
    //     if (inventory[row][col].itemId != 0) {
    //         drawItemIcon(inventory[row][col].itemId, x, y);
    //         drawItemCount(inventory[row][col].count, x, y);
    //     }
    // }

    glEnable(GL_DEPTH_TEST);
}


void Inventory::initialize(int size){
    inventory_size = size;
    for (int i = 0; i < inventory_size; i++)
    {
        inventory.push_back(nullptr);
    }
}


void Inventory::open_inventory(){
    
}

void Inventory::close_inventory(){

}

void Inventory::add_inventory (std::shared_ptr<Item> item, int ind){
    if (ind > inventory_size) {
        throw std::out_of_range("Index out of range");
    }
    if (ind == inventory_size) {
        inventory.push_back(item);
    } else {
        inventory.insert(inventory.begin() + ind, item);
    }
}

void Inventory::erase_inventory (int ind){
    if (ind >= inventory_size) {
        throw std::out_of_range("Index out of range");
    }
    inventory.erase(inventory.begin() + ind);
}

void Inventory::switch_inventory(int ind1, int ind2){
    if (ind1 >= inventory_size || ind2 >= inventory_size) {
        throw std::out_of_range("Index out of range");
    }
    std::swap(inventory[ind1], inventory[ind2]);
}

//Getter Setter

std::vector<std::shared_ptr<Item>> Inventory::get_inventory() const{
    return inventory;
}

std::vector<std::shared_ptr<Item>>& Inventory::set_inventory(){
    return inventory;
}

bool Inventory::get_opened_inventory() const{
    return opened_inventory;
}

bool& Inventory::set_opened_inventory(){
    return opened_inventory;
}

