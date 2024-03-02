#ifndef CASELLA_TERRENO_H
#define CASELLA_TERRENO_H
   
#include "Casella.h"
#include "Giocatore.h"

// Definizione dei colori
#define RED     "\033[38;5;196m"
#define GREEN   "\033[38;5;46m"
#define YELLOW  "\033[38;5;226m"
#define CYAN    "\033[38;5;51m"
#define PINK    "\033[38;5;213m"
#define DARK_BROWN "\033[38;5;94m"
#define ORANGE_RED "\033[38;5;9m\033[38;5;202m"
#define PURPLE "\033[38;5;55m"

class Giocatore;
class Casella_Terreno : public Casella
{
public:
    // Costruttore
    Casella_Terreno(std::string, std::string);

    // Funzioni Membro
    void buy(Giocatore* g); // compro casella (set proprietario), se ho casella compro casa, se ho 4 case compro albergo
    
    // Funzioni GET
    int getAffitto() const; // ritorna il prezzo da pagare di pernottamento (in casa o in albergo)
    int getPrezzo() const;  // restituisce il prezzo per aquistare terreno/casa/albergo
    int getPrezzoTerreno() const { return _prezzo_terreno;}
    Giocatore* getProprietario() const { return _proprietario; }
    char getFamily () const { return _family; }
    bool isCasa1() const { return _casa1; }
    bool isCasa2() const { return _casa2; }
    bool isCasa3() const { return _casa3; }
    bool isCasa4() const { return _casa4; }
    bool isAlbergo() const { return _albergo; }
    bool canBuy() const { return _canBuy; }
    int getStatus() const;  // Restituisco un numero che mi indica a che punto sono i lavori di costruzione

    int getNFamily() const { return _nForFamily; }
    std::string getColor() const;
    
    // Funzioni SET
    void changeCanBuy() { _canBuy = !_canBuy;       if (!_casa1) _canBuy = true; }
    void setCanBuy(bool t) { _canBuy = t; }
    void build();
    void ipoteca();
    void reset();           // reset cella (per quando un giocatore perde)

private:
    char _family;
    int  _nForFamily;
    bool _canBuy;              // possibilità di acquistare case/alberghi (se il giocatore ha tutte le proprietà di questo colore)
    bool _casa1;               // true se c'è 1 casa
    bool _casa2;               // true se ci sono 2 case
    bool _casa3;               // true se ci sono 3 case
    bool _casa4;               // true se ci sono 4 case
    bool _albergo;             // true se c'è un albergo
    Giocatore* _proprietario;  // Utile per capire se posso acquistarla o per sapere a chi devo pagare.
    // Prezzi di acquisto
    int _prezzo_terreno;
    int _prezzo_casa;
    int _prezzo_albergo;
    // Rendita da terreno posseduto
    int _rendita_solo_terreno;
    int _rendita_1casa;
    int _rendita_2case;
    int _rendita_3case;
    int _rendita_4case;
    int _rendita_albergo;
};

std::ostream &operator<<(std::ostream &os, Casella_Terreno A);

#endif // CASELLA_TERRENO_H