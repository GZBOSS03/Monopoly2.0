#ifndef HUMAN_H
#define HUMAN_H

#include "Giocatore.h"
#include <thread>

class Human : public Giocatore
{
public:
    // Costruttore che richiama quello della superclasse
    Human(int ID) : Giocatore(ID){}

    Giocatore& operator=(Human* g);

    bool choice(std::string*) override;
    bool partecipaAsta(int*, Casella*, bool, int) override;
    void Transaction(int, Giocatore *, std::string * = nullptr) override;
};

#endif  // HUMAN_H