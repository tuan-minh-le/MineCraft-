#include "block/rotten_flesh.hpp"

void Rotten_flesh::action (Player& player){
    if (player.get_hunger() < 20)
    {
        player.set_hunger() += give_hunger;
        if (player.get_hunger()>20)
        {
            player.set_hunger() = 20;
        }
    }
    
}

void Rotten_flesh::initialize(){
    give_hunger = 2;
}