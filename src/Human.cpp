#include "../include/Human.h"

bool Human::choice(std::string* p){

    Casella_Terreno *posNow = dynamic_cast<Casella_Terreno *>(_pos);
    Casella_Stazione *posNow1 = dynamic_cast<Casella_Stazione *>(_pos);
    Casella_Societa *posNow2 = dynamic_cast<Casella_Societa *>(_pos);

    if (posNow)
    {
        if (_money < posNow->getPrezzoTerreno())
        {
            std::cout << "Giocatore " << _ID << " non ha abbastanza " << Variabili::getValuta() << " per l'acquisto.\n";
            *p += "Giocatore " + std::to_string(_ID) + " non ha abbastanza " + Variabili::getValuta() + " per l'acquisto.\n";
            return false;
        }
        std::cout << "Giocatore " << _ID << " vuoi comprare " << posNow->getName() << " per " << posNow->getPrezzoTerreno() << "? (Inserire S per si, N per no)\n";
        *p += "Giocatore " + std::to_string(_ID) + " vuoi comprare " + posNow->getName() + " per " + std::to_string(posNow->getPrezzoTerreno()) + "? (Inserire S per si, N per no)\n";
    }
    
    if (posNow1)
    {
        if (_money < posNow1->getPrezzo())
        {
            std::cout << "Giocatore " << _ID << " non ha abbastanza " << Variabili::getValuta() << " per l'acquisto.\n";
            *p += "Giocatore " + std::to_string(_ID) + " non ha abbastanza " + Variabili::getValuta() + " per l'acquisto.\n";
            return false;
        }
        std::cout << "Giocatore " << _ID << " vuoi comprare " << posNow1->getName() << " per " << posNow1->getPrezzo() << "? (Inserire S per si, N per no)\n";
        *p += "Giocatore " + std::to_string(_ID) + " vuoi comprare " + posNow1->getName() + " per " + std::to_string(posNow1->getPrezzo()) + "? (Inserire S per si, N per no)\n";
    }

    if (posNow2)
    {
        if (_money < posNow2->getPrezzo())
        {
            std::cout << "Giocatore " << _ID << " non ha abbastanza " << Variabili::getValuta() << " per l'acquisto.\n";
            *p += "Giocatore " + std::to_string(_ID) + " non ha abbastanza " + Variabili::getValuta() + " per l'acquisto.\n";
            return false;
        }
        std::cout << "Giocatore " << _ID << " vuoi comprare " << posNow2->getName() << " per " << posNow2->getPrezzo() << "? (Inserire S per si, N per no)\n";
        *p += "Giocatore " + std::to_string(_ID) + " vuoi comprare " + posNow2->getName() + " per " + std::to_string(posNow2->getPrezzo()) + "? (Inserire S per si, N per no)\n";
    }

    if (_isInJail)
    {
        if (_money < Variabili::moneyUscitaPrigione)    // Se non ha soldi non può pagare, allora tira i dadi
        {
            std::cout << "Giocatore " << _ID << " non ha abbastanza soldi per uscire di prigione, quindi tira i dadi.\n";
            *p += "Giocatore " + std::to_string(_ID) + " non ha abbastanza soldi per uscire di prigione, quindi tira i dadi.\n";
            return false;
        }
        std::cout << "Giocatore " << _ID << " vuoi pagare " << Variabili::moneyUscitaPrigione << " " << Variabili::getValuta() << " ed uscire di prigione? (Inserire S per si, N per no)\n";
        *p += "Giocatore " + std::to_string(_ID) + " vuoi pagare " + std::to_string(Variabili::moneyUscitaPrigione) + " ed uscire di prigione? (Inserire S per si, N per no)\n";
    }

    do
    {
        char risposta;
        std::cin >> risposta;
        if (risposta == 'S' || risposta == 's')
        {
            if (risposta == 'S')
                *p += "S\n";
            else
                *p += "s\n";
            return true;
        }
        else if (risposta == 'N' || risposta == 'n')
        {
            if (risposta == 'N')
                *p += "N\n";
            else
                *p += "n\n";
            if (_isInJail)
            {
                std::cout << "Giocatore " + std::to_string(_ID) + " vuole tentare la sorte con i dadi.\n";
                *p += "Giocatore " + std::to_string(_ID) + " vuole tentare la sorte con i dadi.\n";
            }
            return false;
        } else
            std::cout << "Comando non riconosciuto (Inserire S per si, N per no)\n";
    } while (true);
    
    return false; // Non lo farà mai perchè ritorna nel do while quando l'utente inserisce S o N
}

