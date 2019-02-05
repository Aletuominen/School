#ifndef CARDS_HH
#define CARDS_HH

#include <iostream>

class Cards {

    public:
      // Dynaamisella rakenteella on oltava rakentaja,
      // joka alustaa paallimmainen-osoittimen null:iksi.
      Cards();

      // Lisää uuden kortin annetulla id:llä pinon päällimmäiseksi.
      void add(int id);

      // Tulostaa tietorakenteen sisällön järjestysnumeroituna
      // parametrina annettuun tietovirtaan aloittaen ensimmäisestä alkiosta.
      void print_from_top_to_bottom(std::ostream& s);

      // Poistaa pinon päällimmäisen kortin ja palauttaa
      // sen viiteparametrissa luku. Paluuarvo true
      // jos onnistuu, false jos pino oli tyhjä.
      bool remove(int& id);

      // Siirrä tietorakenteen viimeinen alkio ensimmäiseksi.
      bool bottom_to_top();

      // Siirrä tietorakenteen ensimmäinen alkio viimeiseksi.
      bool top_to_bottom();

      // Tulostaa tietorakenteen sisällön järjestysnumeroituna
      // parametrina annettuun tietovirtaan aloittaen viimeisestä alkiosta.
      void print_from_bottom_to_top(std::ostream& s);

      // Dynaamisella tietorakenteella on oltava purkaja,
      // jota kutsumalla varattu muisti voidaan
      // vapauttaa, kun rakennetta ei enää tarvita.
      ~Cards();

    private:
      struct Card_data {
        int data;
        Card_data* down;
        Card_data* up;
      };

      int recursive_print(Card_data* top, std::ostream& s);  // Tätä voit käyttää apuna metodin printtaa_lopusta toteuttamisessa
      Card_data* top_;
};

#endif // CARDS_HH
