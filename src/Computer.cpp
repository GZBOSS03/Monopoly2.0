#include "../include/Computer.h"

bool Computer::choice(std::string* p){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distr(1,100);

// Scelta se uscire di prigione
    if (_isInJail)
    {
        if (_money < Variabili::moneyUscitaPrigione)    // Se non ha soldi non può pagare, allora tira i dadi
        {
            std::cout << "Giocatore " << _ID << " non ha abbastanza soldi per uscire di prigione, quindi tira i dadi.\n";
            *p += "Giocatore " + std::to_string(_ID) + " non ha abbastanza soldi per uscire di prigione, quindi tira i dadi.\n";
            return false;
        }
        else                                            // se invece ha soldi allora fa la scelta
        {
            if(distr(gen) <= Variabili::probabilitaSiExitJail || _money > Variabili::initialMoney)
            {
                std::cout << "Giocatore " << _ID << " vuole uscire di prigione ora.\n";
                *p += "Giocatore " + std::to_string(_ID) + " vuole uscire di prigione ora.\n";
                return true;
            }
            else
            {
                std::cout << "Giocatore " << _ID << " vuole tentare la sorte con i dadi.\n";
                *p += "Giocatore " + std::to_string(_ID) + " vuole tentare la sorte con i dadi.\n";
                return false;
            }
        }
    }
    
// Scelta acquisto stazione (della banca)
    Casella_Stazione* st = dynamic_cast<Casella_Stazione*> (_pos);
    if(st)  // Se il computer si trova in una stazione e ne possiede almeno una
    {
        if (_money < st->getPrezzo())
        {
            *p += "Giocatore " + std::to_string(_ID) + " non ha abbastanza " + Variabili::getValuta() + " per l'acquisto di " + st->getName() + ".\n";
            return false;
        }
        else if (!_elenco_proprieta_st.empty() || _money > Variabili::initialMoney)
        {
            *p += "Giocatore " + std::to_string(_ID) + " vuole comprare " + _pos->getName() + ".\n";
            return true;
        }
    }

// Scelta acquisto società (della banca)
    Casella_Societa* so = dynamic_cast<Casella_Societa*> (_pos);
    if(so)  // Se il computer si trova in una stazione e ne possiede almeno una
    {
        if (_money < so->getPrezzo())
        {
            *p += "Giocatore " + std::to_string(_ID) + " non ha abbastanza " + Variabili::getValuta() + " per l'acquisto di " + so->getName() + ".\n";
            return false;
        }
        else if (!_elenco_proprieta_soc.empty() || _money > Variabili::initialMoney)
        {
            *p += "Giocatore " + std::to_string(_ID) + " vuole comprare " + _pos->getName() + ".\n";
            return true;
        }
    }

// Scelta acquisto terreno (della banca)
    Casella_Terreno* a = dynamic_cast<Casella_Terreno*> (_pos);
    if(a)  // Se il computer deve scegliere di acquistare un terreno
    {
        if (_money < a->getPrezzoTerreno())    // Se non può acquistarlo
        {
            *p += "Giocatore " + std::to_string(_ID) + " non ha abbastanza " + Variabili::getValuta() + " per l'acquisto di " + a->getName() + ".\n";
            return false;
        }
        // Cerca tra le sue proprietà se ce ne sono dello stesso colore/famiglia
        for (int i=0; i < _elenco_proprieta.size(); i++)
        {
            // Se il computer ne trova almeno una, allora compra il terreno
            if (_elenco_proprieta[i]->getFamily() == a->getFamily())
            {
                *p += "Giocatore " + std::to_string(_ID) + " vuole comprare " + _pos->getName() + ".\n";
                return true;
            }
        }
    }

    bool b = false;
    std::string toAdd = "Giocatore " + std::to_string(_ID);
    if (distr(gen) <= Variabili::probabilitaSiFirstAcquisto || _money > Variabili::initialMoney)
    {
        b=true;
        toAdd += " vuole comprare ";
        Casella_Terreno* posT = dynamic_cast<Casella_Terreno*> (_pos);
        toAdd += _pos->getName();
    }
    else
        toAdd += " non vuole comprare nulla";

    *p += toAdd +".\n";
    return b;
}

