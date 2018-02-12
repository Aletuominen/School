//------------------------------------------------
//  TIE-02200 Ohjelmoinnin peruskurssi -mallikoodi
//  Copyright © 2017  opersk@tut.fi
//------------------------------------------------

#include "todo_rakenne.hh"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <list>

using namespace std;


Todo::Todo() {
}

// ----- Uuden tehtavan lisääminen todo-listalle ------------------------------

bool Todo::lisaa_tehtava(int kiireellisyys, const string& kuvaus) {
    if ( kiireellisyys < MIN_KIIREELLISYYS
           or kiireellisyys > MAX_KIIREELLISYYS ) {

        return false;
    }

    if ( todo.find(kiireellisyys) == todo.end() ) {
        todo.insert( { kiireellisyys, {} } );
    }

    todo.at(kiireellisyys).push_back(kuvaus);

    return true;
}


// ----- Tehtävien tulostamiseen liittyvät metodit ja apumetodit --------------

void Todo::tulosta_lista_rekursiolla(
        int rivinumero, int kiireellisyys,
        Tehtavalista::const_iterator it,
        Tehtavalista::const_iterator loppu) const {

    if ( it == loppu ) {
        return;

    } else {

        cout << setw(2) << rivinumero << ". taso " << kiireellisyys << ": "
             << '"' << *it << '"' << endl;

        ++it;
        ++rivinumero;

        tulosta_lista_rekursiolla(rivinumero, kiireellisyys, it, loppu);
    }
}


void Todo::tulosta_mappi_rekursiolla(Todo_map::const_reverse_iterator it,
                                     Todo_map::const_reverse_iterator loppu,
                                     int rivinumero) const {
    if ( it ==loppu ) {
        return;

    } else {

        int kiireellisyystaso = it->first;

        tulosta_lista_rekursiolla(rivinumero, kiireellisyystaso,
                                  it->second.begin(), it->second.end());

        rivinumero += it->second.size();
        ++it;

        tulosta_mappi_rekursiolla(it, loppu, rivinumero);
    }
}


void Todo::listaa_tehtavat() const {
    tulosta_mappi_rekursiolla(todo.rbegin(), todo.rend(), 1);
}

bool Todo::suorita()  {

    // Funktio iteroi todo-mapin läpi, tulostaa tehtävä-listan
    // ensimmäisen alkion ja poistaa sen tulostamisen jälkeen.
    // Tämän jälkeen palautetaan true
    // Jos alkioita ei löydy eli todo-map on tyhjä, palautetaan
    // false.

    Todo_map::reverse_iterator it;
    Tehtavalista::iterator it2;
    for (it = todo.rbegin(); it != todo.rend(); it++ ){

        int kiireellisyys = it->first;
        Tehtavalista lista = it->second;

        for (it2 = lista.begin(); it2 != lista.end(); it2++){

            string tehtava = *it2;

            if (it2 == lista.begin()){
                cout << "Suoritusvuorossa: " << tehtava << endl;
                todo.at(kiireellisyys).pop_front();
                return true;
            }
        }

    }

    return false;

}

// ----- Tehtävälistan tallentaminen tiedostoon apumetodeineen ----------------

bool Todo::talleta_lista_tiedostoon_rekursiolla(
         Tehtavalista::const_iterator it,
         Tehtavalista::const_iterator loppu,
         int taso,
         ofstream& virta) const {

    if ( not virta ) {
        return false;

    } else if ( it == loppu ) {
        return true;

    } else {

        virta << taso << ";" << *it << endl;

        ++it;

        return talleta_lista_tiedostoon_rekursiolla(it, loppu, taso, virta);
    }
}


bool Todo::talleta_mappi_tiedostoon_rekursiolla(
         Todo_map::const_iterator it,
         Todo_map::const_iterator loppu,
         ofstream& virta ) const {

    if ( it == loppu ) {
        return true;

    } else {

        if ( not talleta_lista_tiedostoon_rekursiolla(
                     it->second.begin(), it->second.end(),
                     it->first, virta) ) {

            return false;
        }

        ++it;

        return talleta_mappi_tiedostoon_rekursiolla(it, loppu, virta);
    }
}


bool Todo::talleta_tiedostoon(ofstream& virta) const {
    return talleta_mappi_tiedostoon_rekursiolla(
               todo.begin(), todo.end(), virta);
}
