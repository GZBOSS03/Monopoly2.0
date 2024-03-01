#ifndef CASELLA_SOCIETA_H
#define CASELLA_SOCIETA_H

#include "Casella.h"

class Giocatore;
class Casella_Societa : public Casella
{
public:
    // Costruttore
    Casella_Societa(std::string A, std::string B) : Casella(A,B)
    {
        _proprietario = nullptr;
        _prezzo_terreno = 150;
    }

    // Funzioni Membro
    int getPrezzo()  { return _prezzo_terreno; }
    void buy(Giocatore* g) { if (_proprietario == nullptr) _proprietario = g; }
    void reset() { _proprietario = nullptr; }
    Giocatore* getProprietario() const { return _proprietario; }

    // Variabili membro
    Giocatore* _proprietario;  // Utile per capire se posso acquistarla o per sapere a chi devo pagare.
    int _prezzo_terreno;       // Prezzo di acquisto
};

#endif // CASELLA_SOCIETA_H