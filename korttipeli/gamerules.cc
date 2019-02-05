#include "gamerules.hh"
#include "card.hh"

#include <iostream>

// Funktio saa parametrinaan alla olevan kortin ja päälle tulevan kortin
// tiedot merkkijonomuodossa.


bool GameRules::checkIfSame(std::string bottom, std::string top)
{

    std::string topSuitString = top.substr(top.find_first_of(",")+1,1);
    std::string bottomSuitString = bottom.substr(bottom.find_first_of(",")+1,1);
    return topSuitString == bottomSuitString;

}

bool GameRules::checkIfSpade(std::__cxx11::string bottom, std::__cxx11::string top)
{
    std::string bottomSuitString = bottom.substr(bottom.find_first_of(",")+1,1);
    std::string bottomNumberString = bottom.substr(0, bottom.find_first_of(","));
    std::string topSuitString = top.substr(top.find_first_of(",")+1,1);
    std::string topNumberString = top.substr(0, top.find_first_of(","));
    int topNumber = std::stoi(topNumberString);

    if (bottomSuitString.empty()){
        return stoi(topSuitString) == SPADE && topNumber == 1;
    }
    int bottomNumber = std::stoi(bottomNumberString);

    return stoi(topSuitString) == SPADE && bottomNumber + 1 == topNumber;
}
bool GameRules::checkIfHeart(std::__cxx11::string bottom, std::__cxx11::string top)
{
    std::string bottomSuitString = bottom.substr(bottom.find_first_of(",")+1,1);
    std::string bottomNumberString = bottom.substr(0, bottom.find_first_of(","));
    std::string topSuitString = top.substr(top.find_first_of(",")+1,1);
    std::string topNumberString = top.substr(0, top.find_first_of(","));
    int topNumber = std::stoi(topNumberString);

    if (bottomSuitString.empty()){
        return stoi(topSuitString) == HEART && topNumber == 1;
    }
    int bottomNumber = std::stoi(bottomNumberString);

    return stoi(topSuitString) == HEART && bottomNumber + 1 == topNumber;
}

bool GameRules::checkIfDiamond(std::__cxx11::string bottom, std::__cxx11::string top)
{
    std::string bottomSuitString = bottom.substr(bottom.find_first_of(",")+1,1);
    std::string bottomNumberString = bottom.substr(0, bottom.find_first_of(","));
    std::string topSuitString = top.substr(top.find_first_of(",")+1,1);
    std::string topNumberString = top.substr(0, top.find_first_of(","));
    int topNumber = std::stoi(topNumberString);

    if (bottomSuitString.empty()){
        return stoi(topSuitString) == DIAMOND && topNumber == 1;
    }
    int bottomNumber = std::stoi(bottomNumberString);

    return stoi(topSuitString) == DIAMOND && bottomNumber + 1 == topNumber;
}

bool GameRules::checkIfClub(std::__cxx11::string bottom, std::__cxx11::string top)
{
    std::string bottomSuitString = bottom.substr(bottom.find_first_of(",")+1,1);
    std::string bottomNumberString = bottom.substr(0, bottom.find_first_of(","));
    std::string topSuitString = top.substr(top.find_first_of(",")+1,1);
    std::string topNumberString = top.substr(0, top.find_first_of(","));
    int topNumber = std::stoi(topNumberString);

    if (bottomSuitString.empty()){
        return stoi(topSuitString) == CLUB && topNumber == 1;
    }
    int bottomNumber = std::stoi(bottomNumberString);

    return stoi(topSuitString) == CLUB && bottomNumber + 1 == topNumber;
}
