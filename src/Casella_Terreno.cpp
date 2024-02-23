#include "../include/Casella_Terreno.h"

Casella_Terreno::Casella_Terreno(std::string N, std::string NB) : Casella(N, NB)
{
    // Il contenuto e la coordinata li faccio settare alla superclasse casella
    reset(); // Per settare _albergo, _casa e _proprietario
    _nForFamily = 3;

    if (_name == "Vicolo Corto")
    {
        _nForFamily = 2;
        _family = 'M';  // Marrone
        _prezzo_terreno = 60;
        _prezzo_casa = 50;
        _prezzo_albergo = 50;

        _rendita_solo_terreno = 2;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = _rendita_solo_terreno*15;
        _rendita_3case = _rendita_solo_terreno*45;
        _rendita_4case = _rendita_solo_terreno*80;
        _rendita_albergo = _rendita_solo_terreno*125;
    }
    if (_name == "Vicolo Stretto")
    {
        _nForFamily = 2;
        _family = 'M';  // Marrone
        _prezzo_terreno = 60;
        _prezzo_casa = 50;
        _prezzo_albergo = 50;

        _rendita_solo_terreno = 4;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = _rendita_solo_terreno*15;
        _rendita_3case = _rendita_solo_terreno*45;
        _rendita_4case = _rendita_solo_terreno*80;
        _rendita_albergo = 450;
    }

    if ((_name == "Bastioni Gran Sasso") || (_name == "Viale Monterosa"))
    {
        _family = 'A';  // Azzurro (Cyan)
        _prezzo_terreno = 100;
        _prezzo_casa = 50;
        _prezzo_albergo = 50;

        _rendita_solo_terreno = 6;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = _rendita_solo_terreno*15;
        _rendita_3case = _rendita_solo_terreno*45;
        _rendita_4case = 500;
        _rendita_albergo = 550;
    }
    if (_name == "Viale Vesuvio")
    {
        _family = 'A';  // Azzurro (Cyan)
        _prezzo_terreno = 120;
        _prezzo_casa = 50;
        _prezzo_albergo = 50;

        _rendita_solo_terreno = 8;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = 100;
        _rendita_3case = 300;
        _rendita_4case = 450;
        _rendita_albergo = 600;
    }

    if ((_name == "Via Accademia") || (_name == "Corso Ateneo"))
    {
        _family = 'P';  // Pink (Rosa)
        _prezzo_terreno = 140;
        _prezzo_casa = 100;
        _prezzo_albergo = 100;

        _rendita_solo_terreno = 10;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = _rendita_solo_terreno*15;
        _rendita_3case = _rendita_solo_terreno*45;
        _rendita_4case = 625;
        _rendita_albergo = 750;
    }
    if (_name == "Piazza Universita'")
    {
        _family = 'P';  // Pink (Rosa)
        _prezzo_terreno = 160;
        _prezzo_casa = 100;
        _prezzo_albergo = 100;

        _rendita_solo_terreno = 12;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = _rendita_solo_terreno*15;
        _rendita_3case = 500;
        _rendita_4case = 700;
        _rendita_albergo = 900;
    }
    
    if ((_name == "Via Verdi") || (_name == "Corso Raffaello"))
    {
        _family = 'O';  // Orange (Arancione/Marrone scuro)
        _prezzo_terreno = 180;
        _prezzo_casa = 100;
        _prezzo_albergo = 100;

        _rendita_solo_terreno = 14;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = 200;
        _rendita_3case = 550;
        _rendita_4case = 750;
        _rendita_albergo = 950;
    }
    if (_name == "Piazza Dante")
    {
        _family = 'O';  // Orange (Arancione/Marrone scuro)
        _prezzo_terreno = 200;
        _prezzo_casa = 100;
        _prezzo_albergo = 100;

        _rendita_solo_terreno = 16;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = 220;
        _rendita_3case = 600;
        _rendita_4case = 800;
        _rendita_albergo = 1000;
    }

    if ((_name == "Via Marco Polo") || (_name ==  "Corso Magellano"))
    {
        _family = 'R';  // Red (Rosso)
        _prezzo_terreno = 220;
        _prezzo_casa = 150;
        _prezzo_albergo = 150;

        _rendita_solo_terreno = 18;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = 250;
        _rendita_3case = 700;
        _rendita_4case = 875;
        _rendita_albergo = 1050;
    }
    if (_name == "Largo Colombo")
    {
        _family = 'R';  // Red (Rosso)
        _prezzo_terreno = 240;
        _prezzo_casa = 150;
        _prezzo_albergo = 150;

        _rendita_solo_terreno = 20;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = _rendita_1casa*3;
        _rendita_3case = 750;
        _rendita_4case = 925;
        _rendita_albergo = 1100;
    }

    if ((_name == "Viale Traiano") || (_name == "Viale Costantino"))
    {
        _family = 'Y';  // Yellow (Giallo)
        _prezzo_terreno = 260;
        _prezzo_casa = 150;
        _prezzo_albergo = 150;

        _rendita_solo_terreno = 22;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = _rendita_1casa*3;
        _rendita_3case = 800;
        _rendita_4case = 975;
        _rendita_albergo = 1150;
    }
    if (_name == "Piazza Giulio Cesare")
    {
        _family = 'Y';  // Yellow (Giallo)
        _prezzo_terreno = 280;
        _prezzo_casa = 150;
        _prezzo_albergo = 150;

        _rendita_solo_terreno = 24;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = _rendita_1casa*3;
        _rendita_3case = 850;
        _rendita_4case = 1025;
        _rendita_albergo = 1200;
    }
    
    if ((_name == "Via Roma") || (_name == "Corso Impero"))
    {
        _family = 'G';  // Green (Verde)
        _prezzo_terreno = 300;
        _prezzo_casa = 200;
        _prezzo_albergo = 200;

        _rendita_solo_terreno = 26;
        _rendita_1casa = _rendita_solo_terreno*5;
        _rendita_2case = _rendita_1casa*3;
        _rendita_3case = 900;
        _rendita_4case = 1100;
        _rendita_albergo = 1275;
    }
    if (_name == "Largo Augusto")
    {
        _family = 'G';  // Green (Verde)
        _prezzo_terreno = 320;
        _prezzo_casa = 200;
        _prezzo_albergo = 200;

        _rendita_solo_terreno = 28;
        _rendita_1casa = 150;
        _rendita_2case = _rendita_1casa*3;
        _rendita_3case = 1000;
        _rendita_4case = 1200;
        _rendita_albergo = 1400;
    }
    if (_name == "Viale Dei Giardini")
    {
        _nForFamily = 2;
        _family = 'B';  // Blu
        _prezzo_terreno = 350;
        _prezzo_casa = 200;
        _prezzo_albergo = 200;

        _rendita_solo_terreno = 35;
        _rendita_1casa = 175;
        _rendita_2case = 500;
        _rendita_3case = 1100;
        _rendita_4case = 1300;
        _rendita_albergo = 1500;
    }
    if (_name == "Parco Della Vittoria")
    {
        _nForFamily = 2;
        _family = 'B';  // Blu
        _prezzo_terreno = 400;
        _prezzo_casa = 200;
        _prezzo_albergo = 200;

        _rendita_solo_terreno = 50;
        _rendita_1casa = 200;
        _rendita_2case = 600;
        _rendita_3case = 1400;
        _rendita_4case = 1700;
        _rendita_albergo = 2000;
    }
}

