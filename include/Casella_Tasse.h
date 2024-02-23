#ifndef CASELLA_TASSE_H
#define CASELLA_TASSE_H

#include "Casella.h"

class Casella_Tasse : public Casella
{
public:
    // Costruttore
    Casella_Tasse(std::string A, std::string B) : Casella(A,B) {};

    // Funzioni Membro
    int getTax() const // ritorna la tassa da pagare
    {
        if (_name == "Tassa Patrimoniale")
            return 200;
        if (_name == "Tassa Di Lusso")
            return 100;
        return 0; // non lo fa
    }

};

#endif // CASELLA_TASSE_H