bool Computer::partecipaAsta(int* p, Casella* tobuy, bool lessMoneyPossible, int minimaOffertaAsta){
    int probabilitaSi = 30; // inizialmente il computer vuole rilanciare al 30%
    int k = 10; // k costante moltiplicativa dell'aggiunta a probabilitaSi
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> scelta(1,100);

    int prezzo;
    Casella_Terreno* a = dynamic_cast<Casella_Terreno*> (tobuy);
    if(a)  // Se il computer deve scegliere di acquistare un terreno
    {
        probabilitaSi += k*a->getPrezzoTerreno()/(*p);
        prezzo = a->getPrezzoTerreno();
        // Cerca tra le sue proprietà se ce ne sono dello stesso colore/famiglia
        int z = 0; // contatore
        for (int i=0; i < _elenco_proprieta.size(); i++)
        {
            if (_elenco_proprieta[i]->getFamily() == a->getFamily())
                z++;
        }
        if (z==2 || z==1) // Se gliene manca 1 o 2 e le completa, allora vuole comprarla assolutamente
            probabilitaSi = 100;
    }

    Casella_Stazione* st = dynamic_cast<Casella_Stazione*> (tobuy);
    if(st) // Se tobuy è una stazione
    {
        probabilitaSi += k*st->getPrezzo()/(*p);
        // Se il computer deve scegliere se acquistarlo per la prima volta o possiede altre stazioni allora vuole comprare assolutamente
        prezzo = st->getPrezzo();
        if (!_elenco_proprieta_st.empty() || lessMoneyPossible)
            probabilitaSi = 100;
    }

    Casella_Societa* so = dynamic_cast<Casella_Societa*> (tobuy);
    if(so)  // Se il computer possiede l'altra società e tobuy è una società, allora vuole comprare assolutamente
    {
        probabilitaSi += k*so->getPrezzo()/(*p);
        prezzo = so->getPrezzo();
        if (!_elenco_proprieta_soc.empty())
            probabilitaSi = 100;
    }

    if (_money > 4*Variabili::initialMoney/3) // Se il giocatore è "ricco", allora vuole acquistare
        probabilitaSi = 100;
    else if (*p > Variabili::maxMoneyAsta*prezzo) // Se il prezzo è salito di troppo
    {
        return false;
    }
    
    int rilancio = minimaOffertaAsta;
    if (_money < *p +  minimaOffertaAsta)
    {
        return false;
    }
    else
    {// Il giocatore decide se partecipare all'asta
        if (scelta(gen) <= probabilitaSi)
        {
            if (!lessMoneyPossible)  // Se una qualche condizione dal chiamante è falsa
            {
                if (_money - *p < 50)
                {
                    std::uniform_int_distribution<int> aumentoprezzopochimoney(minimaOffertaAsta,(_money - *p));
                    rilancio = aumentoprezzopochimoney(gen);
                }
                else
                {
                    std::uniform_int_distribution<int> aumentoprezzo(minimaOffertaAsta, minimaOffertaAsta+50);
                    rilancio = aumentoprezzo(gen);  // I computer rilanciano sempre al massimo di 50
                }
            }
            // altrimenti rilancio rimane al minimo sindacale
            *p = *p + rilancio;
            return true;
        }
        else
        {
            return false;
        }
    }
}

