
// Ohjelma on miinaharavapeli, jossa tarkoituksena on löytää laudalta jokainen miina ja liputtaa se.
// Miinat tulee löytää ilman, että miinaruutua itseään avataan. Kun pelaaja avaa miinattoman ruudun,
// siihen ilmestyy numero, joka kertoo, montako miinaa sitä ympäröivistä ruuduista löytyy. Näiden
// tietojen pohjalta on (yleensä) mahdollista päätellä, missä miinat sijaitsevat. Peli voitetaan,
// kun pelaaja on liputtanut kaikki miinat ilman että yhtäkään lippua on käytetty turhaan. Jos pelaaja
// avaa miinaruudun, peli hävitään. Pelissä ei siis tarvitse avata kaikkia miinattomia ruutuja kuten
// perinteisessä miinaharavassa. Käyttäjän apuna pelissä on ASCII-grafiikalla toteutettu pelilauta, joka
// tulostuu jokaisen ruudun avauksen jälkeen.

// Käyttöliittymänä toimii yksinkertainen syöterivi, joka ottaa vastaan kokonaisluku- ja tekstisyötteitä
// Peli kysyy ensin pelaajalta seedin, jonka perusteella miinojen sijainti arvotaan. Tämän jälkeen
// pelaaja avaa laudalta yksitellen ruutuja syöttämällä kaksi kokonaislukua välilyönnillä erotettuna.
// Pelaaja voi halutessaan liputtaa ruudun tai poistaa ruudussa olevan lipun syöttämällä K/E sitä kysyttäessä
// Käyttäjän syötteille ei erikseen tehdä tarkistuksia, vaan käyttäjän tulee itse huolehtia siitä, että syötteet
// ovat järkevää tietotyyppiä, eikä esimerkiksi laudan ulkopuolisia ruutuja yritetä avata.

// Rakenteellisesti ohjelma on toteutettu niiin, että kaikki toiminnallisuus ja pelaajan kanssa kommunikointi
// tapahtuu main-moduulissa. Ruutu-moduuli taas sisältää kaiken yksittäiseen ruutuun liittyvän tiedon. Mainiin
// on rakennettu kaksiulotteinen matriisi, johon kaikki Ruutu-oliot tallennetaan.

// Tekijä: Aleksi Tuominen


#ifndef RUUTU_HH
#define RUUTU_HH
#include <vector>

const int KOKO = 6;

class Ruutu;

using Lauta = std::vector < std::vector < Ruutu > >;

class Ruutu
{
public:
    Ruutu(int x, int y, bool onkoMiinaa, Lauta * lauta);
    void tulosta(std::ostream &virta);
    void laskeViereiset();
    bool onkoValmis();
    bool onkoLippu();
    void poistaLippu();
    void lisaaLippu();
    bool avaa();

private:
    bool onkoMiinaa_;
    bool onkoLippu_ = false;
    bool onkoAuki_ = false;
    int x_;
    int y_;
    int viereiset_ = 0;
    Lauta* lauta_; // Viite kaksiulotteiseen vektoriin, joka sisältää kaikkien ruutujen oliot
                   // tarpeellinen, kun ruutu haluaa tietää, mitä muista ruuduista löytyy
};

#endif // RUUTU_HH
