//------------------------------------------------
//  TIE-02200 Ohjelmoinnin peruskurssi -mallikoodi
//  Copyright © 2017  opersk@tut.fi
//------------------------------------------------

#ifndef TODO_RAKENNE_HH
#define TODO_RAKENNE_HH

#include <map>
#include <list>
#include <string>

using namespace std;


const int MIN_KIIREELLISYYS = 1;
const int MAX_KIIREELLISYYS = 5;


using Tehtavalista = list<string>;
using Todo_map = map<int, Tehtavalista>;


class Todo {
  public:
    Todo();

    bool lisaa_tehtava(int kiireellisyys, const string& kuvaus);
    void listaa_tehtavat() const;
    bool suorita();
    bool talleta_tiedostoon(ofstream& virta) const;


  private:
    void tulosta_lista_rekursiolla(int rivinumero, int kiireellisyys,
                                   Tehtavalista::const_iterator it,
                                   Tehtavalista::const_iterator loppu) const;
    void tulosta_mappi_rekursiolla(Todo_map::const_reverse_iterator it,
                                   Todo_map::const_reverse_iterator loppu,
                                   int rivinumero) const;

    bool talleta_lista_tiedostoon_rekursiolla(
                                   Tehtavalista::const_iterator it,
                                   Tehtavalista::const_iterator loppu,
                                   int taso,
                                   ofstream& virta) const;
    bool talleta_mappi_tiedostoon_rekursiolla(
                                   Todo_map::const_iterator it,
                                   Todo_map::const_iterator loppu,
                                   ofstream& virta ) const;

    Todo_map todo;
};

#endif
