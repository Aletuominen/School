#include "deck.hh"
#include "opendeck.cc"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QPainter>
#include <QLabel>

Deck::Deck(QWidget *parent) : QFrame(parent)
{
    setMinimumSize(180, 260);
    setMaximumSize(180, 260);
    setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    for (int suit = SPADE; suit != SUIT_COUNT; ++suit){

        for (unsigned value = 1; value <= 13; ++value){
            auto card = new Card(static_cast<CardSuit>(suit), value, this);
            int xOffset = (this->width() - card->width()) / 2;
            int yOffset = (this->height() - card->height()) / 2;
            card->move(xOffset, yOffset);
            cards_.push_back(card);
        }
    }

    // Sekoitetaan pakka valitsemalla satunnaisesti yksi kortti pakan alku-
    // päästä ja yksi kortti pakan loppupäästä ja vaihtamalla näiden paikat.
    // Toistetaan 100 kertaa (riittävän huolellinen sekoitus)

    srand (time(NULL));

    for (unsigned int sekoit = 0; sekoit < 100; ++sekoit){

        int slot1 = rand() % 26;
        int slot2 = rand() % 26 + 26;

        Card* card1 = cards_[slot1];
        Card* card2 = cards_[slot2];

        cards_[slot1] = card2;
        cards_[slot2] = card1;
    }


}

Card* Deck::pickCard()
{
    if (cards_.empty()){
        return nullptr;
    } else {
        Card* card = cards_.back();
        cards_.pop_back();

        return card;
    }
}

void Deck::addCard(Card *card)
{
    if (card == nullptr){
        return;
    }
    cards_.push_back(card);
}

void Deck::returnCards()
{
    cards_ = usedCards_;

    for (Card* card : cards_){

        card->setParent(this);
        card->turn();
        card->show();
    }
}

// Suoritetaan, kun pakkaa klikataan.
void Deck::mousePressEvent(QMouseEvent *event)
{
    // Ohjelma ei saa kaatua tyhjän pakan klikkaukseen.
    if (cards_.empty()){
        emit deckIsEmpty();
        returnCards();
    }

    // Otetaan osoittimen päähän pakan päällimmäinen kortti ja poistetaan se pakasta.
    Card* card = cards_.back();
    cards_.pop_back();
    // Ohjelma ei saa kaatua, jos korttia ei olekaan.
    if (!card){
        return;
    }

    // Ilmoitetaan, että pakasta otettiin kortti.
    emit cardPicked(card);
    usedCards_.push_back(card);

    // Hyväksytään klikkaus, koska se käsitellään aina tässä metodissa.
    event->accept();
}
