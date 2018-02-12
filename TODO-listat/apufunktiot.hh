//------------------------------------------------
//  TIE-02200 Ohjelmoinnin peruskurssi -mallikoodi
//  Copyright © 2017  opersk@tut.fi
//------------------------------------------------

#ifndef APUFUNKTIOT_HH
#define APUFUNKTIOT_HH

#include <string>
#include <vector>

using namespace std;


void virheilmoitus(const string& teksti);
vector<string> paloittele_sanoiksi(const string& teksti);
bool muuta_string_intiksi(const string& mjono, int& tulos);
string yhdista_valilyonneilla(vector<string>::const_iterator it,
                              vector<string>::const_iterator loppu);
bool onko_merkkijono_tyhja(const string &mjono);
vector<string> paloittele_puolipisteesta(const string& teksti);
#endif
