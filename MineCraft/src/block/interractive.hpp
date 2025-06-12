#pragma once

#include "block/tool.hpp"
#include "block/block.hpp"

class Interractive : public Block{
    protected:

    public:

    virtual void action () = 0;
    virtual ~Interractive() = default;
    
};