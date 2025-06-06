#pragma once
#include "cgp/cgp.hpp"
#include "block/grass.hpp"
#include "block/sand.hpp"
#include "block/stone.hpp"

class Primary_World{
    private:

        Grass grass;
        Stone stone;
        Sand sand;
        std::vector<Block*> vectorBlockType;

    public:
        Primary_World();
        ~Primary_World();
        

        void initialize();
        std::vector<Block*> getVectorBlockType() const;

};