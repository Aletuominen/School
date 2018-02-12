#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

// Ohjelma ottaa vastaan henkilöpareja järjestyksessä Suosittelija Suositeltava
// ja tallentaa nämä verkostoon, jonka syvyyden ja laajuuden voi laskea. Lisäksi
// ohjelma osaa tulostaa verkoston oikein sisennettynä

// Aleksi Tuominen

const std::string HELP_TEXT = "S = store id1 i2\n P = print id\n"
                              "C = count id\n D = depth id";

std::vector < std::vector < std::string >> verkosto;
using Verkko = std::vector < std::vector < std::string > >;

std::vector<std::string> split(const std::string& s, const char delimiter, bool ignore_empty = false){
    std::vector<std::string> result;
    std::string tmp = s;

    while(tmp.find(delimiter) != std::string::npos)
    {
        std::string new_part = tmp.substr(0, tmp.find(delimiter));
        tmp = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        if(not (ignore_empty and new_part.empty()))
        {
            result.push_back(new_part);
        }
    }
    if(not (ignore_empty and tmp.empty()))
    {
        result.push_back(tmp);
    }
    return result;
}

void command_s (std::string nimi1, std::string nimi2, Verkko & verkosto){

    std::vector < std::string > nimet;
    nimet.push_back(nimi1);
    nimet.push_back(nimi2);
    verkosto.push_back(nimet);
}

void command_p (std::string nimi, Verkko verkosto, int kierros = 0){

    std::string pisteet;

    for (int i = 0; i < kierros; i++){
        pisteet += "..";
    }

    std::cout << pisteet << nimi << std::endl;

    for (unsigned int i = 0; i < verkosto.size(); i++){

        if (verkosto[i][0] == nimi){

            command_p(verkosto[i][1], verkosto, kierros + 1);
        }
    }

    return;
}

int command_c (std::string nimi, Verkko verkosto, int count = 0){

    for (unsigned int i = 0; i < verkosto.size(); i++){

        if (verkosto[i][0] == nimi){

            count = command_c(verkosto[i][1], verkosto, count + 1);
        }

    }
    return count;
}

std::vector < int > command_d (std::string nimi, Verkko verkosto, std::vector < int > depth = {1, 1}){

    depth[1] = std::max(depth[0], depth[1]);

    for (unsigned int i = 0; i < verkosto.size(); i++){

        if (verkosto[i][0] == nimi){

            depth[0] += 1;
            depth = command_d(verkosto[i][1], verkosto, depth);
        }
    }
    depth[0] -= 1;
    return depth;
}

int main()
{

    while(true){
        std::string line;
        std::cout << "> ";
        getline(std::cin, line);
        std::vector<std::string> parts = split(line, ' ', true);

        std::string command = parts.at(0);

        if(command == "S" or command == "s"){
            if(parts.size() != 3){
                std::cout << "Erroneous parameters!" << std::endl << HELP_TEXT;
            }
            std::string id1 = parts.at(1);
            std::string id2 = parts.at(2);

            command_s(id1, id2, verkosto);

        } else if(command == "P" or command == "p"){
            if(parts.size() != 2){
                std::cout << "Erroneous parameters!" << std::endl << HELP_TEXT;
            }
            std::string id = parts.at(1);

            command_p(id, verkosto);

        } else if(command == "C" or command == "c"){
            if(parts.size() != 2){
                std::cout << "Erroneous parameters!" << std::endl << HELP_TEXT;
            }
            std::string id = parts.at(1);

            std::cout << command_c(id, verkosto) << std::endl;

        } else if(command == "D" or command == "d"){
            if(parts.size() != 2){
                std::cout << "Erroneous parameters!" << std::endl << HELP_TEXT;
            }
            std::string id = parts.at(1);

            std::cout << command_d(id, verkosto)[1] << std::endl;

        } else if(command == "Q" or command == "q"){
           return EXIT_SUCCESS;
        } else {
            std::cout << "Erroneous command!" << std::endl << HELP_TEXT;
        }
    }
}
