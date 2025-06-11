#pragma once

#include "cgp/cgp.hpp"
#include "item.hpp"

class Tool : public Item{
    protected:
    

    public:

    virtual void action (int* entier) = 0;
        
};