#include "../include/Giocatore.h"

Giocatore::Giocatore(int n) : _ID{n}, _isInGame{true}, _money{Variabili::initialMoney}  
{
    _isInJail = false;
    _turniJail = 1;
    _nturno = 1;
    FreeExitPrisonImpr = false;
    FreeExitPrisonProb = false;
}

Giocatore &Giocatore::operator=(Giocatore *g){
    _pos = g->getPosition();
    _ID = g->getID();
    _isInGame = g->isInGame();
    _money = g->getMoney();
    // Aggiungo il giocatore al tabellone
    _pos->addPlayer(_ID);
    return *this;
}

void Giocatore::setPos(Casella* P){
    _pos = P;
    _pos->addPlayer(_ID);
}

void Giocatore::pushInToBuild(Casella_Terreno* A){
    if (A)
    {
        _elenco_proprieta_to_build.push_back(A);
        std::sort(_elenco_proprieta_to_build.begin(), _elenco_proprieta_to_build.end(), confrontaElementi);
    }
    else
        std::cout << "Puntatore non valido.\n";
}
    
void Giocatore::buy(){
    // Si effettua il cast della posizione
    Casella_Terreno *_pos1 = dynamic_cast<Casella_Terreno *>(_pos);
    if (_pos1)
    {
        pay(_pos1->getPrezzoTerreno());
        _pos1->buy(this);                                           // buy di Casella Laterale setta le sue cose
        _elenco_proprieta.push_back(_pos1);                         // aggiungo il terreno alla lista di proprietà
        // Ordinamento del vector utilizzando il predicato personalizzato
        std::sort(_elenco_proprieta.begin(), _elenco_proprieta.end(), confrontaElementi);
    }

    // Si effettua il cast della posizione
    Casella_Stazione *_pos2 = dynamic_cast<Casella_Stazione *>(_pos);
    if (_pos2)
    {
        pay(_pos2->getPrezzo());                     // lancia eccezione se il giocatore non ha abbastanza soldi
        _pos2->buy(this);                            // buy di Casella Stazione setta le sue cose
        _elenco_proprieta_st.push_back(_pos2);       // aggiungo la stazione alla lista di proprietà stazioni
    }

    // Si effettua il cast della posizione
    Casella_Societa *_pos3 = dynamic_cast<Casella_Societa *>(_pos);
    if (_pos3)
    {
        pay(_pos3->getPrezzo());                     // lancia eccezione se il giocatore non ha abbastanza soldi
        _pos3->buy(this);                            // buy di Casella Società setta le sue cose
        _elenco_proprieta_soc.push_back(_pos3);  // aggiungo la società alla lista di proprietà Società
    }
}

void Giocatore::buy(int prezzo, Casella* c){
    // Si effettua il cast della posizione
    Casella_Terreno *_pos1 = dynamic_cast<Casella_Terreno *>(c);
    if (_pos1)
    {
        pay(prezzo);
        _pos1->buy(this); // buy di Casella Laterale setta le sue cose
        if (!_pos1->isCasa1() && (_pos1->getProprietario() == this)) // Se è avvenuto l'acquisto del terreno (non c'è una casa e il proprietario è il giocatore)
        {
            _elenco_proprieta.push_back(_pos1);                     // aggiungo il terreno alla lista di proprietà
            // Ordinamento del vector utilizzando il predicato personalizzato
            std::sort(_elenco_proprieta.begin(), _elenco_proprieta.end(), confrontaElementi);
        }
    }

    // Si effettua il cast della posizione
    Casella_Stazione *_pos2 = dynamic_cast<Casella_Stazione *>(c);
    if (_pos2)
    {
        pay(prezzo);
        _pos2->buy(this); // buy di Casella Stazione setta le sue cose
        if (_pos2->getProprietario() == this)
            _elenco_proprieta_st.push_back(_pos2); // aggiungo la stazione alla lista di proprietà stazioni
    }

    // Si effettua il cast della posizione
    Casella_Societa *_pos3 = dynamic_cast<Casella_Societa *>(c);
    if (_pos3)
    {
        pay(prezzo);
        _pos3->buy(this);                                // buy di Casella Società setta le sue cose
        if (_pos3->getProprietario() == this)
            _elenco_proprieta_soc.push_back(_pos3);      // aggiungo la società alla lista di proprietà Società
    }
}

