#include "inventory.hpp"

//Methode
void Inventory::initialize(){
    
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

