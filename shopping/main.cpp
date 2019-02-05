#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <locale>

/* Ohjelma luo csv-tiedostosta tietorakenteen, jolle on mahdollista suorittaa
erilaisia operaatioita. Csv-tiedoston tulee sisältää 4 alkiota jokaisella
rivillä puolipisteellä (;) eroteltuna. Alkioiden tulee myös olla oikeassa
järjestyksessä: kauppaketjun_nimi;kaupan_sijainti;tuotteen_nimi;tuotteen_hinta
Tiedoston onnistuneen lukemisen jälkeen käyttäjälle aukeaa tekstipohjainen
komentokenttä, joka tuntee seuraavat komennot:

(tarvittavat parametrit ilmoitettu suluissa komennon perässä, parametrit
erotetaan toisistaan välilyönnillä
Esimerkiksi: stores K-citymarket)

quit: lopettaa ohjelman suorituksen ilman tulostuksia
chains: tulostaa kaikki tietokannasta löytyvät kauppaketjut aakkosjärjestyksessä
products: tulostaa kaikista kaupoista löytyvät tuotteet aakkosjärjestyksessä
stores (ketjun_nimi): tulostaa syötetyn kauppaketjun perusteella
kaikki siihen kuuluvat myymälät
cheapest (tuotteen nimi): etsii syötetyn tuotteen halvimman
hinnan ja halvimman tuotteen sijainnin/sijainnit
selection (ketjun_nimi, tuotteen_nimi: tulostaa syötetyn
kauppaketjun ja kaupan koko tuotevalikoiman

Tekijä: Aleksi Tuominen */

struct Tuote{ // Tuote-rakenteeseen tallennetaan tuotenimi-hinta-pareja

    std::string tuotenimi;
    double hinta;
};

// Merkitään tietoa loppuneesta tuotteesta asettamalla hinnaksi 9999.
// On kohtuullista olettaa, etteivät päivittäistavarakaupan
// hinnat nouse ikinä näin korkealle. Arvoa
// täytyy nostaa, jos konfliktit ovat mahdollisia.

const double TUOTE_LOPPU = 9999;

// Lyhennetään pitkät tietorakenteet helpommin kirjoitettaviksi

using Ketjumap = std::map <std::string, std::map < std::string,
                std::vector < Tuote > > >;

using Liikemap = std::map < std::string, std::vector < Tuote > >;

