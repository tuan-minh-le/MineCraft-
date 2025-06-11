#include "block/raw_chicken.hpp"

void Raw_chicken::action (Player& player){
    if (player.get_hunger() < 20)
    {
        player.set_hunger() += give_hunger;
        if (player.get_hunger()>20)
        {
            player.set_hunger() = 20;
        }
    }
    
}

void Raw_chicken::action (){

}

void Raw_chicken::initialize(){
    give_hunger = 4;
}