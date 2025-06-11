#pragma once

#include "block/tool.hpp"
#include "block/block.hpp"

class Interractive : public Block, public Tool{
    protected:

    public:

    virtual void action (int* entier) = 0;
    virtual ~Interractive() = default;
    
};