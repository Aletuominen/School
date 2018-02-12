#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include "ruutu.hh"
#include <algorithm>
#include <list>

// Ruutu-luokan olioina toimivat miinaharavalaudan ruudut. Jokainen ruutu sisältää
// tietoa siitä, onko ruudussa miinoja, onko ruudussa lippuja ja montako miinaa
// viereisissä ruuduissa on ym.
Ruutu::Ruutu(int x, int y, bool onkoMiinaa, Lauta * lauta)
{
    x_ = x;
    y_ = y;
    onkoMiinaa_ = onkoMiinaa;
    lauta_ = lauta; // tallennetaan viite pääfunktion kaksiulotteiseen ruutu-vectoriin,
                    // jotta ruutu voi tarvittaessa tarkistaa viereisten ruutujen sisällön
}

// Metodi lisää viiteparametriin "virta" halutun tulostusmerkin sen mukaan, mitä
// ruutu sisältää. If-else-rakenteen järjestys määrittää tulostusmerkkien
// arvojärjestyksen. Esimerkiksi avaamattoman ruudun ei kuulu tulostaa miinaa,
// vaikka se sisältäisikin sellaisen.
void Ruutu::tulosta(std::ostream& virta){

    if (onkoLippu_){
        virta << "P";
        return;
    }
    else if (not onkoAuki_){
            virta << ".";
            return;
    }
    else if (onkoMiinaa_){
        virta << "*";
        return;
    }
    else{
        virta << viereiset_;
        return;
    }
}

// Metodi laskee jokaisen ruudun vieressä olevien miinojen määrän ja tallentaa sen luokan omaan
// vieeiset_ -muuttujaan
void Ruutu::laskeViereiset(){

    for (int y = std::max(0, y_ - 1); y <= std::min(KOKO - 1, y_ + 1); y++){
        for (int x = std::max(0, x_ - 1); x <= std::min(KOKO - 1, x_ + 1); x++){

            // Käsitellään kaikki ruudut 3x3 alueella ruudun ympärillä, minmax estää
            // out_of_bounds-ongelmat ruudukon reunoilla

            if (lauta_->at(y).at(x).onkoMiinaa_){
                viereiset_ += 1;
            }
        }
    }
}

// Metodi kertoo pääohjelmalle, täyttääkö yksittäinen ruutu voittokriteerit (true = kyllä). Peli on voitettu,
// kun kaikki miinat on liputettu eikä lippuja ole käytetty turhaan. Kaikkia ruutuja ei siis
// tarvitse avata.
bool Ruutu::onkoValmis()
{
    if (onkoMiinaa_ and not onkoLippu_){
        return false;
    }
    else if(onkoLippu_ and not onkoMiinaa_){
        return false;
    }
    return true;
}
// Gettereitä ja settereitä ei ole sen kummemmin kommentoitu, toiminnallisuus on aika itsestäänselvää
bool Ruutu::onkoLippu()
{
    if (onkoLippu_){
        return true;
    }
    return false;
}

void Ruutu::poistaLippu()
{
    onkoLippu_ = false;
}

void Ruutu::lisaaLippu()
{
    onkoLippu_ = true;
}

// Metodi avaa pyydetyn ruudun, ja tekee tarvittavat jatkotoimenpiteet
bool Ruutu::avaa()

