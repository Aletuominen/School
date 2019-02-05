#ifndef GAMERULES_HH
#define GAMERULES_HH
#include <string>

namespace GameRules {

    // Tarkastusfunktio, joka varmistaa, että päälle laitetaan vain herttaa.
    bool checkIfHeart(std::string bottom, std::string top);
    bool checkIfSpade(std::string bottom, std::string top);
    bool checkIfDiamond(std::string bottom, std::string top);
    bool checkIfClub(std::string bottom, std::string top);

    bool checkIfSame(std::string bottom, std::string top);
    bool matchSuit(std::string bottom, std::string top);
}

#endif // GAMERULES_HH
