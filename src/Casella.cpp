#include "../include/Casella.h"

Casella::Casella(std::string N, std::string Nbreve)
{
    _succ = nullptr;
    _prec = nullptr;
    _name = N;
    _easyname = Nbreve;
}

Casella* Casella::find(Casella* target)
{
    // Provo a cercare "andando avanti" rispetto alla Casella next
    // Questo avviene finchè non ritorno da dove sono partito (controllo nella condizione while)
    Casella* next = this;
    do
    {
        // Controllo che il nodo su cui viene chiamato è quello che cerco
        if (next->getNameOutput() == target->getNameOutput())
            return this;   // Trovato
        next = next->_succ;
    } while (next->getNameOutput() != this->getNameOutput());

    // Se non lo ho trovato significa che non c'è (non dovrebbe succedere), quindi restituisco nullptr
    return nullptr;
}

Casella* Casella::find(std::string s)
{
    // Provo a cercare "andando avanti" rispetto alla Casella next
    // Questo avviene finchè non ritorno da dove sono partito (controllo nella condizione while)
    Casella* next = this;
    do
    {
        // Controllo che il nodo su cui viene chiamato è quello che cerco
        if (next->getNameOutput() == s)
            return this;   // Trovato
        next = next->_succ;
    } while (next->getNameOutput() != this->getNameOutput());

    // Se non lo ho trovato significa che non c'è (non dovrebbe succedere), quindi restituisco nullptr
    return nullptr;
}

void Casella::addPlayer(int g)
{
    _players.push_back(g);
}

void Casella::removePlayer(int a)
{
    for (int i=0; i < _players.size(); i++)
    {
        if (_players[i] == a)
            _players.erase(_players.begin()+i);
    }
}

std::ostream &operator<<(std::ostream &os, Casella A)
{
    // Capisco quanti spazi mettere davanti/dietro a seconda di quante case/alberghi/giocatori devo stampare
    std::string pre = "", post = "";
    int x = 7 + A._players.size()*2; // Spazi extra da stampare ai lati
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

// Inizia la stampa 
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);    
    os << pre;
    if (A._players.size() == 6)
        os << "|";  // Senza spazio
    else
        os << "| ";
    if (A.getNameOutput().substr(0,2) == "St")
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | 0);
    if (A.getNameOutput().substr(0,2) == "So")
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
    if (A.getNameOutput().substr(0,2) == "?P")
        os << BLUE;
    if (A.getNameOutput().substr(0,2) == "?I")
        os << ORANGE;
    // Stampa del nome
    os << A.getNameOutput() << RESET;

    for (int i = 0; i < A._players.size(); i++)
    {
        // Utilizza una std::ostringstream per formattare il testo
        std::ostringstream playerFormatted;
        playerFormatted << "\033[4m" << A._players.at(i) << "\033[0m";
        os << std::setw(10) << playerFormatted.str();
    }

    if (A._players.size() == 6)
        os << "|";  // Senza spazio
    else
        os << " |";
    os << post;
    
    return os;
}
