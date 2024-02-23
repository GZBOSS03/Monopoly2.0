#ifndef CASELLA_PROB_IMPR_H
#define CASELLA_PROB_IMPR_H

#include "Casella.h"
#include <random>

class Casella_Prob_Impr : public Casella
{
public:
    // Costruttore
    Casella_Prob_Impr(std::string A, std::string B) : Casella(A,B) {};
};

#endif // CASELLA_PROB_IMPR_H