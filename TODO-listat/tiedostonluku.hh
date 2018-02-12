//------------------------------------------------
//  TIE-02200 Ohjelmoinnin peruskurssi -mallikoodi
//  Copyright © 2017  opersk@tut.fi
//------------------------------------------------

#ifndef TIEDOSTONLUKU_HH
#define TIEDOSTONLUKU_HH

#include "todo_rakenne.hh"
#include <fstream>

using namespace std;


bool lue_tehtavatiedosto(ifstream& tiedosto, Todo& tehtavarakenne);


#endif
