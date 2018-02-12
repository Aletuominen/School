//------------------------------------------------
//  TIE-02200 Ohjelmoinnin peruskurssi -mallikoodi
//  Copyright © 2017  opersk@tut.fi
//------------------------------------------------


#include "todo_rakenne.hh"
#include "apufunktiot.hh"
#include "todo_rakenne.hh"
#include <fstream>
#include <string>

using namespace std;


// ----------------------------------------------------------------------------
// lue_tehtavatiedosto
// -------------------
// Funktio lukee parametrina saamansa tietovirran avulla tehtävälistatiedoston
// ja lisää sieltä saamansa tiedot tehtavarakenteeseen.  Jos tiedostossa
// on virheellisiä rivejä, funktion on palautettava false.  Jos tiedosto on
// virheetön, paluuarvon oltava true.  Tiedoston lukemin tämän funktion
// avulla EI SAA TULOSTAA NÄYTÖLLE MITÄÄN, sillä virheilmoitusten tulostuksesta
// on huolehdittu pääohjelmassa.
//
// KOKO TIEDOSTON LUKEMINEN ON TOTEUTETTAVA REKURSION AVULLA.  Tämä funktio,
// eikä sen avuksi itse toteutetut apufunktiot saa sisältää yhtään silmukka-
// rakennetta.  Silmukkarakenteet ovat tässä yhteydessä työn hylkäämisperuste.
//
// Funktion paluuarvo ja parametrien tyypit ja määrä on säilytettävä.
// ET SIIS SAA MUUTTAA LUE_TEHTAVATIEDOSTO-FUNKTION RAJAPINTAA.
// ----------------------------------------------------------------------------
bool lue_tehtavatiedosto(ifstream& virta, Todo& tehtavarakenne) {

    // Funktio lukee tiedostovirrasta tekstirivejä yksitellen.
    // Funktion toimintaperiaate on rekursiivinen
    // Jos jollain tekstirivillä havaitaan virhe, funktio antaa
    // paluuarvokseen "false". Muussa tapauksessa paluuarvo on "true"

    string rivi;
    getline(virta, rivi);

    if (virta.eof()){

        return true;
    }

    if (onko_merkkijono_tyhja(rivi)){

        return false;
    }

    vector<string> rivi_paloiteltu = paloittele_puolipisteesta(rivi);

    if (rivi_paloiteltu.empty()){

        // Funktio palauttaa tyhjän vectorin jos puolipiste on
        // rivillä väärässä kohdassa, tämä virhe johtaa
        // false-paluuarvoon

        return false;
    }

    string tehtava = rivi_paloiteltu[1];

    int tarkeys;

    if (not muuta_string_intiksi(rivi_paloiteltu[0] ,tarkeys)){

        // Funktio muuttaa tekstinpätkän intiksi tai palauttaa
        // false jos muutos ei ollut mahdollinen, false välitetään
        // eteenpäin

        return false;
    }

    if (not tehtavarakenne.lisaa_tehtava(tarkeys, tehtava)){

        // Funktio lisää uuden tehtävän Todo-luokkaan. Virheiden
        // sattuessa palautetaan false ja false välitetään eteenpäin

        return false;
    }

    if (!virta.eof()){

        // Jos tiedostoa ei ole vielä luettu loppuun, luetaan uusi rivi

        if(not lue_tehtavatiedosto(virta, tehtavarakenne)){

            // Aiempien ehtolauseiden false palautuu tänne, josta
            // se välitetään eteenpäin alkuperäiseen funktiokutsuun

            return false;
        }
    }

    // Jos yhtäkään false-paluuarvoa ei ole saavutettu, palautetaan
    // true ja ohjelman suoritus jatkuu

    return true;

}
