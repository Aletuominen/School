#include <iostream>
#include <fstream>
#include <map>
#include <vector>

// Ohjelma lukee tekstitiedoston ja laskee jokaiselle sanalle rivien, joilla se esiintyy, määrän

// Aleksi Tuominen

void lisaa_mappiin (std::string sana, int kerrat, std::map < std::string, std::vector < int > >& mappi){

    if (mappi.find(sana) == mappi.end()){

        mappi[sana] = {1, kerrat};
        return;
    }

    for (unsigned int i = 1; i < mappi[sana].size(); i++){

        if (kerrat == mappi[sana][i]){ // jos sama sana on jo tullut rivillä vastaan
            return;
        }
    }
    mappi[sana][0] += 1;
    mappi[sana].push_back(kerrat);
}


int main()
{

    std::string tiedostonimi;
    std::map < std::string, std::vector < int > > sanat_kerrat;

    std::cout << "Input file: ";
    std::getline(std::cin, tiedostonimi);

    std::ifstream tiedostovirta(tiedostonimi);

    if (not tiedostovirta){
        std::cout << "Error! The file " << tiedostonimi << " cannot be opened." << std::endl;
        tiedostovirta.close();
        return EXIT_FAILURE;
    }

    std::string rivi;
    std::string sana = "";
    unsigned int rivinumero = 1;

    while (getline(tiedostovirta, rivi)){

        for (unsigned int i = 0; i < rivi.size(); i++){

            if (i == rivi.size() - 1){
                sana += rivi[i];
                lisaa_mappiin(sana, rivinumero, sanat_kerrat);
                sana = "";
            }

            else if (rivi[i] == ' '){

                lisaa_mappiin(sana, rivinumero, sanat_kerrat);
                sana = "";
            }

            else{
                sana += rivi[i];
            }
        }

        rivinumero += 1;
    }

    std::map < std::string, std::vector < int > >::iterator it;

    for(it = sanat_kerrat.begin(); it != sanat_kerrat.end(); it++){

        std::cout << it->first << " ";

        if (it->second.size() == 2){
            std::cout << it->second[0] << ": " << it->second[1] << std::endl;
        }

        else{
            std::cout << it->second[0] << ": ";

            for (unsigned int numero = 1; numero < it->second.size(); numero++){
                if (numero == it->second.size() - 1){
                    std::cout << it->second[numero] << std::endl;
                }

                else{
                    std::cout << it->second[numero] << ", ";
                }
            }
        }
    }

    return 0;




}
