#include "inventory.hpp"
#include "block/craft_table.hpp"

Inventory::Inventory(){

}
Inventory::~Inventory(){

}


void Inventory::initialize(int size,bool add_craft,bool liitle){
    set_inventory_size() = size;
    little=liitle;
    
    for (int i = 0; i < get_inventory_size();i++)
    {
        inventory.push_back(std::vector<std::shared_ptr<Item>>());
    }
    for (int i = 0; i < 40; i++)
    {
        inventory[0].push_back(std::shared_ptr<Item>(new Grass()));
        inventory[1].push_back(std::shared_ptr<Item>(new Stone()));
        inventory[2].push_back(std::shared_ptr<Item>(new Raw_chicken()));
        if (add_craft)
        {
            inventory[3].push_back(std::shared_ptr<Item>(new Craft_table()));
        }
    }
}


void Inventory::open_inventory(){
    
}

void Inventory::close_inventory(){

}

bool Inventory::add_inventory (std::shared_ptr<Item> item){
    for (auto& it : inventory)
    {
        if (it.empty())
        {
            it.push_back(item);
            return true;
        }
        else if (it[0]->getItemName()==item->getItemName() && it.size()<max_size)
        {
            it.push_back(item);
            return true;
        }
    }
    return false;
}

bool Inventory::erase_inventory (int ind){
    if (0<=ind<get_inventory_size())
    {
        if (inventory[ind].empty())
        {
           return false;
        }
        else{
            inventory[ind].erase(inventory[ind].begin());
            return true;
        }
    }
    return false;
}

void Inventory::fast_switch_inventory(int ind,SlotType from_type, Inventory& inv){
    int tail;
    if (little) {tail = 2;} else{tail = 3;}

    if (from_type == SlotType::CRAFT)
    {
        if (ind==tail)
        {
            bool test_bool = true;
            while(test_bool){
                finish_craft(false);
                test_bool = check_craft();
            }
        }

        for (int i = 0; i < inv.get_inventory_size(); i++)
        {
            if (inv.get_inventory()[i].empty())
            {
                std::swap(inventory[ind],inv.get_inventory()[i]);
                return;
            }
            if (inv.get_inventory()[i][0]->getItemName()==inventory[ind][0]->getItemName())
            {
                while (inv.get_inventory()[i].size()<max_size)
                {
                    inv.get_inventory()[i].push_back(inventory[ind].front());
                    inventory[ind].erase(inventory[ind].begin());

                    if (inventory[ind].empty())
                    {
                        return;
                    }
                }
            }
        }
    }
    else
    {
        if (ind<get_inventory_size()/4)
        {
            for (int i = get_inventory_size()-1; i >= get_inventory_size()/4; i--)
            {
                if (inventory[i].empty())
                {
                    std::swap(inventory[ind],inventory[i]);
                    return;
                }

                if (inventory[i][0]->getItemName()==inventory[ind][0]->getItemName())
                {
                    while (inventory[i].size()<max_size)
                    {
                        inventory[i].push_back(inventory[ind].front());
                        inventory[ind].erase(inventory[ind].begin());

                        if (inventory[ind].empty())
                        {
                            return;
                        }
                    }
                }
            }
        }
        else
        {
            for (int i = 0; i < get_inventory_size()/4; i++)
            {
                if (inventory[i].empty())
                {
                    std::swap(inventory[ind],inventory[i]);
                    return;
                }

                if (inventory[i][0]->getItemName()==inventory[ind][0]->getItemName())
                {
                    while (inventory[i].size()<max_size)
                    {
                        inventory[i].push_back(inventory[ind].front());
                        inventory[ind].erase(inventory[ind].begin());

                        if (inventory[ind].empty())
                        {
                            return;
                        }
                    }
                }
            }
        }   
    }
}

int Inventory::get_inventory_size() const{
    return inventory_size;
}

int& Inventory::set_inventory_size(){
    return inventory_size;
}

