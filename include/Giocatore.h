#ifndef GIOCATORE_H
#define GIOCATORE_H

#include <random>
#include <algorithm>
#include "Casella_Terreno.h"
#include "Casella_Stazione.h"
#include "Casella_Societa.h"

class Casella_Terreno;

class Giocatore
{
public:
    // Costruttore
    Giocatore(int);
    Giocatore(){};

    Giocatore &operator=(Giocatore *);

    // Scelte del giocatore
    virtual bool choice(std::string*) { return false; }; // Il giocatore sceglie se comprare (return true) o no (return false)
    virtual bool partecipaAsta(int*, Casella*, bool, int) { return false; }; // Il giocatore sceglie se comprare (return true) o no (return false)
    virtual bool wantToBuild (Casella_Terreno*) { return true; };

    // Funzioni membro
    void buy(); // Effettua l'acquisto della casella su cui il giocatore si trova
    void buy(int, Casella*); // Effettua l'acquisto della casella passata con il prezzo passato (usato nelle aste)
    void pushInToBuild(Casella_Terreno* A);

    // Funzioni di movimento
    void move(int);
    void moveBack(int n);
    void goTo(Casella*);
    void moveTo(int);
    void goTo(std::string);   // Riceve un nome di una casella e sposta il giocatore in quella casella
    
    // Funzioni GET
    int getMoney() const { return _money; }
    Casella *getPosition() const { return _pos; }
    int getID() const { return _ID; }
    int getTurniJail() const { return _turniJail; }
    int getNTurno() const { return _nturno; }
    bool isInGame() const { return _isInGame; }
    bool isInJail() const { return _isInJail; }
    bool hasFreeExitPrisonProb() {return FreeExitPrisonProb;}
    bool hasFreeExitPrisonImpr() {return FreeExitPrisonImpr;}
    std::string getToBuild();

    // Funzioni SET
    void setJail(bool t)  { _isInJail = t;  if(!t) _turniJail=1; }
    void setPos(Casella*);
    void setFreeExitPrisonProb(bool a) { FreeExitPrisonProb = a; }
    void setFreeExitPrisonImpr(bool a) { FreeExitPrisonImpr = a; }
    void incrementTurniJail() { _turniJail++; }
    void incrementNTurno() { _nturno++; }
    void resetNTurno() { _nturno = 1; }

    // Funzioni di modifica soldi di un giocatore
    virtual void Transaction(int, Giocatore *, std::string *) {}; // Se non si hanno abbastanza soldi il giocatore ipoteca fin che può e infine, quando non potra più pagare, viene eliminato e lancio eccezione
    void pay(int);                         // Se non si hanno abbastanza soldi il giocatore non può pagare e lancio eccezione
    void deposit(int);                     // chiamato quando un altro giocatore cade su un mio terreno e mi deve pagare o quando passo dal via

    // Eccezioni
    class Not_Enough_Money{};
    class Player_Lost{};

    // Variabili oggetto di giocatore
    std::vector<Casella_Terreno *> _elenco_proprieta;
    std::vector<Casella_Terreno *> _elenco_proprieta_to_build;  // Proprietà dove il giocatore può costruire case/alberghi
    std::vector<Casella_Stazione *> _elenco_proprieta_st;
    std::vector<Casella_Societa *> _elenco_proprieta_soc;
protected:
    int _turniJail;
    int _nturno;
    int _money;
    int _ID;        // Variabile che identifica il giocatore (tipo un codice fiscale, ogni giocatore ne avrà uno unico)
    bool _isInGame; // = true (nel costruttore), utile per capire se il giocatore è ancora in gioco
    bool _isInJail;
    Casella *_pos;  // La Casella su cui si trova il giocatore
    bool FreeExitPrisonProb;
    bool FreeExitPrisonImpr;
};

std::ostream &operator<<(std::ostream &, Giocatore);

// Predicati per la funzione di ordinamento
bool confrontaElementi(const Casella_Terreno*, const Casella_Terreno*);
bool confrontaElementiStatus(const Casella_Terreno*, const Casella_Terreno*);

#endif // GIOCATORE_H