{
    onkoAuki_ = true;

    if (onkoMiinaa_){ // Jos pelaaja avaa miinan, palautetaan tieto siitä pääohjelmalle, jotta peli
        return false; // saadaan lopetettua
    }

    else if (not viereiset_ == 0){ // Jos ruudun vieressä on yksi tai useampi miina, ei tarvitse avata
        return true;               // muita ruutuja
    }

    std::pair < int, int > koord; // pairin käyttö ei tarpeellista, kun alkiot ovat samaa tietotyyppiä,
                                  // mutta harjoitellaan samalla senkin käyttöä

    std::list < std::pair < int, int > > avatut; // valitaan list eikä vector, koska listaa halutaan muokata
                                                 // kesken iteraation. Vectoria iteroitaessa alkioiden lisääminen
                                                 // invalidoi iteraattorin

    koord.first = x_;
    koord.second = y_;
    avatut.push_back(koord);
    int uusia_avattu = 1; // alustetaan millä tahansa nollasta poikkeavalla arvolla

    while (true){

        // Tarkistetaan uuden avatun ruudun viereiset (ei kulmittaiset) ruudut. Jos löytyy nollaruutu,
        // se avataan ja siirretään avattaviin. Samalla pidetään kirjaa siitä, montako uutta ruutua
        // tällä iteraatiolla on avattu. Jos uusia avattavia ei löydy, suoritus lopetetaan.

        // Tällä tavalla toimimalla tulee hieman turhaa laskemista koska joka iteraatiolla lasketaan
        // myös vanhat ruudut uudestaan. Ohjelmaa voisi keventää lisäämällä toteutukseen toisen
        // listan niin, että yksi pitäisi kirjaa jo avatuista ja yksi kirjaa avausvuorossa olevista
        // ruuduista. Toisaalta tällöin jouduttaisiin iteroimaan molemmat listat läpi joka kerta.

        // Selkeyden puolesta tämä toteutus on toimivampi, on silti varmasti olemassa kevyempikin tapa
        // toteuttaa tämä

        if (uusia_avattu == 0){
            break;
        }
        uusia_avattu = 0;

        for (std::pair < int, int > pari : avatut){

            int x = pari.first;
            int y = pari.second;

            if(x == KOKO - 1){
                ;
            }
            else{

                // Käydään vuorotellen läpi kaikki neljä ruudun viereistä ruutua, ja tarkistetaan, ovatko nekin
                // nollaruutuja. Tässä osiossa jonkin verran toisteista koodia, mutten keksinyt järkevämpääkään
                // tapaa toteuttaa.

                if (lauta_->at(y).at(x + 1).viereiset_ == 0 and not lauta_->at(y).at(x + 1).onkoAuki_){

                    lauta_->at(y).at(x + 1).onkoAuki_ = true;
                    koord.first = x + 1;
                    koord.second = y;
                    avatut.push_back(koord);
                    uusia_avattu += 1;

                }
            }

            if(x == 0){
                ; // Jos x on 0, sen vasemmanpuolista ei luonnollisesti ole olemassa. Samantyyliset tarkastelut
                  // tehdään jokaisen suunnan kanssa
            }
            else{
                if (lauta_->at(y).at(x - 1).viereiset_ == 0 and not lauta_->at(y).at(x - 1).onkoAuki_){

                    lauta_->at(y).at(x - 1).onkoAuki_ = true;
                    koord.first = x - 1;
                    koord.second = y;
                    avatut.push_back(koord);
                    uusia_avattu += 1;
                }
            }

            if(y == KOKO - 1){
                ;
            }
            else{
                if (lauta_->at(y + 1).at(x).viereiset_ == 0 and not lauta_->at(y + 1).at(x).onkoAuki_){

                    lauta_->at(y + 1).at(x).onkoAuki_ = true;
                    koord.first = x;
                    koord.second = y + 1;
                    avatut.push_back(koord);
                    uusia_avattu += 1;
                }
            }

            if(y == 0){
                ;
            }
            else{
                if (lauta_->at(y - 1).at(x).viereiset_ == 0 and not lauta_->at(y - 1).at(x).onkoAuki_){

                    lauta_->at(y - 1).at(x).onkoAuki_ = true;
                    koord.first = x;
                    koord.second = y - 1;
                    avatut.push_back(koord);
                    uusia_avattu += 1;
                }
            }
        }
    }
        // Jokaisen nollaruudun ympäröivät ruudut avataan. Miinoja ei tarvitse huomioida kohtalaisen
        // ilmiselvästä syystä

        // Nelinkertaisen silmukan tilalle olisi myös voinut valita toisen toteutustavan, mutta tämäkin
        // toimii

        for (int y = 0; y < KOKO; y++){
            for (int x = 0; x < KOKO; x++){

                if (lauta_->at(y).at(x).viereiset_ == 0 and lauta_->at(y).at(x).onkoAuki_){

                    for (int y2 = std::max(0 , y - 1); y2 <= std::min(KOKO - 1, y + 1);y2++){
                        for(int x2 = std::max(0, x - 1); x2 <= std::min(KOKO - 1, x + 1);x2++){
                            lauta_->at(y2).at(x2).onkoAuki_ = true;
                        }
                    }
                }
            }
        }
    return true;
}

