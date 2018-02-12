//------------------------------------------------
//  TIE-02200 Ohjelmoinnin peruskurssi -mallikoodi
//  Copyright © 2017  opersk@tut.fi
//------------------------------------------------

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;


void virheilmoitus(const string& teksti) {
    cout << "Virhe: " << teksti << endl;
}


vector<string> paloittele_sanoiksi(const string& teksti) {
    istringstream virta(teksti);
    vector<string> tulos = { };

    string sana;
    while ( virta >> sana ) {
        tulos.push_back(sana);
    }

    return tulos;
}


bool muuta_string_intiksi(const string& mjono, int& tulos) {
    istringstream virta(mjono);

    int aputulos;

    virta >> aputulos;
    if ( not virta ) {
        return false;
    }

    virta >> ws;
    virta.get();

    if ( not virta.eof() ) {
        return false;

    } else {

        tulos = aputulos;

        return true;
    }
}


string yhdista_valilyonneilla(vector<string>::const_iterator it,
                              vector<string>::const_iterator loppu) {
    if ( it == loppu ) {
        return "";

    } else if ( it + 1 == loppu ) {
        return *it;

    } else {

        return *it + " " + yhdista_valilyonneilla(it + 1, loppu);
    }
}

bool onko_merkkijono_tyhja(const string &mjono){

    // Funktio lukee merkkijonon merkki merkiltä ja palauttaa false
    // jos löytää muita merkkejä kuin välilyöntejä
    // Jos merkkijono sisälsi pelkkiä välilyöntejä, palautetaan true

    for (string::const_iterator it = mjono.begin();
                                it != mjono.end(); ++it)
        if (*it != ' '){

            return false;
        }

    return true;
}

vector<string> paloittele_puolipisteesta(const string& teksti){

    // Funktio etsii tekstiriviltä puolipisteen ja paloittelee
    // tekstirivin kahtia sen molemmilta puolin.
    // Jos puolipiste on väärässä kohdassa, palautetaan tyhjä
    // vector, jotta funktiota kutsunut ohjelma tietää virheen
    // tapahtuneen
    // Muussa tapauksessa palautetaan (kiireellisyys, tehtävä)-
    // muotoinen vector

    char erotinkohta = teksti.find(';');

    if (erotinkohta != 1){

        return {};
    }

    vector<string> tulos = { };

    string kiireellisyys = teksti.substr(0, erotinkohta);
    string tehtava = teksti.substr(erotinkohta + 1);

    tulos.push_back(kiireellisyys);
    tulos.push_back(tehtava);

    return tulos;
}