bool Human::wantToBuild(Casella_Terreno* A){
    
    if (_money < A->getPrezzo()) // Se il giocatore non ha abbastanza soldi per costruire
        return false;

    std::string tmp = "una casa in ";
    if (A->isCasa4())
        tmp = "un albergo in ";
    std:: cout << "Giocatore " + std::to_string(_ID) + ", vuoi acquistare " + tmp + A->getName() + " per " << A->getPrezzo() << " " << Variabili::getValuta() + "? (Inserire S per si, N per no)\n";
    do
    {
        char risposta;
        std::cin >> risposta;
        if (risposta == 'S' || risposta == 's')
        {
            return true;
        }
        else if (risposta == 'N' || risposta == 'n')
        {
            return false;
        } else
            std::cout << "Comando non riconosciuto (Inserire S per si, N per no)\n";
    } while (true);
}

bool Human::partecipaAsta(int* p, Casella*, bool, int minimaOffertaAsta){

    int rilancio = 0;
    if (_money <= *p + minimaOffertaAsta)
    {
        std::cout << "Giocatore " << _ID << " non ha abbastanza " << Variabili::getValuta() << " per rilanciare.\n";
        std::this_thread::sleep_for(std::chrono::seconds(Variabili::pausa));
        return false;
    }
    else
    {// Il giocatore decide se partecipare all'asta
        std::cout <<  "Giocatore " << _ID << ", vuoi rilanciare (puntata minima: " << minimaOffertaAsta << ")? (Inserire S per si, N per no)\n";
        do
        {
            char risposta;
            std::cin >> risposta;
            if (risposta == 'S' || risposta == 's')
            {
                do
                {
                    std::cout << "Inserisci di quanto vuoi rilanciare (" << minimaOffertaAsta << "-" << (_money-*p) << "):\n";
                    while (!(std::cin >> rilancio)) {
                    std::cin.clear(); // Pulisce lo stato dell'input
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora il resto dell'input fino a newline
                    std::cout << "Input non valido. Inserisci un numero intero:\n";
                    }
                    if(rilancio > (_money-*p) || rilancio < minimaOffertaAsta)
                    {
                        std::cout << "Non puoi rilanciare di " << rilancio << " " << Variabili::getValuta() << ", riprova.\n";
                    }
                    else
                    {
                        *p = *p + rilancio;
                        return true;
                    }
                } while (true);
            }
            else if (risposta == 'N' || risposta == 'n')
            {
                return false;
            } else
                std::cout << "Comando non riconosciuto (Inserire S per si, N per no)\n";
        } while (true);
    }

    return false; // Non lo farà mai
}