void Inventory::switch_inventory(int ind1, int ind2, SlotType from_type, SlotType to_type, Inventory& from_inv,bool split, bool one) {
    int tail;
    if (little) {tail = 2;} else{tail = 3;}
    if (ind2 == tail && to_type == SlotType::CRAFT)
    {
        return;
    }
    
    auto* to = &inventory;
    auto* from = (from_type == to_type) ? &inventory : &from_inv.set_inventory();

    int from_size;
    if (from_type == to_type)
    {
        from_size = get_inventory_size();
    }
    else
    {
        from_size = from_inv.get_inventory_size();
    }

    if (ind1 >= from_size || ind2 >= get_inventory_size()) {
        throw std::out_of_range("Index out of range");
    }

    auto& from_slot = (*from)[ind1];
    auto& to_slot = (*to)[ind2];

    bool from_empty = from_slot.empty();
    bool to_empty = to_slot.empty();

    int cpt = 0;
    int size = from_slot.size();

    // Cas 1 : un des deux slots est vide
    if (from_empty || to_empty) {
        if (split)
        {
            for (int i = 0; i < (size+1)/2; i++)
            {
                to_slot.push_back(from_slot.front());
                from_slot.erase(from_slot.begin());
            }
        }
        else if (one)
        {
            to_slot.push_back(from_slot.front());
            from_slot.erase(from_slot.begin());
        }
        
        else
        {
            std::swap(from_slot, to_slot);
        }
        return;
    }

    // Cas 2 : objets différents → swap total
    if (from_slot[0]->getItemName() != to_slot[0]->getItemName()) {
        std::swap(from_slot, to_slot);
        return;
    }

    // Cas 3 : même item → empilement
    while (!from_slot.empty() && to_slot.size() < max_size) {

        if (split && cpt>=(size+1)/2)
        {
            return;
        }

        if(one){
            to_slot.push_back(from_slot.front());
            from_slot.erase(from_slot.begin());
            return;
        }
        
        to_slot.push_back(from_slot.front());
        from_slot.erase(from_slot.begin());
        cpt++;
    }
}


std::vector<std::vector<std::shared_ptr<Item>>>& Inventory::get_inventory(){
    return inventory;
}

std::vector<std::vector<std::shared_ptr<Item>>>& Inventory::set_inventory(){
    return inventory;
}

bool Inventory::check_craft(){
    int tail;
    if (little) {tail = 2;} else{tail = 3;}

    if (little)
    {
        if (inventory[3].empty() || inventory[0].empty())
        {
            return false;
        }
        
        if (inventory[3][0]->getItemName() == "Grass" && inventory[0][0]->getItemName() == "Grass" && inventory[1].empty() && inventory[4].empty())
        {
            inventory[2].push_back(std::make_shared<Stone>());

            return true;
        }
    }
    else
    {
        if (inventory[4].empty() || inventory[0].empty() || inventory[7].empty())
        {
            return false;
        }
        
        if (inventory[4][0]->getItemName() == "Grass" && inventory[0][0]->getItemName() == "Grass" && inventory[7][0]->getItemName() == "Grass" 
        && inventory[1].empty() && inventory[2].empty() && inventory[5].empty() && inventory[6].empty() && inventory[8].empty() && inventory[9].empty())
        {
            inventory[3].push_back(std::make_shared<Stone>());

            return true;
        }
    }
    
    return false;
}

void Inventory::finish_craft(bool erase){
    int tail;
    if (little) {tail = 2;} else{tail = 3;}
    for (int i = 0; i < get_inventory_size(); i++)
    {
        if (!(inventory[i].empty()))
        {
            if (erase || i!=tail)
            {
                inventory[i].erase(inventory[i].begin());
            }
        }
    }
}

bool Inventory::get_opened_inventory() const{
    return opened_inventory;
}

bool& Inventory::set_opened_inventory(){
    return opened_inventory;
}

