#include "Casella_Terreno.h"
#include "Casella_Tasse.h"
#include "Casella_Stazione.h"
#include "Casella_Prob_Impr.h"
#include "Casella_Societa.h"

class Tabellone
{
public:
    // Costruttore
    Tabellone(); // Assegna di default le variabili predefinite
    
    // Variabili oggetto del tabellone
    // Puntatori a caselle particolari
    Casella* partenza;
    Casella* prison;
    Casella* parcheggio;
    Casella* alt;
    const int dim_x = 11; // Numero di colonne
    const int dim_y = 11; // Numero di righe: A=1, B=2, ... , H = 8, conversione con Index_riga
    const int tot_caselle = 40;
};

std::ostream &operator<<(std::ostream &os, Tabellone A);