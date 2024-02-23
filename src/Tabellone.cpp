#include "../include/Tabellone.h"

Tabellone::Tabellone()
{
    std::string name_Casella = "Via";
    std::string easyname = "VIA";
    // Creo e riempio di dati la prima casella
    partenza = new Casella{name_Casella, easyname};

    Casella *Casella_Now = partenza;

    // Creo le altre caselle
    for (int i = 1; i < tot_caselle; i++) // i=1 perchè ho già sistemato la prima casella
    {
        // Creo il puntatore ad una nuova casella (quella che sto per creare)
        Casella *New_Casella;

        switch (i)
        {
            case 1:
            name_Casella = "Vicolo Corto";
            easyname = "Vco";
            break;

            case 2:
            name_Casella = "Probabilita'";
            easyname = "?P?";
            break;

            case 3:
            name_Casella = "Vicolo Stretto";
            easyname = "VSt";
            break;

            case 4:
            name_Casella = "Tassa Patrimoniale";
            easyname = "TPa";
            break;

            case 5:
            name_Casella = "Stazione Sud";
            easyname = "StS";
            break;
        
            case 6:
            name_Casella = "Bastioni Gran Sasso";
            easyname = "BGS";
            break;

            case 7:
            name_Casella = "Imprevisti";
            easyname = "?I?";
            break;
        
            case 8:
            name_Casella = "Viale Monterosa";
            easyname = "VMo";
            break;
        
            case 9:
            name_Casella = "Viale Vesuvio";
            easyname = "VVe";
            break;
        
            case 10:
            name_Casella = "Transito";
            easyname = "T/P";
            break;
        
            case 11:
            name_Casella = "Via Accademia";
            easyname = "VAc";
            break;

            case 12:
            name_Casella = "Societa' elettrica";
            easyname = "SoE";
            break;

            case 13:
            name_Casella = "Corso Ateneo";
            easyname = "CAt";
            break;

            case 14:
            name_Casella = "Piazza Universita'";
            easyname = "PUn";
            break;

            case 15:
            name_Casella = "Stazione Ovest";
            easyname = "StO";
            break;

            case 16:
            name_Casella = "Via Verdi";
            easyname = "VVe";
            break;

            case 17:
            name_Casella = "Probabilita'";
            easyname = "?P?";
            break;

            case 18:
            name_Casella = "Corso Raffaello";
            easyname = "Cra";
            break;

            case 19:
            name_Casella = "Piazza Dante";
            easyname = "PDa";
            break;

            case 20:
            name_Casella = "Parcheggio";
            easyname = "Par";
            break;        

            case 21:
            name_Casella = "Via Marco Polo";
            easyname = "VMP";
            break;

            case 22:
            name_Casella = "Imprevisti";
            easyname = "?I?";
            break;

            case 23:
            name_Casella = "Corso Magellano";
            easyname = "CMa";
            break;

            case 24:
            name_Casella = "Largo Colombo";
            easyname = "LCo";
            break;

            case 25:
            name_Casella = "Stazione Nord";
            easyname = "StN";
            break;

            case 26:
            name_Casella = "Viale Costantino";
            easyname = "VCo";
            break;

            case 27:
            name_Casella = "Viale Traiano";
            easyname = "VTr";
            break;

            case 28:
            name_Casella = "Societa' Acqua Potabile";
            easyname = "SoA";
            break;

            case 29:
            name_Casella = "Piazza Giulio Cesare";
            easyname = "PGC";
            break;

            case 30:
            name_Casella = "Vai in prigione";
            easyname = "ALT";
            break;

            case 31:
            name_Casella = "Via Roma";
            easyname = "VRo";
            break;

            case 32:
            name_Casella = "Corso Impero";
            easyname = "CIm";
            break;

            case 33:
            name_Casella = "Probabilita'";
            easyname = "?P?";
            break;

            case 34:
            name_Casella = "Largo Augusto";
            easyname = "LAu";
            break;

            case 35:
            name_Casella = "Stazione Est";
            easyname = "StE";
            break;

            case 36:
            name_Casella = "Imprevisti";
            easyname = "?I?";
            break;

            case 37:
            name_Casella = "Viale Dei Giardini";
            easyname = "VdG";
            break;

            case 38:
            name_Casella = "Tassa Di Lusso";
            easyname = "TdL";
            break;

            case 39:
            name_Casella = "Parco Della Vittoria";
            easyname = "PdV";
            break;

        }
        
        if ((i == dim_x - 1) || (i == dim_x + dim_y - 2) || (i == 2 * dim_x + dim_y - 3))
        {   // Mi trovo agli angoli (escluso P, già creato)
            // Creo una nuova casella angolare
            New_Casella = new Casella{name_Casella, easyname};

            if(easyname == "Par")
                parcheggio = New_Casella;
            if(easyname == "ALT")
                alt = New_Casella;
            if(easyname == "T/P")
                prison = New_Casella;
        }
        else
        {
            // Creo una nuova casella laterale
            if ((easyname == "TdL") || (easyname == "TPa"))          // Casella tasse
                New_Casella = new Casella_Tasse{name_Casella, easyname};
            else if (easyname[0] == '?')                             // Casella Probabilità/imprevisti
                New_Casella = new Casella_Prob_Impr{name_Casella, easyname};
            else if ((easyname[0] == 'S') && (easyname[1] == 't'))   // Casella Stazione
                New_Casella = new Casella_Stazione{name_Casella, easyname};
            else if ((easyname[0] == 'S') && (easyname[1] == 'o'))   // Casella Società
                New_Casella = new Casella_Societa{name_Casella, easyname};
            else                                                     // Casella terreno
                New_Casella = new Casella_Terreno{name_Casella, easyname};
        }

        // Aggiungo la casella appena creata al tabellone (Casella_Now è "l'ultima" casella del tabellone)
        Casella_Now->setSucc(New_Casella);
        // Ora setto il precedente (setPrec) della nuova casella (Casella_Now->getSucc) con la casella prima (Casella_Now)
        Casella_Now->getSucc()->setPrec(Casella_Now);
        // Mi sposto sulla casella appena aggiunta al tabellone (l'ultima creata)
        Casella_Now = New_Casella;

        // Chiusura della lista concatenata su sè stessa ("head" = "tail" = "tabellone")
        if (i == tot_caselle - 1)
        {
            Casella_Now->setSucc(partenza);
            partenza->setPrec(Casella_Now);
        }
    }
}

