#include <QFrame>
#include <QHBoxLayout>

#include "mainwindow.hh"
#include "cardslot.hh"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setupLayout();
    connect(deck_, &Deck::cardPicked, pickedCards_, &OpenDeck::addCard);
    connect(deck_, &Deck::deckIsEmpty, pickedCards_, &OpenDeck::emptyDeck);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupLayout()
{
    // Pääikkunan isoimpana rakenteena on frame, joka sisältää...
    QFrame* frame = new QFrame;
    QVBoxLayout* frameLayout = new QVBoxLayout(frame);

    // ... alarivin ja ylärivin.
    QHBoxLayout* bottomRowLayout = new QHBoxLayout();
    QHBoxLayout* topRowLayout = new QHBoxLayout();
    frameLayout->addLayout(topRowLayout);
    frameLayout->addLayout(bottomRowLayout);

    // Luodaan pakkaoliot.
    deck_ = new Deck(this);
    pickedCards_ = new OpenDeck(this);

    // Lisätään yläriville suljettu ja avoin pakka...
    topRowLayout->addWidget(deck_);
    topRowLayout->addWidget(pickedCards_);

    topRowLayout->addWidget(new CardSlot(&GameRules::checkIfHeart, this));
    topRowLayout->addWidget(new CardSlot(&GameRules::checkIfSpade, this));
    topRowLayout->addWidget(new CardSlot(&GameRules::checkIfDiamond, this));
    topRowLayout->addWidget(new CardSlot(&GameRules::checkIfClub, this));


    // ... ja alariville yksi cardslot.
    CardSlot* valmisSlotti1 = (new CardSlot(&GameRules::checkIfSame, this));
    CardSlot* valmisSlotti2 = (new CardSlot(&GameRules::checkIfSame, this));
    bottomRowLayout->addWidget(valmisSlotti1);
    bottomRowLayout->addWidget(valmisSlotti2);
    valmisSlotti1->addCard(deck_->pickCard());
    valmisSlotti2->addCard(deck_->pickCard());

    bottomRowLayout->addWidget(new CardSlot(&GameRules::checkIfHeart, this, false));
    bottomRowLayout->addWidget(new CardSlot(&GameRules::checkIfHeart, this, false));
    bottomRowLayout->addWidget(new CardSlot(&GameRules::checkIfHeart, this, false));


    setCentralWidget(frame);
}