// Funktio pätkii merkkijonon vectoriksi. Parametreina otetaan
// pätkittävä merkkijono ja erotinmerkki
std::vector<std::string> split(const std::string& s, const char delimiter,
                               bool ignore_empty = false){

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

// Funktio palauttaa alkion, jossa tuote sijaitsee halutussa Tuote-vectorissa.
// Jos tuotetta ei ole vielä olemassa, palautetaan -1.
int onko_tuote_olemassa(const std::vector < Tuote >& tuotteet,
                        const std::string& tuote){

    for (unsigned int alkio = 0; alkio < tuotteet.size(); ++alkio){

        if(tuotteet[alkio].tuotenimi == tuote){
            return alkio;
        }
    }
    return -1;
}

// Tulostetaan kaikki tietorakenteen ostosketjut
void komento_chains (const Ketjumap& ketjut){

    for (Ketjumap::const_iterator it = ketjut.begin(); it != ketjut.end(); ++it){
        std::cout << it->first << std::endl;
    }
}

// Tulostetaan halutun ostosketjun kaikki myymälät. Jos ostosketjua ei ole
// rakenteessa, funktio tulostaa itse virheilmoituksen ja palaa pääohjelmaan.
void komento_stores (const std::string& ketju, const Ketjumap& ketjut,
                     bool loytyi = false){

    for (Ketjumap::const_iterator it = ketjut.begin(); it != ketjut.end(); ++it){

        if (it->first == ketju){
            loytyi = true;

            for (Liikemap::const_iterator it2 = it->second.begin();
                 it2 != it->second.end(); ++it2){

                std::cout << it2->first << std::endl;
            }
        }
    }
    // Koko rakenne käyty läpi eikä löytynyt yhtäkään --> virheilmoitus
    if (not loytyi){
        std::cout << "Error: an unknown chain" << std::endl;
    }
}

// Funktio käy koko rakenteen läpi ja tulostaa siitä löytyvät tuote-hintaparit.
// Mukana kuljetetaan virheparametria, joka pitää kirjaa siitä, onko haluttu
// kauppaketju ja haluttu myymälä löytynyt. Jos haluttuja tietoja ei löydy,
// funktio tulostaa itse virheilmoituksen ja palaa pääohjelmaan.

void komento_selection (const std::string& ketju, const std::string& myymala,
                        const Ketjumap& ketjut, int virheparametri = 2){

    // Käytetään apuvektoria, johon tallennetaan halutut tulosteet.
    // Näin tulosteiden aakkostaminen on helpompaa.

    std::vector < std::string > tulostettavat = {};

    for (Ketjumap::const_iterator it = ketjut.begin(); it != ketjut.end(); ++it){

        if (it->first == ketju){
            virheparametri = 1;

            for (Liikemap::const_iterator it2 = it->second.begin();
                 it2 != it->second.end(); ++it2){

                if (it2->first == myymala){
                    virheparametri = 0;

                for (Tuote tuotetieto : it2->second){

                    if (tuotetieto.hinta == TUOTE_LOPPU){
                        tulostettavat.push_back(tuotetieto.tuotenimi
                                                + " " + "out of stock");
                    }
                    else{

                        // Double-tyyppinen hinta muutetaan merkkijonoksi
                        // tulostusta varten, hyödynnetään stringstreamia
                        // desimaalien muotoiluun.

                        std::stringstream stream;
                        stream << std::fixed << std::setprecision(2)
                               << tuotetieto.hinta;

                        std::string hinta_string = stream.str();
                        tulostettavat.push_back(tuotetieto.tuotenimi
                                                + " " + hinta_string);
                    }
                }
                }
            }
        }
    }

    if (virheparametri == 2){ // Ketjua ei löytynyt
        std::cout << "Error: unknown chain" << std::endl;
    }
    else if (virheparametri == 1){ // Myymälää ei löytynyt
        std::cout << "Error: unknown store" << std::endl;
    }
    else{ // Molemmat löytyivät, tulostetaan
        std::sort(tulostettavat.begin(), tulostettavat.end());

        for (std::string s : tulostettavat){
            std::cout << s << std::endl;
        }
    }
}

/* Funktio etsii kaikkien ketjujen kaikista myymälöistä halutun tuotteen ja
vertaa sen hintaa muihin vastaaviin tuotteisiin. Halvimman tuotteen hinta,
ja kaikki ne myymälät, joissa tuote on sen hintainen, tallennetaan apuvectoriin,
joka tulostetaan haun loputtua. Funktio toimii oikein, kun kaikki mahdolliset
 hinnat ovat pienempiä kuin TUOTE_LOPPU-vakio. Jos on vaarana, että
hinnat nousevat samalle tasolle, TUOTE_LOPPU-vakiota tulee kasvattaa.
Lisäksi löytyi-totuusarvo tarkistaa, löytyikö tuotetta ylipäätään.
Funktio tulostaa lopuksi halvimman hinnan ja sijainnit tai vaihtoehtoisesti
virheilmoitukset. Tämän jälkeen palataan pääfunktioon. */

void komento_cheapest (const std::string& tuote, const Ketjumap& ketjut){

    bool loytyi = false;
    double halvin = TUOTE_LOPPU;
    std::vector < std::string > halvimmat = {};
    std::string tmp = "";

    for (Ketjumap::const_iterator it = ketjut.begin(); it != ketjut.end(); ++it){

        for (Liikemap::const_iterator it2 = it->second.begin();
             it2 != it->second.end(); ++it2){

            for (Tuote tuotetieto : it2->second){

            // Listaan lisätään alkiota vain jos tuotenimi on oikea
            if (tuotetieto.tuotenimi == tuote){
                loytyi = true;

                // Sama hinta --> lisätään myymälän nimi listaan
                if (tuotetieto.hinta == halvin){
                    tmp = it->first + " " + it2->first;
                    halvimmat.push_back(tmp);
                }
                // Halvempi hinta --> listaan pelkästään tämä myymälä
                else if (tuotetieto.hinta < halvin){
                    halvin = tuotetieto.hinta;
                    halvimmat.clear();
                    tmp = it->first + " " + it2->first;
                    halvimmat.push_back(tmp);
                }
            }
            }
        }
    }

    if (not loytyi){
        std::cout << "Product is not part of product selection." << std::endl;
    }
    // Jos halvin hinta on TUOTE_LOPPU, se on loppu kaikkialta
    else if (halvin == TUOTE_LOPPU){
        std::cout << "The product is temporarily out of stock everywhere."
                  << std::endl;
    }
    else{
        std::cout << std::fixed << std::setprecision(2) << halvin << " euros"
                  << std::endl;

        for (std::string s: halvimmat){
            std::cout << s << std::endl;
        }
    }
}

// Funktio etsii rakenteesta kaikki mahdolliset tuotteet ja tallentaa ne
// apuvectoriin tuotteet. Lopuksi vector tulostetaan ja palataan pääfunktioon.
void komento_products (const Ketjumap& ketjut){

    std::vector < std::string > tuotteet = {};

    for (Ketjumap::const_iterator it = ketjut.begin(); it != ketjut.end(); ++it){
        for (Liikemap::const_iterator it2 = it->second.begin();
             it2 != it->second.end(); ++it2){

            for (Tuote tuotetiedot : it2->second){

                std::vector < std::string >::iterator etsi =
                        find(tuotteet.begin(), tuotteet.end(),
                             tuotetiedot.tuotenimi);

                // find-algoritmi palauttaa vectorin end-alkion jos ei löytynyt
                if (etsi == tuotteet.end()){
                    tuotteet.push_back(tuotetiedot.tuotenimi);
                }
            }
        }
    }
    std::sort(tuotteet.begin(), tuotteet.end()); // Tuotteet aakkosjärjestykseen

    for (std::string s : tuotteet){
        std::cout << s << std::endl;
    }
}
/* Funktio tarkistaa, että syötetiedoston rivit täyttävät seuraavat ehdot:
 - Kaikki rivit sisältävät neljä osaa
 - Yksikään rivi ei ole tyhjä
 - Yksikään rivi ei sisällä pelkkiä välilyöntejä
 - Rivin viimeinen osa on desimaaliluku */

bool tarkista_rivit (std::vector < std::string > osat){

    if(osat.size() != 4){
        return false;
    }

    // Jos neljännessä osassa on jotain muuta kuin out-of-stock tai
    // desimaaliluku, se ei kelpaa
    if (osat[3] != "out-of-stock"){
        try{
            std::stod(osat[3]);
        }
        catch(std::invalid_argument){
            return false;
        }
    }

    // Tarkistetaan että jokaisessa osassa on välilyönnistä poikkeava merkki
    std::string::iterator it;
    for (std::string s : osat){
        for (it = s.begin(); it != s.end(); ++it){
            if (*it != ' '){
                break;
            }
        }
        if (it == s.end()){
            return false;
        }
    }
    return true;
}

/* Funktio lukee käyttäjän haluaman tekstitiedoston ja tarkistaa sen sisällön.
Jos tiedostoa ei löydy tai se on virheellinen tulostetaan virheilmoitus ja
palautetaan false, jolloin pääfunktio lopettaa ohjelman suorituksen.
Tiedostosta luetut rivit tallennetaan Rakenne-tietorakenteeseen, joka
mahdollistaa tallennetun tiedon käsittelyn.*/
bool muodosta_tietorakenne (Ketjumap& ketjut){

    std::string tiedostonimi;
    std::cout << "Input file: ";
    std::getline(std::cin, tiedostonimi);
    std::ifstream tiedosto(tiedostonimi);
    // Jos tiedoston luku ei onnistu, palautetaan false ja pääfunktio
    // lopettaa ohjelman suorituksen
    if (not tiedosto){
        std::cout << "Error: the input file cannot be opened" << std::endl;
        return false;
    }

    double uusi_hinta = 0;
    std::string rivi = "";
    std::vector < std::string > osat = {};

    while (getline(tiedosto, rivi)){

        osat = split(rivi, ';');

        if (not tarkista_rivit(osat)){
            std::cout << "Error: the file has an erroneous line" << std::endl;
            return false;
        }
        // Tallennetaan tuotteen hinta doublena tai double-vakiona TUOTE_LOPPU
        // jos tuotetta ei ole
        if(osat[3] == "out-of-stock"){
            uusi_hinta = TUOTE_LOPPU;
        }
        else{
            uusi_hinta = std::stod(osat[3]);
        }
        // Haetaan apufunktiolta tieto siitä, missä kohdassa Tuotevectoria
        // tuote sijaitsee. Uusi hinta päivitetään tuotteelle.
        // Jos tuotetta ei vielä ole, apufunktio palauttaa -1,
        // jolloin luodaan uusi tuote
        int indeksi = onko_tuote_olemassa(ketjut[osat[0]][osat[1]], osat[2]);

        if (indeksi == -1){
            Tuote uusi_tuote = {osat[2], uusi_hinta};
            ketjut[osat[0]][osat[1]].push_back(uusi_tuote);
        }
        else{
            ketjut[osat[0]][osat[1]][indeksi].hinta = uusi_hinta;
        }
    }
    return true;
}
/* Pääfunktio huolehtii käyttöliittymästä ja käyttäjän syötteiden vastaanotosta
(pl. tiedostonimi jonka virhetarkastuksen toteuttaa muodosta_tietorakenne)
Jos syötteissä on syntaksivirheitä, pääfunktio tulostaa itse virheilmoituksen.
Muussa tapauksessa kutsutaan oikea funktio käsittelemään tietorakennetta.
Pääfunktio tulostaa siis itse vain virheilmoituksia. Pääfunktio myös huolehtii
ohjeman lopettamisesta tiedostonlukuun liittyvän virheen tapauksessa tai kun
Ohjelma suoritetaan onnistuneesti. */
int main()
{

    Ketjumap ketjut;

    if (not muodosta_tietorakenne(ketjut)){
        return EXIT_FAILURE;
    }

    std::string syote = "";
    std::vector < std::string > osat = {};

    // Virheellinen syöte ei lopeta ohjelman suoritusta. Ohjelman
    // suoritus päättyy vasta, kun käyttäjä syöttää "quit"
    while (true){

        std::cout << "> ";
        std::getline(std::cin, syote);
        osat = split(syote, ' ');

        if (osat.size() == 1 and osat[0] == "quit"){
            break;
        }

        else if(osat[0] == "chains"){
            if (osat.size() != 1){
                std::cout << "Error: error in command chains" << std::endl;
            }
            else{
                komento_chains(ketjut);
            }
        }

        else if(osat[0] == "products"){
            if (osat.size() != 1){
                std::cout << "Error: error in command products" << std::endl;
            }
            else{
                komento_products(ketjut);
            }
        }

        else if(osat[0] == "stores"){
            if (osat.size() != 2){
                std::cout << "Error: error in command stores" << std::endl;
            }
            else{
                komento_stores(osat[1], ketjut);
            }
        }

        else if (osat [0] == "selection"){
            if (osat.size() != 3){
                std::cout << "Error: error in command selection" << std::endl;
            }
            else{
                komento_selection(osat[1], osat[2], ketjut);
            }
        }

        else if (osat[0] == "cheapest"){
            if (osat.size() != 2){
                std::cout << "Error: error in command cheapest" << std::endl;
            }
            else{
                komento_cheapest(osat[1], ketjut);
            }
        }
        else{
            std::cout << "Error: unknown command" << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
