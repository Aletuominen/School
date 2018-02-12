//------------------------------------------------
//  TIE-02200 Ohjelmoinnin peruskurssi -mallikoodi
//  Copyright © 2017  opersk@tut.fi
//------------------------------------------------

#include "todo_rakenne.hh"
#include "tiedostonluku.hh"
#include "kayttoliittyma.hh"
#include "apufunktiot.hh"
#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;


int main() {
    Todo tehtavat;

    string tiedoston_nimi;
    cout << "Syota tehtavatiedoston nimi: ";
    getline(cin, tiedoston_nimi);

    ifstream virta(tiedoston_nimi);
    if ( not virta ) {
        virheilmoitus("syotetiedostoa " + tiedoston_nimi + " ei voida lukea.");

        // main:in on palautettava nollasta eroava arvo, mikäli
        // ohjelman suoritus päättyi virhetilanteeseen.
        return 1;

    } else if ( not lue_tehtavatiedosto(virta, tehtavat) ) {
        virheilmoitus("syotetiedosto " + tiedoston_nimi + " virheellinen.");
        return 1;

    } else {

        virta.close();

        kayttoliittyma(tehtavat);
    }
}
