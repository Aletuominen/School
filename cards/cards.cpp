#include "cards.hh"
// TODO: Implement the methods here

Cards::Cards(): top_(nullptr){
}

Cards::~Cards(){

    Card_data* address_toremove;

    while (top_ != nullptr){

        address_toremove = top_;
        top_ = top_->down;
        delete address_toremove;
    }
}

void Cards::add(int id)
{

    Card_data* new_address = new Card_data{id, nullptr, nullptr};

    if (top_ == nullptr){
        top_ = new_address;
    }

    else{
        new_address->down = top_;
        top_->up = new_address;
        top_ = new_address;
    }
}

void Cards::print_from_top_to_bottom(std::ostream &s)
{

    Card_data* address_toprint = top_;
    int i = 1;
    while (address_toprint != nullptr){


        s << i << ": " << address_toprint->data << std::endl;
        address_toprint = address_toprint->down;
        ++i;
    }
}

bool Cards::remove(int &id)
{
    Card_data* temp_address;

    if(top_ == nullptr){
        return false;
    }
    else if(top_->down == nullptr){
        id = top_->data;
        delete top_;
    }
    else{
        id = top_->data;
        temp_address = top_->down;
        temp_address->up =nullptr;
        delete top_;
        top_ = temp_address;

    }
    return true;
}

bool Cards::bottom_to_top()
{
    Card_data* find_bottom;
    Card_data* temp;

    if(top_ == nullptr){
        return false;
    }
    else if(top_->down == nullptr){
        return false;
    }
    find_bottom = top_;

    while (find_bottom->down != nullptr){

        find_bottom = find_bottom->down;
    }

    temp = find_bottom->up;
    temp->down = nullptr;

    find_bottom->up = nullptr;
    find_bottom->down = top_;

    top_->up = find_bottom;
    top_ = find_bottom;

    return true;

}

bool Cards::top_to_bottom()
{

    Card_data* find_bottom;
    Card_data* temp;

    if(top_ == nullptr){
        return false;
    }
    else if(top_->down == nullptr){
        return false;
    }

    find_bottom = top_;

    while (find_bottom->down != nullptr){

        find_bottom = find_bottom->down;
    }

    temp = top_->down;
    temp-> up = nullptr;

    top_->down = nullptr;
    find_bottom->down = top_;

    top_->up = find_bottom;
    top_ = temp;

    return true;
}

void Cards::print_from_bottom_to_top(std::ostream &s)
{
    int i = 1;

    if(top_ == nullptr){
        return;
    }

    Card_data* find_bottom;

    find_bottom = top_;

    while (find_bottom->down != nullptr){

        find_bottom = find_bottom->down;
    }

    while (find_bottom != nullptr){

        s << i << ": " << find_bottom->data << std::endl;
        ++i;
        find_bottom = find_bottom->up;
    }


}