void Giocatore::pay(int n){
    // se il giocatore ha abbastanza soldi, allora paga, altrimenti si lancia l'eccezione e il giocatore perde
    if (n <= _money)
        _money = _money - n;
    else
        throw Not_Enough_Money();
}

void Giocatore::deposit(int n){
    // deposito n soldi sul conto del giocatore
    _money = _money + n;
}

void Giocatore::move(int n){
    // mi sposto di n posizioni in avanti rispetto alla posizione attuale (_pos)
    for (int i = 0; i < n; i++)
    {
        _pos = _pos->getSucc();
        if (_pos->getNameOutput() == "VIA") // Passaggio dal via che incrementa il budget del giocatore
            deposit(Variabili::moneyPassaggioVia);
    }
}

void Giocatore::moveBack(int n){
    // mi sposto di n posizioni in avanti rispetto alla posizione attuale (_pos)
    for (int i = 0; i < n; i++)
    {
        _pos = _pos->getPrec();
    }
    _pos->addPlayer(_ID);
}

void Giocatore::moveTo(int type)
{
    if (type == 1)  // Sto cercando la più vicina stazione
    {
        while (!dynamic_cast<Casella_Stazione*>(_pos)) // finche non mi trovo sulla casella in cui devo andare
        {
            _pos = _pos->getSucc();
            if (_pos->getNameOutput() == "VIA") // Passaggio dal via che incrementa il budget del giocatore
                deposit(Variabili::moneyPassaggioVia);
        };
        _pos->addPlayer(_ID);
    }
    
    if (type == 2)  // Sto cercando la più vicina società
    {
        while (!dynamic_cast<Casella_Societa*>(_pos)) // finche non mi trovo sulla casella in cui devo andare
        {
            _pos = _pos->getSucc();
            if (_pos->getNameOutput() == "VIA") // Passaggio dal via che incrementa il budget del giocatore
                deposit(Variabili::moneyPassaggioVia);
        };
        _pos->addPlayer(_ID);
    }
}

void Giocatore::goTo(Casella* A)
{
    if (A)  // Sto cercando la più vicina stazione
    {
        while (_pos->getName() != A->getName())
        {
            _pos = _pos->getSucc();
        };
    }
}

void Giocatore::goTo(std::string N)
{
    // Per essere sicuri di non lasciare mai il tabellone mi sposto sempre a partire dalla casella in cui mi trovo
    while (_pos->getName() != N) // finche non mi trovo sulla casella in cui devo andare
    {
        _pos = _pos->getSucc();
        if (_pos->getNameOutput() == "VIA") // Passaggio dal via che incrementa il budget del giocatore
            deposit(Variabili::moneyPassaggioVia);
    };
}

std::string Giocatore::getToBuild()
{
    std::string s = "";
    for (int i = 0; i < _elenco_proprieta_to_build.size(); i++)
    {
        s += " " + _elenco_proprieta_to_build[i]->getName();
    }
    return s;
}

// Predicati per la funzione di ordinamento
bool confrontaElementi(const Casella_Terreno* elem1, const Casella_Terreno* elem2) {
    return elem1->getPrezzoTerreno() <= elem2->getPrezzoTerreno();
}

bool confrontaElementiStatus(const Casella_Terreno* elem1, const Casella_Terreno* elem2) {
    return elem1->getStatus() < elem2->getStatus();
}

