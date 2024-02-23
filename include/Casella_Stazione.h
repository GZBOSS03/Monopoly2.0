#ifndef CASELLA_STAZIONE_H
#define CASELLA_STAZIONE_H

#include "Casella.h"

class Giocatore;
class Casella_Stazione : public Casella
{
public:
    // Costruttore
    Casella_Stazione(std::string A, std::string B) : Casella(A,B)
    {
        _proprietario = nullptr;
        _prezzo_terreno = 200;
        _rendita1St = 25;
    }

    // Funzioni Membro
    int getAffitto() const { return _rendita1St; }
    int getPrezzo() const  { return _prezzo_terreno; }
    void buy(Giocatore* g) { if (_proprietario == nullptr) _proprietario = g; }
    void reset() { _proprietario = nullptr; }
    Giocatore* getProprietario() const { return _proprietario; }

    // Variabili membro
    Giocatore* _proprietario;  // Utile per capire se posso acquistarla o per sapere a chi devo pagare.
    int _prezzo_terreno;       // Prezzo di acquisto
    int _rendita1St;           // Rendita con 1 stazione
};

#endif // CASELLA_STAZIONE_H