std::ostream &operator<<(std::ostream &os, Tabellone A)
{
    std::string space="";
    // Creo la stringa space in base alla dimensione di una Casella
    for (int i = 0; i < Variabili::dimMaxCasella; i++)
    {
        space += " ";
    }

    for (int i = 0; i < A.dim_y; i++)
    {
        for (int j = 0; j < A.dim_x; j++)
        {
            Casella* toPrint = nullptr;
            if ((i == 0) || (i == A.dim_y - 1)) // Se sono sulla 1a o sull'ultima riga del tabellone
            {
                if (i == 0)
                    toPrint = A.parcheggio;
                if (i == A.dim_y - 1)
                    toPrint = A.prison;
                // A seconda di che casella devo stampare (j), mi sposto su di essa
                for (int h = 0; h < j; h++)
                {
                    if (i == 0)
                        toPrint = toPrint->getSucc();
                    if (i == A.dim_y - 1)
                        toPrint = toPrint->getPrec();
                }
            }
            else if ((j == 0) || (j == A.dim_x - 1)) // Se sono sulla 1a o sull'ultima colonna del tabellone
            {
                if (j == 0)
                    toPrint = A.parcheggio;
                if (j == A.dim_y - 1)
                    toPrint = A.alt;
                // A seconda di che casella devo stampare (i), mi sposto su di essa
                for (int h = 0; h < i; h++)
                {
                    if (j == 0)
                        toPrint = toPrint->getPrec();
                    if (j == A.dim_y - 1)
                        toPrint = toPrint->getSucc();
                }
            }
            
            // Stampa casella
            // Controllo se è una casella terreno
            Casella_Terreno* p = dynamic_cast<Casella_Terreno*> (toPrint); 
            if (p)
                os << *p;
            else if (toPrint)
                os << *toPrint;
            else // Sono in mezzo, quindi devo stampare spazio vuoto
                os << space;
        }
        // Fine riga
        os << "\n";
    }
    
    return os;
}