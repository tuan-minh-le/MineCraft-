#pragma once

#include "block/tool.hpp"
#include "block/block.hpp"

class Interractive : public Block{
    protected:

    public:

    virtual void action (int* entier) = 0;
    virtual ~Interractive() = default;
    
};