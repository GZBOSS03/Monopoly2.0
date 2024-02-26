#ifndef CASELLA_H
#define CASELLA_H

#include <iostream>
#include <iomanip>  // Per std::setw
#include <sstream>  // Per std::ostringstream
#include <vector>
#include <windows.h>
#include "Variabili.h"

// Definizione dei colori
#define RESET   "\033[0m"
#define BLUE    "\033[38;5;21m"
#define ORANGE "\033[38;5;214m"

// Questa è una casella generica e rappresenta le caselle agli angoli
class Casella
{
public:
    // Costruttore
    Casella(std::string, std::string);

    // Funzioni membro
    void addPlayer(int);      // Agginge un giocatore nella casella (push_back())
    void removePlayer(int);             // Rimuve un giocatore dalla casella (erase())
    Casella* find(Casella*);
    Casella* find(std::string);

    // Utile per la stampa
    virtual std::string to_String() { return "";}

    // Funzioni GET
    std::string getNameOutput() const { return _easyname; }
    std::string getName() const {return _name;}
    Casella* getSucc() const { return _succ; }
    Casella* getPrec() const { return _prec; }

    // Funzioni SET
    void setSucc(Casella *next) { _succ = next; }
    void setPrec(Casella *p) { _prec = p; }

    // Variabili oggetto
    std::vector<int> _players; // Rappresenta i giocatori che sono sopra alla casella
protected:
    Casella* _succ;
    Casella* _prec; // precedente
    std::string _easyname;  // Come viene raffigurata in output, acronimo di cosa contiene la casella
    std::string _name;
};

std::ostream &operator<<(std::ostream &os, Casella A);

#endif // CASELLA_H