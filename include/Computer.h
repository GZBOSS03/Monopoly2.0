#ifndef COMPUTER_H
#define COMPUTER_H

#include "Giocatore.h"
#include <thread>

class Computer : public Giocatore
{
public:

    Computer(int ID) : Giocatore(ID){}

    Giocatore& operator=(Computer* g);

    bool choice(std::string*) override;
    bool partecipaAsta(int*, Casella*, bool, int) override;
    void Transaction(int, Giocatore *, std::string * = nullptr) override;
};

#endif  // COMPUTER_H