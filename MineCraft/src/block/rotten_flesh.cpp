#include "block/rotten_flesh.hpp"

Rotten_flesh::Rotten_flesh(){
    give_hunger = 2;
    item_name = "Rotten";
}

void Rotten_flesh::action (int* entier){
    if (*entier<20)
    {
        *entier += give_hunger;
        if (*entier>20)
        {
            *entier=20;
        }
        
    } 
}

int Rotten_flesh::get_hunger() const{
    return give_hunger;
}

int& Rotten_flesh::set_hunger(){
    return give_hunger;
}