void Casella_Terreno::buy(Giocatore *g)
{
    if (_proprietario == nullptr)      // Se la casella non è di nessuno, si procede all'acquisto del terreno
       _proprietario = g;
    else
    {// Tentativo di acquistare una casa/albergo su tutte le proprietà della famiglia
    char target = _family;
        for (int i=0; i< g->_elenco_proprieta.size(); i++)
        {
            if (g->_elenco_proprieta[i]->getFamily() == target) // Se il terreno è della stessa famiglia della proprietà su cui è il giocatore
            {
                g->_elenco_proprieta[i]->build();
            }
        }
    }
}

void Casella_Terreno::build()
{
    if (!_albergo && _casa4)
    {     
        _albergo = true;
    }
    if (!_casa4 && _casa3)
    {
        _casa4 = true;
    }
    if (!_casa3 && _casa2)
    {
        _casa3 = true;
    }
    if (!_casa2 && _casa1)
    {
        _casa2 = true;
    }
    if (!_casa1 &&_canBuy)
    {
        _casa1 = true;
    }
}

void Casella_Terreno::ipoteca()
{
    if (_albergo)
    {     
        _albergo = false;
    }
    else if (_casa4)
    {
        _casa4 = false;
    }
    else if (_casa3)
    {
        _casa3 = false;
    }
    else if (_casa2)
    {
        _casa2 = false;
    }
    else if (_casa1)
    {
        _casa1 = false;
    }
}

