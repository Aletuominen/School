#ifndef DECK_HH
#define DECK_HH

#include <vector>
#include <iostream>
#include <QFrame>
#include "card.hh"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

class Deck : public QFrame
{
    Q_OBJECT

public:
    Deck(QWidget *parent = 0);

    // Korttipakasta voi nostaa päällimmäisen kortin.
    Card* pickCard();
    void addCard(Card* card);
    void returnCards();

signals:
    // Signaali, jonka pakka lähettää, kun kortti nostetaan.
    void cardPicked(Card* card);
    void deckIsEmpty();

protected:
    // Itse toteutettu Qt:n widgetin klikkaamiseen liittyvä metodi.
    void mousePressEvent(QMouseEvent *event);

private:
    // Korttipakan sisältämät kortit.
    std::vector<Card*> cards_;
    std::vector<Card*> usedCards_;
};

#endif // DECK_HH
