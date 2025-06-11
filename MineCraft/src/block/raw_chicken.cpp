#include "block/raw_chicken.hpp"

Raw_chicken::Raw_chicken(){
    give_hunger = 4;
    item_name = "Chicken";
}

void Raw_chicken::action (int* entier){
    if (*entier<20)
    {
        *entier += give_hunger;
        if (*entier>20)
        {
            *entier=20;
        }
    }
    std::cout << *entier << std::endl; 
}

int Raw_chicken::get_hunger() const{
    return give_hunger;
}

int& Raw_chicken::set_hunger(){
    return give_hunger;
}