int Casella_Terreno::getAffitto() const
{
    if (_albergo)
        return _rendita_albergo;
    if (_casa4)
        return _rendita_4case;
    if (_casa3)
        return _rendita_3case;
    if (_casa2)
        return _rendita_2case;
    if (_casa1)
        return _rendita_1casa;
    if (_canBuy)    // Se il proprietario ha tutte le caselle di quel tipo, ma non ci ha ancora costruito nulla
        return _rendita_solo_terreno*2;
    // else
    return _rendita_solo_terreno;
}

int Casella_Terreno::getPrezzo() const
{
    if (_casa4 && _canBuy)
        return _prezzo_albergo*_nForFamily;
    if (!_casa4 && _canBuy)
        return _prezzo_casa*_nForFamily;
    // if (_proprietario == nullptr)
    return _prezzo_terreno;
}

std::string Casella_Terreno::getColor() const
{
    if (_family == 'M')
    return "marrone";
    if (_family == 'A')
    return "azzurro";
    if (_family == 'P')
    return "rosa";
    if (_family == 'O')
    return "arancione";
    if (_family == 'R')
    return "rosso";
    if (_family == 'Y')
    return "giallo";
    if (_family == 'G')
    return "verde";
    if (_family == 'B')
    return "viola";

    return "";  //non lo fa mai
}

void Casella_Terreno::reset()
{
    _canBuy = false;
    _casa1 = false;
    _casa2 = false;
    _casa3 = false;
    _casa4 = false;
    _albergo = false;
    _proprietario = nullptr;
}

std::ostream &operator<<(std::ostream &os, Casella_Terreno A)
{
// Capisco quanti spazi mettere davanti/dietro a seconda di quante case/alberghi/giocatori devo stampare
    std::string pre = "", post = "";
    int x = 7 + A._players.size(); // Spazi extra da stampare ai lati
    if (A.isAlbergo())
        x++;
    else
    {
        if(A.isCasa1())
            x++;
        if(A.isCasa2())
            x++;
        if(A.isCasa3())
            x++;
        if(A.isCasa4())
            x++;
    }
    int j=0;
    for (int i = 0; i < (Variabili::dimMaxCasella - x)/2; i++)
    {
        pre += " ";
        post += " ";
        j++;
    }
    x = x + j*2;
    if (x == Variabili::dimMaxCasella-1)
        pre += " ";

// Inizia la stampa (x==dimMaxCasella)
    os << pre;
    os << "| ";

// Decisione del colore della casella
    if (A.getFamily() == 'M')
        os << DARK_BROWN;
    if (A.getFamily() == 'A')
        os << CYAN;
    if (A.getFamily() == 'P')
        os << (PINK);
    if (A.getFamily() == 'O')
        os << ORANGE_RED;
    if (A.getFamily() == 'R')
        os << RED;
    if (A.getFamily() == 'Y')
        os << YELLOW;
    if (A.getFamily() == 'G')
        os << GREEN;
    if (A.getFamily() == 'B')
        os << PURPLE;

    // Stampa del nome
    os << A.getNameOutput() << RESET;

    // Se c'è una casa o un'albergo lo salvo in s
    if (A.isAlbergo())
        os << "^";
    else
    {
        if(A.isCasa1())
            os << "*";
        if(A.isCasa2())
            os << "*";
        if(A.isCasa3())
            os << "*";
        if(A.isCasa4())
            os << "*";
    }
    // Salva in s eventuali giocatori
    for (int i = 0; i < A._players.size(); i++)
    {
        os << A._players.at(i);
    }

    os << " |";
    os << post;
    
    return os;
}