void Human::Transaction(int n, Giocatore *Other, std::string *output){
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
            std::this_thread::sleep_for(std::chrono::seconds(pausa));
        std::string *toAdd = &init;
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
        if (FreeExitPrisonProb && _money<n)
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

        while (_money < n)   // Finché il giocatore non ha abbastanza soldi per pagare
        {
            // Richiesta all'utente di quale tipo di casella egli voglia ipotecare (se non ha nulla da ipotecare perde)
            char risposta;
            if (_elenco_proprieta_soc.size() > 0 && _elenco_proprieta_st.size() > 0 && _elenco_proprieta.size() > 0)
            {
                std::cout << "Cosa vuoi ipotecare: inserire T per i terreni, C per le Societa', S per le Stazioni:\n";
                bool rispostaValida = false;
                do
                {
                    std::cin >> risposta;
                    if (risposta == 'T' || risposta == 't' || risposta == 'C' || risposta == 'c' || risposta == 'S' || risposta == 's')
                    {
                        rispostaValida = true;
                    }
                    else
                    {
                        rispostaValida = false;
                        std::cout << "Comando non riconosciuto (Inserire T per i terreni, C per le Societa', S per le Stazioni):\n";
                    }
                } while (!rispostaValida);
            }
            else if(_elenco_proprieta_soc.size() == 0 && _elenco_proprieta_st.size() > 0 && _elenco_proprieta.size() > 0)
            {
                std::cout << "Cosa vuoi ipotecare: inserire T per i terreni, S per le Stazioni:\n";
                bool rispostaValida = false;
                do
                {
                    std::cin >> risposta;
                    if (risposta == 'T' || risposta == 't' || risposta == 'S' || risposta == 's')
                    {
                        rispostaValida = true;
                    }
                    else
                    {
                        rispostaValida = false;
                        std::cout << "Comando non riconosciuto (Inserire T per i terreni, S per le Stazioni):\n";
                    }
                } while (!rispostaValida);
            }
            else if (_elenco_proprieta_soc.size() > 0 && _elenco_proprieta_st.size() == 0 && _elenco_proprieta.size() > 0)
            {
                std::cout << "Cosa vuoi ipotecare: inserire T per i terreni, C per le Societa':\n";
                bool rispostaValida = false;
                do
                {
                    std::cin >> risposta;
                    if (risposta == 'T' || risposta == 't' || risposta == 'C' || risposta == 'c')
                    {
                        rispostaValida = true;
                    }
                    else
                    {
                        rispostaValida = false;
                        std::cout << "Comando non riconosciuto (Inserire T per i terreni, C per le Societa'):\n";
                    }
                } while (!rispostaValida);
            }
            else if (_elenco_proprieta_soc.size() > 0 && _elenco_proprieta_st.size() > 0 && _elenco_proprieta.size() == 0)
            {
                std::cout << "Cosa vuoi ipotecare: inserire C per le Societa', S per le Stazioni:\n";
                bool rispostaValida = false;
                do
                {
                    std::cin >> risposta;
                    if (risposta == 'C' || risposta == 'c' || risposta == 'S' || risposta == 's')
                    {
                        rispostaValida = true;
                    }
                    else
                    {
                        rispostaValida = false;
                        std::cout << "Comando non riconosciuto (Inserire C per le Societa', S per le Stazioni):\n";
                    }
                } while (!rispostaValida);
            }
            else if (_elenco_proprieta_soc.size() > 0 && _elenco_proprieta_st.size() == 0 && _elenco_proprieta.size() == 0)
            {
                risposta = 'C';
            }
            else if (_elenco_proprieta_soc.size() == 0 && _elenco_proprieta_st.size() > 0 && _elenco_proprieta.size() == 0)
            {
                risposta = 'S';
            }
            else if (_elenco_proprieta_soc.size() == 0 && _elenco_proprieta_st.size() == 0 && _elenco_proprieta.size() > 0)
            {
                risposta = 'T';
            }
            else if (_elenco_proprieta.empty() && _elenco_proprieta_st.empty() && _elenco_proprieta_soc.empty() && _money<n)
            {
                // Se il giocatore non ha più nulla da ipotecare, paga tutti i soldi che ha ...
                std::cout << "Giocatore " + std::to_string(_ID) + " non ha piu nulla da ipotecare e non riesce a pagare.\n";
                *toAdd += "Giocatore " + std::to_string(_ID) + " non ha piu nulla da ipotecare e non riesce a pagare.\n";
                std::this_thread::sleep_for(std::chrono::seconds(pausa));
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
            
            // Richiesta all'utente casella per casella se vuole ipotecarla o meno
            std::this_thread::sleep_for(std::chrono::seconds(pausa));
            if (risposta == 'C' || risposta == 'c') // Società
            {
                for (int i=0; i < _elenco_proprieta_soc.size(); i++)
                {
                    if (_money < n)
                    {
                        std::cout << "Vuoi ipotecare " << _elenco_proprieta_soc[i]->getName() << "? (Inserire S per si, N per no)\n";
                        bool done = false;
                        do
                        {
                            char gg;
                            std::cin >> gg;
                            if (gg == 'S' || gg == 's')
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
                                _elenco_proprieta_soc[i]->reset();
                                _elenco_proprieta_soc.erase(_elenco_proprieta_soc.begin() + i);
                                i--;
                                done = true;
                            }
                            else if (gg == 'N' || gg == 'n')
                            {
                                std::cout << "Giocatore " << _ID << " non ha ipotecato " << _elenco_proprieta_soc[i]->getName() << ".\n";
                                done = true;
                            } else
                                std::cout << "Comando non riconosciuto (Inserire S per si, N per no)\n";
                        } while (!done);
                    }
                    else
                        i=50;   // Per uscire dal ciclo (sono apposto e non devo più ipotecare)
                }
            }
            if (risposta == 'S' || risposta == 's') // Stazione
            {
                for (int i=0; i < _elenco_proprieta_st.size(); i++)
                {
                    if (_money < n)
                    {
                        std::cout << "Vuoi ipotecare " << _elenco_proprieta_st[i]->getName() << "? (Inserire S per si, N per no)\n";
                        bool done = false;
                        do
                        {
                            char gg;
                            std::cin >> gg;
                            if (gg == 'S' || gg == 's')
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
                                _elenco_proprieta_st[i]->reset();
                                _elenco_proprieta_st.erase(_elenco_proprieta_st.begin() + i);
                                i--;
                                done = true;
                            }
                            else if (gg == 'N' || gg == 'n')
                            {
                                done = true;
                            }
                            else
                                std::cout << "Comando non riconosciuto (Inserire S per si, N per no)\n";
                        } while (!done);
                    }
                    else
                        i=50;   // Per uscire dal ciclo (sono apposto e non devo più ipotecare)
                }
            }
            if (risposta == 'T' || risposta == 't') // Terreno
            {
                if (!_elenco_proprieta_to_build.empty())
                {
                    bool toDo = true; // Finché nel vettore c'è ancora 1 sola proprietà con 1 casa non si esce dal ciclo (o se raggiungo il giusto da pagare)
                    while (toDo)
                    {
                        for (int i=0; i < _elenco_proprieta_to_build.size(); i++)
                        {
                            if (_money < n)
                            {
                                if (_elenco_proprieta_to_build[i]->isCasa1())
                                {
                                    std::string s = " una casa in ";
                                    if (_elenco_proprieta_to_build[i]->isAlbergo())
                                        s = " l'albergo in ";
                                    std::cout << "Vuoi ipotecare" << s << _elenco_proprieta_to_build[i]->getName() << "? (Inserire S per si, N per no)\n";
                                    bool done = false;
                                    do
                                    {
                                        char gg;
                                        std::cin >> gg;
                                        if (gg == 'S' || gg == 's')
                                        {
                                            int schei = _elenco_proprieta_to_build[i]->getPrezzo()/2;
                                            std::cout << "Giocatore " << _ID << " ha ipotecato" << s << _elenco_proprieta_to_build[i]->getName() << " e ha ricavato " << schei << " " << Variabili::getValuta() << ".\n";
                                            *toAdd += "-" + s + _elenco_proprieta_to_build[i]->getName() + " (" + std::to_string(schei) + ")\n";
                                            deposit(schei);
                                            _elenco_proprieta_to_build[i]->ipoteca();
                                            _elenco_proprieta_to_build[i]->setCanBuy(true);
                                            for (int j=0; j<_elenco_proprieta_to_build.size(); j++)
                                            {
                                                if (_elenco_proprieta_to_build[j]->getFamily() == _elenco_proprieta_to_build[i]->getFamily() && _elenco_proprieta_to_build[j]->getStatus() > _elenco_proprieta_to_build[i]->getStatus())
                                                    _elenco_proprieta_to_build[j]->setCanBuy(false);
                                            }
                                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                            std::cout << "Giocatore " << _ID << " ora ha " << _money << " " << Variabili::getValuta();
                                            if (_money < n)
                                                std::cout << ", ma ancora non bastano.\n";
                                            else
                                                std::cout << ", abbastanza per pagare.\n";
                                            done = true;
                                        }
                                        else if (gg == 'N' || gg == 'n')
                                        {
                                            done = true;
                                        } else
                                            std::cout << "Comando non riconosciuto (Inserire S per si, N per no)\n";
                                    } while (!done);
                                    
                                }
                            }
                        }
                        if (toDo = true && _money < n)
                        {
                            bool allsold = true;
                            for (int s = 0; s < _elenco_proprieta_to_build.size(); s++)
                            {
                                if (_elenco_proprieta_to_build[s]->isCasa1())
                                    allsold = false;
                            }
                            if (allsold)
                                toDo = false;
                        }
                    }
                }
                for (int i=0; i < _elenco_proprieta.size(); i++)
                {
                    if (_money < n)
                    {
                        if (!_elenco_proprieta[i]->isCasa1())
                        {
                            std::cout << "Vuoi ipotecare " << _elenco_proprieta[i]->getName() << "? (Inserire S per si, N per no)\n";
                            bool done = false;
                            do
                            {
                                char gg;
                                std::cin >> gg;
                                if (gg == 'S' || gg == 's')
                                {
                                    int schei = _elenco_proprieta[i]->getPrezzoTerreno()/2;
                                    std::cout << "Giocatore " << _ID << " ha ipotecato " << _elenco_proprieta[i]->getName() << " e ha ricavato " << schei << " " << Variabili::getValuta() << ".\n";
                                    *toAdd += "-" + _elenco_proprieta[i]->getName() + " (" + std::to_string(schei) + ")\n";
                                    deposit(schei);
                                    if (_elenco_proprieta[i]->canBuy()) // Se il giocatore aveva tutte le proprietà del colore di quella ipotecata
                                    {
                                        for (int x=0; x < _elenco_proprieta_to_build.size(); x++)
                                        {
                                            if (_elenco_proprieta_to_build[x]->getFamily() == _elenco_proprieta[i]->getFamily())
                                            {
                                                _elenco_proprieta_to_build[x]->setCanBuy(false);    // Per ciascun terreno della stessa famiglia di quello che ho appena ipotecato setto canBuy a false
                                                _elenco_proprieta_to_build.erase(_elenco_proprieta_to_build.begin() + x); // e li tolgo dal vettore _elenco_proprieta_to_build
                                                x--;
                                            }
                                        }
                                    }
                                    _elenco_proprieta[i]->reset();
                                    _elenco_proprieta.erase(_elenco_proprieta.begin() + i);
                                    i--;

                                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                    std::cout << "Giocatore " << _ID << " ora ha " << _money << " " << Variabili::getValuta();
                                    if (_money < n)
                                        std::cout << ", ma ancora non bastano.\n";
                                    else
                                        std::cout << ", abbastanza per pagare.\n";
                                    done = true;
                                }
                                else if (gg == 'N' || gg == 'n')
                                {
                                    done = true;
                                } else
                                    std::cout << "Comando non riconosciuto (Inserire S per si, N per no)\n";
                            } while (!done);
                        }
                    }
                }
            }
        }

        // Se non viene lanciata l'eccezione, allora esco dal ciclo con una quantità di soldi sufficiente da poter pagare (ovviamente avendo ipotecato qualcosa)
        if (Other)
            Other->deposit(n);
        this->pay(n);
        *output += *toAdd;
    }
}

Giocatore &Human::operator=(Human *g){
    _pos = g->getPosition();
    _ID = g->getID();
    _isInGame = g->isInGame();
    _money = g->getMoney();
    // Aggiungo il giocatore al tabellone
    _pos->addPlayer(_ID);
    return *this;
}