std::ostream &operator<<(std::ostream &os, Giocatore G)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
    std::string toAdd = "";
    if (G.hasFreeExitPrisonImpr())
        toAdd = toAdd + "*";
    if (G.hasFreeExitPrisonProb())
        toAdd = toAdd + "*";
    std::string s = "\nGiocatore " + std::to_string(G.getID()) + toAdd + ": ";
    os << "\nGiocatore " << G.getID() << toAdd << ": ";
    for (int i = 0; i < G._elenco_proprieta_soc.size(); i++)
    {
        std::ostringstream gg1;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        if (G._elenco_proprieta_soc.size() == 2)
            gg1 << "\033[4m" << G._elenco_proprieta_soc[i]->getName() << "\033[0m" << RESET;    // Sottolineato
        else
            gg1 << G._elenco_proprieta_soc[i]->getName() << RESET;
        os << std::setw(10) << gg1.str();
        s += G._elenco_proprieta_soc[i]->getName();
        if ((i+1 != G._elenco_proprieta_soc.size()) || (G._elenco_proprieta.size() > 0) || (G._elenco_proprieta_st.size() > 0))
        {
            os << ", ";
            s += ", ";
        }
    }
    for (int i = 0; i < G._elenco_proprieta_st.size(); i++)
    {
        std::ostringstream gg;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | 0);
        if (G._elenco_proprieta_st.size() == 4)
            gg << "\033[4m" << G._elenco_proprieta_st[i]->getName() << "\033[0m" << RESET;    // Sottolineato
        else
            gg << G._elenco_proprieta_st[i]->getName() << RESET;
        os << std::setw(10) << gg.str();
        s += G._elenco_proprieta_st[i]->getName();
        if ((i+1 != G._elenco_proprieta_st.size()) || (G._elenco_proprieta.size() > 0))
        {
            os << ", ";
            s += ", ";
        }
    }
    int h=1; // Contatore di andate a capo
    for (int i = 0; i < G._elenco_proprieta.size(); i++)
    {   
        // Se l'elenco è troppo lungo e devo scrivere ancora, vado a capo
        if ( s.length() + G._elenco_proprieta[i]->getName().size() > (Variabili::dimMaxOutput*h) )
        {
            os << "\n";
            h++;
        }

        // Decisione del colore della casella
        if (G._elenco_proprieta[i]->getFamily() == 'M')
            os << DARK_BROWN;
        if (G._elenco_proprieta[i]->getFamily() == 'A')
            os << CYAN;
        if (G._elenco_proprieta[i]->getFamily() == 'P')
            os << PINK;
        if (G._elenco_proprieta[i]->getFamily() == 'O')
            os << ORANGE_RED;
        if (G._elenco_proprieta[i]->getFamily() == 'R')
            os << RED;
        if (G._elenco_proprieta[i]->getFamily() == 'Y')
            os << YELLOW;
        if (G._elenco_proprieta[i]->getFamily() == 'G')
            os << GREEN;
        if (G._elenco_proprieta[i]->getFamily() == 'B')
            os << PURPLE;

        // Stampa del nome
        // Se ho tutte le caselle dello stesso colore le sottolineo in output (se ci posso costruire)
        // Utilizza una std::ostringstream per formattare il testo
        std::ostringstream playerFormatted;
        if (G._elenco_proprieta[i]->isAlbergo())
            playerFormatted << "\033[1m"  << "\033[4m" << G._elenco_proprieta[i]->getName() << "\033[0m" << RESET;    // Grassetto e sottolineato
        else if (G._elenco_proprieta[i]->canBuy())
            playerFormatted << "\033[4m" << G._elenco_proprieta[i]->getName() << "\033[0m" << RESET;    // Sottolineato
        else
            playerFormatted << G._elenco_proprieta[i]->getName() << RESET;
        os << std::setw(10) << playerFormatted.str();
        
        s += G._elenco_proprieta[i]->getName();

        if (i+1 != G._elenco_proprieta.size())
        {
            os << ", ";
            s += ", ";
        }
        else
        {
            os << ".";
            s += ".";
        }

    }

    return os;
}