void Computer::Transaction(int n, Giocatore *Other, std::string *output){
    // trasferimento di soldi da un giocatore ad un altro (pagamento dell'affitto)
    try
    {
        this->pay(n);
        if (Other != nullptr)
            Other->deposit(n);
    }
    catch (const Giocatore::Not_Enough_Money &e)
    {
        int pausa = Variabili::pausa;  // Pausa del programma tra una stampa e un'altra
        // Vediamo se c'è qualcosa da ipotecare:
        std::cout << "Giocatore " << _ID << " non ha abbastanza " << Variabili::getValuta() << " per pagare e deve ipotecare qualcosa.\n";
        std::string init = "Giocatore " + std::to_string(_ID) + " non ha abbastanza " + Variabili::getValuta() + " per pagare e ipoteca:\n";
        // Ordinamento del vector utilizzando il predicato personalizzato
        std::string *toAdd = &init;
        std::this_thread::sleep_for(std::chrono::seconds(pausa));
        if (FreeExitPrisonImpr)
        {
            setFreeExitPrisonImpr(false);
            deposit(50);
            std::cout << "Venduta carta imprevisto uscita gratis di prigione alla banca, ricavati: 50 " << Variabili::getValuta() << ".\n";
            *toAdd += "- carta imprevisto uscita gratis di prigione (50)\n";
            std::this_thread::sleep_for(std::chrono::seconds(pausa));
            std::cout << "Giocatore " << _ID << " ora ha " << _money << " " << Variabili::getValuta();
            if (_money < n)
                std::cout << ", ma ancora non bastano.\n";
            else
                std::cout << ", abbastanza per pagare.\n";
            std::this_thread::sleep_for(std::chrono::seconds(pausa));
        }
        if (FreeExitPrisonProb)
        {
            setFreeExitPrisonProb(false);
            deposit(50);
            std::cout << "Venduta carta probabilita' uscita gratis di prigione alla banca, ricavati: 50 " << Variabili::getValuta() << ".\n";
            *toAdd += "- carta probabilita' uscita gratis di prigione (50)\n";
            std::this_thread::sleep_for(std::chrono::seconds(pausa));
            std::cout << "Giocatore " << _ID << " ora ha " << _money << " " << Variabili::getValuta();
            if (_money < n)
                std::cout << ", ma ancora non bastano.\n";
            else
                std::cout << ", abbastanza per pagare.\n";
            std::this_thread::sleep_for(std::chrono::seconds(pausa));
        }
        while(_money < n)   // Finché il giocatore non ha abbastanza soldi per pagare
        {
            // Il computer elimina prima le società e le stazioni ...
            if (_elenco_proprieta_soc.size() > 0)
            {
                for (int i=0; i < _elenco_proprieta_soc.size(); i++)
                {
                    if (_money < n)
                    {
                        std::cout << "Giocatore " << _ID << " ha ipotecato " << _elenco_proprieta_soc[i]->getName() << " e ha ricavato " <<  _elenco_proprieta_soc[i]->getPrezzo()/2 << " " << Variabili::getValuta() << ".\n";
                        *toAdd += "- " + _elenco_proprieta_soc[i]->getName() + " (" + std::to_string(_elenco_proprieta_soc[i]->getPrezzo()/2) + ")\n";
                        deposit(_elenco_proprieta_soc[i]->getPrezzo()/2);
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        std::cout << "Giocatore " << _ID << " ora ha " << _money << " " << Variabili::getValuta();
                        if (_money < n)
                            std::cout << ", ma ancora non bastano.\n";
                        else
                            std::cout << ", abbastanza per pagare.\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        _elenco_proprieta_soc[i]->reset();
                        _elenco_proprieta_soc.erase(_elenco_proprieta_soc.begin() + i);
                        i--;
                    }
                    else
                        i=50;   // Per uscire dal ciclo (sono apposto e non devo più ipotecare)
                }
            }
            if( _elenco_proprieta_st.size() > 0)
            {
                for (int i=0; i < _elenco_proprieta_st.size(); i++)
                {
                    if (_money < n)
                    {
                        std::cout << "Giocatore " << _ID << " ha ipotecato " << _elenco_proprieta_st[i]->getName() << " e ha ricavato " <<  _elenco_proprieta_st[i]->getPrezzo()/2 << " " << Variabili::getValuta() << ".\n";
                        *toAdd += "- " + _elenco_proprieta_st[i]->getName() + " (" + std::to_string(_elenco_proprieta_st[i]->getPrezzo()/2) + ")\n";
                        deposit(_elenco_proprieta_st[i]->getPrezzo()/2);
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        std::cout << "Giocatore " << _ID << " ora ha " << _money << " " << Variabili::getValuta();
                        if (_money < n)
                            std::cout << ", ma ancora non bastano.\n";
                        else
                            std::cout << ", abbastanza per pagare.\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        _elenco_proprieta_st[i]->reset();
                        _elenco_proprieta_st.erase(_elenco_proprieta_st.begin() + i);
                        i--;
                    }
                    else
                        i=50;   // Per uscire dal ciclo (sono apposto e non devo più ipotecare)
                }
            }
            // ... poi il computer elimina case/alberghi ...
            for (int i=0; i < _elenco_proprieta_to_build.size(); i++)
            {
                if (_money < n)
                {
                    if (_elenco_proprieta_to_build[i]->isCasa1())
                    {
                        if (i != _elenco_proprieta_to_build.size()-1)
                        {       
                            if (_elenco_proprieta_to_build[i]->getStatus() < _elenco_proprieta_to_build[i+1]->getStatus())
                                continue;
                        }
                        std::string s = " una casa in ";
                        if (_elenco_proprieta_to_build[i]->isAlbergo())
                            s = " l'albergo in ";
                        std::cout << "Giocatore " << _ID << " ha ipotecato" << s << _elenco_proprieta_to_build[i]->getName() << " e ha ricavato " << _elenco_proprieta_to_build[i]->getPrezzo()/2 << " " << Variabili::getValuta() << ".\n";
                        *toAdd += "-" + s + _elenco_proprieta_to_build[i]->getName() + " (" + std::to_string(_elenco_proprieta_to_build[i]->getPrezzo()/2) + ")\n";
                        deposit(_elenco_proprieta_to_build[i]->getPrezzo()/2);
                        _elenco_proprieta_to_build[i]->ipoteca();
                        _elenco_proprieta_to_build[i]->setCanBuy(true);
                        for (int j=0; j < _elenco_proprieta_to_build.size(); j++)
                        {
                            if (_elenco_proprieta_to_build[j]->getFamily() == _elenco_proprieta_to_build[i]->getFamily() && _elenco_proprieta_to_build[j]->getStatus() > _elenco_proprieta_to_build[i]->getStatus())
                                _elenco_proprieta_to_build[j]->setCanBuy(false);
                        }
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        std::cout << "Giocatore " << _ID << " ora ha " << _money << " " << Variabili::getValuta();
                        if (_money < n)
                        {
                            std::cout << ", ma ancora non bastano.\n";
                        }
                        else
                            std::cout << ", abbastanza per pagare.\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    }
                }
            }
            if (_elenco_proprieta.size() > 0)
            {
                std::sort(_elenco_proprieta.begin(), _elenco_proprieta.end(), confrontaElementiStatus);
                for (int i=0; i < _elenco_proprieta.size(); i++)
                {
                    if (_money < n)
                    {
                        bool terrenoIpotecabile = true;
                        for (int j=0; j<_elenco_proprieta_to_build.size(); j++)
                        {
                            // Se in _elenco_proprieta_to_build c'è almeno 1 proprietà dello stesso colore con 1 casa, allora non posso ipoteacare il terreno i
                            // (prima ipoteco tutte le case della famiglia e poi posso ipotecare i terreni)
                            if (_elenco_proprieta_to_build[j]->getFamily() == _elenco_proprieta[i]->getFamily() && _elenco_proprieta_to_build[j]->isCasa1())
                                terrenoIpotecabile = false;
                        }
                        if (terrenoIpotecabile)
                        {
                            std::cout << "Giocatore " << _ID << " ha ipotecato " << _elenco_proprieta[i]->getName() << " e ha ricavato " << _elenco_proprieta[i]->getPrezzoTerreno()/2 << " " << Variabili::getValuta() << ".\n";
                            *toAdd += "- " + _elenco_proprieta[i]->getName() + " (" + std::to_string(_elenco_proprieta[i]->getPrezzoTerreno()/2) + ")\n";
                            deposit(_elenco_proprieta[i]->getPrezzoTerreno()/2);
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            std::cout << "Giocatore " << _ID << " ora ha " << _money << " " << Variabili::getValuta();
                            if (_money < n)
                                std::cout << ", ma ancora non bastano.\n";
                            else
                                std::cout << ", abbastanza per pagare.\n";
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            for (int x=0; x < _elenco_proprieta_to_build.size(); x++)
                            {
                                if (_elenco_proprieta_to_build[x]->getFamily() == _elenco_proprieta[i]->getFamily())
                                {
                                    _elenco_proprieta_to_build[x]->setCanBuy(false);    // Per ciascun terreno della stessa famiglia di quello che ho appena ipotecato setto canBuy a false
                                    _elenco_proprieta_to_build.erase(_elenco_proprieta_to_build.begin() + x); // e li tolgo dal vettore _elenco_proprieta_to_build
                                    x--;
                                }
                            }
                            _elenco_proprieta[i]->reset();
                            _elenco_proprieta.erase(_elenco_proprieta.begin() + i);
                            i--;
                        }
                    }
                }
                std::sort(_elenco_proprieta.begin(), _elenco_proprieta.end(), confrontaElementi);
            }
            if(_elenco_proprieta.empty() && _elenco_proprieta_st.empty() && _elenco_proprieta_soc.empty() && _money<n)
            {
                // Se il giocatore non ha più nulla da ipotecare, paga tutti i soldi che ha ...
                std::cout << "Giocatore " + std::to_string(_ID) + " non ha piu nulla da ipotecare e non riesce a pagare.\n";
                *toAdd += "Giocatore " + std::to_string(_ID) + " non ha piu nulla da ipotecare e non riesce a pagare.\n";
                std::this_thread::sleep_for(std::chrono::seconds(Variabili::pausaLunga));
                std::string s = "Giocatore " + std::to_string(_ID) + " ha pagato tutti i suoi " + Variabili::getValuta() + ", " + std::to_string(_money);
                if (Other)
                {
                    s += ", al giocatore " + std::to_string(Other->getID()) + ".\n";
                    Other->deposit(_money);
                }
                else
                    s += ", alla banca.\n";
                this->pay(_money);
                std::cout << s;
                *output += *toAdd + s;

                // ... ed esce dal gioco
                _isInGame = false;
            
                throw Player_Lost();
            }
        }

        // Se non viene lanciata l'eccezione, allora esco dal ciclo con una quantità di soldi sufficiente da poter pagare (ovviamente avendo ipotecato qualcosa)
        std::this_thread::sleep_for(std::chrono::seconds(Variabili::pausaLunga));
        if (Other)
        {
            Other->deposit(n);
        }
        this->pay(n);
        *output += *toAdd;
    }
}

// Migliorabile con imlementazione di "IA" aggiuntiva
bool Computer::wantToBuild(Casella_Terreno* A){
    if (_money < 300) // Se il giocatore non ha abbastanza soldi per costruire
        return false;
    return true;
}

Giocatore& Computer::operator=(Computer* g){
    _pos= g->getPosition();
    _ID = g->getID();
    _isInGame = g->isInGame();
    _money = g->getMoney();
    // Aggiungo il giocatore al tabellone
    _pos->addPlayer(_ID);
    return *this;
}