#include "../include/Human.h"
#include "../include/Computer.h"
#include "../include/Tabellone.h"

std::pair<int,int> getDiceRoll();                   // restituisce la somme di due numeri casuali tra 1 e 6
void startingOrder(std::vector<Giocatore *> &players); // ordina i players in base ai lanci dei dadi
void aggiornaSchermo(Tabellone, std::vector<Giocatore *>, std::string);
void clearScreen() { system("cls"); }
int tiriIniziali = 0;
int pausa = Variabili::pausa;  // Pausa che il programma fa in output tra una stampa e la successiva

int main(int argc, char *argv[])
{
// Dichiarazione di variabili
    int j = 0,z=0;  // j = Contatore di turni, z = contatore di proprietà sdello stesso colore (usata anche come nGiocatori ad inizio game)
    int nMaxTurni = 200, nScelteP = 16, nScelteI = 16, minimaOffertaAsta = 10;
    std::pair<int,int> tiro;
    std::string muro="";
    for (int i = 0; i < Variabili::dimMaxOutput; i++)
        muro += "*";  
    Tabellone T;                      // creo il tabellone
    std::vector<Giocatore *> players; // I giocatori che sono dentro players sono quelli che giocano, se un giocatore viene eliminato esce dal vettore
    std::vector<int> winner;          // Vettore che serve a fine partita, conterrà il/i vincitore/i
    
// Richiesta all'utente del numero di giocatori
    clearScreen();
    std::cout << muro << "\nBenvenuto/a, sta per iniziare una partita di Monopoly.";
    std::cout << "\nInserire il numero di giocatori (da 2 a 6):\n";
    do
    {
        std::cin >> z;
        if (z < 2 || z > 6)
        {
            std::cout << "Comando non valido, inserire un numero da 2 a 6:\n";
        }
    } while (z < 2 || z > 6);

// Creazione dei giocatori
    // Creazione dei giocatori e relativo push nel vettore di players
    for (int i = 1; i <= z; i++)
    {
        // Richiesta all'utente se il giocatore che devo creare è un computer o un human
        std::cout << "Che tipo di giocatore e' giocatore " << i << "? (inserire C per computer, H per Human)\n";
        char risposta;
        do
        {
            std::cin >> risposta;
            if (risposta != 'C' && risposta != 'c' && risposta != 'H' && risposta != 'h')
            {
                std::cout << "Comando non valido, inserire C per computer, H per Human:\n";
            }
        }while (risposta != 'C' && risposta != 'c' && risposta != 'H' && risposta != 'h');
        // Creazione del giocatore, prima puntatore da inserire in players e poi il giocatore
        Giocatore *player;
        if (risposta == 'C' || risposta == 'c')
            player = new Computer{i};
        else
            player = new Human{i};
        players.push_back(player);
        std::cout << "Creato Giocatore " << i;
        if (risposta == 'C' || risposta == 'c')
            std::cout << ", Computer.\n";
        else
            std::cout << ", Human.\n";
        
    }
    std::cout << "Tutti i giocatori sono stati creati.\n" << muro;
    std::this_thread::sleep_for(std::chrono::seconds(pausa));
    clearScreen();
    std::cout << muro << "\nTutti i giocatori lanciano i dadi per determinare l'ordine di gioco.\n";
    std::this_thread::sleep_for(std::chrono::seconds(pausa));

// Tutti i giocatori lanciano i dadi per determinare l’ordine di gioco (ordinamento di players)
    startingOrder(players);
    std::cout << "\nOrdine dei giocatori:";
    for (int i = 0; i < players.size(); i++)
    {
        std::cout << " " << players[i]->getID();
    }
    std::cout << ".\n" << muro;
    std::this_thread::sleep_for(std::chrono::seconds(pausa));

// Aggiungo i giocatori al tabellone
    for (int i = 0; i < players.size(); i++)
    {
        players[i]->setPos(T.partenza);
    }

// Conto alla rovescia per far iniziare il game
    for (int i=0; i < Variabili::contoAllaRovescitaDa; i++)
    {
        aggiornaSchermo(T, players, "\n" +  muro);
        std::cout << "\nORA INIZIA IL GIOCO.\n" << Variabili::contoAllaRovescitaDa-i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

// Ciclo while = finche ha senso giocare (finchè non c'è almeno un vincitore)
    while (winner.empty())
    {
        j++; // aumento il conteggio dei turni
        
// Verifico che non siano passati troppi turni, altrimenti il gioco è in stallo, quindi finisce
        if ((j >= nMaxTurni))   // Arrivo al turno di gioco max per ogni giocatore, il gioco si ferma e vince chi ha più soldi
        {
            // Ordino il vettore da quello con più soldi [0], a quello con meno soldi [players.size()-1]
            for (int i = 1; i < players.size(); i++)
            {
                if (players[i]->getMoney() > players[i-1]->getMoney())
                {
                    // Scambio di posto i 2 player
                    Giocatore* tmp = players[i-1];
                    players[i-1] = players[i];
                    players[i] = tmp;
                    i=0;    // Se viene effettuato uno scambio torno all'inizio del vettore e ricomincio la verifica se è ordinato
                }
            }
            // Ora che il vettore players è ordìdinato in modo che il primo giocatore sia quello con più soldi
            // controllo se qualche altro giocatore ha lo stesso quantitativo di soldi
            for (int i = 1; i < players.size(); i++)
            {
                if (players[i]->getMoney() == players[0]->getMoney())   // C'è più di un vincitore
                {
                    winner.push_back(players[i]->getID());  // push ID del possibile altro vincitore tra i winner
                }
            }
            winner.push_back(players[0]->getID());  // push dell'ID del primo elemento di players (quello con più soldi)
            break; // Per uscire dal while
        }

// Ciclo for = i giocatori ancora in gioco (quelli dentro players) fanno il loro turno
        for (int i = 0; i < players.size(); i++)
        {
            std::string output = "";    // eventi del giocatore stampati a video
            bool toDoRoll = true;
            bool sold = true;  // Per capire se fare l'asta
            bool affittoRaddoppiato = false;
            bool wantToBuy;

// Aggiorno schermo
            aggiornaSchermo(T,players,output);
            
// Inizio turno di un giocatore
            std::cout << "\n" << muro;
            output += "\n"+ muro;
            
            std::string turno = "turno";
            if (players[i]->getNTurno() == 2)    // Secondo turno extra (causa lancio dei dadi)
                turno = "secondo turno extra";
            if (players[i]->getNTurno() == 3)    // Terzo turno extra (causa lancio dei dadi)
                turno = "terzo turno extra";
            std::cout << "\nInizio " << turno << " del giocatore " << players[i]->getID() << ".\n";
            output += "\nInizio " + turno + " del giocatore " + std::to_string(players[i]->getID()) + ".\n";
            std::this_thread::sleep_for(std::chrono::seconds(pausa));


// Il giocatore decide se comprare case/alberghi in proprietà se può farlo
        if (!players[i]->_elenco_proprieta_to_build.empty())
        {
            bool playerCanBuild = true; // se ci sono le condizioni per l'acquisto di case/alberghi
            if (players[i]->getMoney() < players[i]->_elenco_proprieta_to_build[0]->getPrezzo())
            {   // Se il giocatore non ha abbastanza soldi per comprare una casa/albergo
                playerCanBuild = false;
            }
            bool terreniEdificabili = false;
            // Controllo se posso costruire (se ci sono tutti alberghi non posso costruire nulla)
            for (int d = 0; d < players[i]->_elenco_proprieta_to_build.size(); d++)
            {
                if (!players[i]->_elenco_proprieta_to_build[d]->isAlbergo())    // Se almeno in uno degli elementi del vettore non c'è ancora un albergo
                    terreniEdificabili = true;
            }
            if (!terreniEdificabili)
                playerCanBuild = false;
            // Se playerCanBuild è true significa che il giocatore ha abbastanza soldi e ci sono terreni su cui può costruire
            if (playerCanBuild) 
            {
                if (dynamic_cast<Human*>(players[i]))
                {
                    std::cout << "Giocatore " + std::to_string(players[i]->getID()) + ", vuoi acquistare qualche casa/albergo? (Inserire S per si, N per no)\n";
                    bool a = true; // a per uscire dal ciclo
                    char risposta;
                    do
                    {
                        std::cin >> risposta;
                        if (risposta == 'S' || risposta == 's')
                        {
                            a = false;
                        }
                        else if (risposta == 'N' || risposta == 'n')
                        {
                            a = false;
                            playerCanBuild = false;
                        } else
                            std::cout << "Comando non riconosciuto (Inserire S per si, N per no)\n";
                    } while (a);
                }
                if (playerCanBuild)
                {
                    for (int l=0; l < players[i]->_elenco_proprieta_to_build.size(); l++)
                    {
                        if (!players[i]->_elenco_proprieta_to_build[l]->isAlbergo())    // Se non c'è ancora l'albergo si può costruire
                        {
                            // Aggiorno tutte le proprietà se è possibile costruire o meno
                            int luigi = 0;
                            for (int h=0; h < players[i]->_elenco_proprieta_to_build.size(); h++)
                            {
                                if (players[i]->_elenco_proprieta_to_build[l]->getFamily() == players[i]->_elenco_proprieta_to_build[h]->getFamily())
                                {
                                    if (players[i]->_elenco_proprieta_to_build[h]->getStatus() >= players[i]->_elenco_proprieta_to_build[l]->getStatus())
                                    {
                                        players[i]->_elenco_proprieta_to_build[h]->setCanBuy(false);
                                        luigi++;
                                    }
                                }
                            }
                            if (luigi == players[i]->_elenco_proprieta_to_build[l]->getNFamily())  // Se sono tutte con canbuy false, ciò significa che la costruzione è in pari, perciò (quelle della stessa famiglia) le re-setto tutte a true
                            {
                                for (int h=0; h < players[i]->_elenco_proprieta_to_build.size(); h++)
                                {
                                    if (players[i]->_elenco_proprieta_to_build[l]->getFamily() == players[i]->_elenco_proprieta_to_build[h]->getFamily())
                                        players[i]->_elenco_proprieta_to_build[h]->setCanBuy(true);
                                }
                            }
                            if (!players[i]->_elenco_proprieta_to_build[l]->canBuy()) // Se in questo terreno non posso comprare vado al prossima
                                continue;
                            
                            if (players[i]->wantToBuild(players[i]->_elenco_proprieta_to_build[l]))  // Il giocatore sceglie se comprare una casa/albergo (se non ha abbastanza soldi per l'acquisto restituisce false)
                            {
                                try
                                {
                                    players[i]->pay(players[i]->_elenco_proprieta_to_build[l]->getPrezzo());
                                    players[i]->_elenco_proprieta_to_build[l]->build();
                                    if (players[i]->_elenco_proprieta_to_build[l]->isAlbergo()) // se dopo l'acquisto c'è un albergo
                                    {
                                        output += "Giocatore " + std::to_string(players[i]->getID()) + " ha pagato " + std::to_string(players[i]->_elenco_proprieta_to_build[l]->getPrezzo()) + " " + Variabili::getValuta() + " e ha acquistato un albergo in " + players[i]->_elenco_proprieta_to_build[l]->getName() + ".\n";
                                    }
                                    else if (players[i]->_elenco_proprieta_to_build[l]->isCasa1())    // c'è una casa dopo l'acquisto, quindi dovro stampare ...
                                    {
                                        output += "Giocatore " + std::to_string(players[i]->getID()) + " ha pagato " + std::to_string(players[i]->_elenco_proprieta_to_build[l]->getPrezzo()) + " " + Variabili::getValuta() + " e ha acquistato una casa in " + players[i]->_elenco_proprieta_to_build[l]->getName() + ".\n";
                                    }
                                    aggiornaSchermo(T,players,output);
                                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                }
                                catch (const Giocatore::Not_Enough_Money& e)    // non dovrebbe mai lanciarla, ma meglio gestirla ...
                                {
                                    // Non succede nulla
                                }
                            }
                        }
                    }
                }
            }
        }

    // Se il giocatore è in prigione
            if(players[i]->isInJail())
            {
                if (players[i]->hasFreeExitPrisonImpr())
                {
                    players[i]->setJail(false);
                    std::cout << "Giocatore " << players[i]->getID() << " ha usato la carta imprevisto uscita gratis di prigione che aveva ottenuto in precedenza.\n";
                    output += "Giocatore " + std::to_string(players[i]->getID()) + " ha usato la carta per uscire gratis di prigione che aveva ottenuto in precedenza.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    nScelteI = 16;
                    players[i]->setFreeExitPrisonImpr(false);
                }
                else if (players[i]->hasFreeExitPrisonProb())
                {
                    players[i]->setJail(false);
                    std::cout << "Giocatore " << players[i]->getID() << " ha usato la carta probabilita' uscita gratis di prigione che aveva ottenuto in precedenza.\n";
                    output += "Giocatore " + std::to_string(players[i]->getID()) + " ha usato la carta probabilita' uscita gratis di prigione che aveva ottenuto in precedenza.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    nScelteP = 16;
                    players[i]->setFreeExitPrisonProb(false);
                }

                if (players[i]->isInJail())
                {
                    std::cout << "Giocatore " << players[i]->getID() << " e' in prigione (turno n" << players[i]->getTurniJail() << ").\n";
                    output += "Giocatore " + std::to_string(players[i]->getID()) + " e' in prigione (turno n" + std::to_string(players[i]->getTurniJail()) + ").\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    
                    if (players[i]->choice(&output)) // Il giocatore sceglie se uscire pagando tot euro
                    {
                        players[i]->pay(Variabili::moneyUscitaPrigione);
                        players[i]->setJail(false);
                        std::cout << "Giocatore " << players[i]->getID() << " ha pagato " << Variabili::moneyUscitaPrigione << " " << Variabili::getValuta() << " per uscire di prigione.\n";
                        output += "Giocatore " + std::to_string(players[i]->getID()) + " ha pagato " + std::to_string(Variabili::moneyUscitaPrigione) + " " + Variabili::getValuta() + " per uscire di prigione.\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    }
                    else   // se il giocatore non è uscito pagando
                    {                             // allora tira i dadi
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        tiro = getDiceRoll();
                        toDoRoll = false;
                        std::cout << "Giocatore " << players[i]->getID() << " ha tirato i dadi e ha fatto " << tiro.first << " + " << tiro.second << " = " << (tiro.first + tiro.second) << ".\n";
                        output += "Giocatore " + std::to_string(players[i]->getID()) + " ha tirato i dadi e ha fatto "+ std::to_string(tiro.first) + " + " + std::to_string(tiro.second) + " = " + std::to_string(tiro.first + tiro.second) + ".\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));

                        if (tiro.first == tiro.second)
                        {
                            players[i]->setJail(false);
                            std::cout << "Giocatore " << players[i]->getID() << " ha fatto 2 numeri uguali, quindi esce di prigione.\n";
                            output += "Giocatore " + std::to_string(players[i]->getID()) + " ha fatto 2 numeri uguali, quindi esce di prigione.\n";
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        }
                        else
                        {
                            // Se il giocatore è in prigione e ci ha passato già nMaxTurniPrigione turni
                            if (players[i]->getTurniJail() == Variabili::nMaxTurniPrigione)
                            {
                                players[i]->setJail(false);
                                std::cout << "Giocatore " << players[i]->getID() << " ha passato " << Variabili::nMaxTurniPrigione << " turni in prigione, quindi esce pagando " << Variabili::moneyUscitaPrigione << " " + Variabili::getValuta() + ".\n";
                                output += "Giocatore " + std::to_string(players[i]->getID()) + " ha passato " + std::to_string(Variabili::nMaxTurniPrigione) + " turni in prigione, quindi esce pagando " + std::to_string(Variabili::moneyUscitaPrigione) + " " + Variabili::getValuta() + ".\n";
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                bool goodTransaction;
                                try
                                {
                                    players[i]->Transaction(50, nullptr, &output);
                                    goodTransaction = true;
                                }
                                catch (const Giocatore::Player_Lost& e)
                                {
                                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                    output += "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
                                    T.prison->removePlayer(players[i]->getID());
                                    goodTransaction = false;
                                }
                                aggiornaSchermo(T,players,output);
                                if (goodTransaction == false)
                                {
                                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                    std::cout << "Fine " << turno << " del giocatore " << players[i]->getID() << ".\n";
                                    std::cout << muro;
                                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                    continue;
                                }

                            }
                            else
                            {
                        
                                if (players[i]->getNTurno() > 1)   // Se è in un turno extra (secondo o terzo) e ha fatto un numero di dadi diverso
                                    players[i]->resetNTurno();  // Il turno del giocatore torna ad 1 e si passa al giocatore successivo
                                std::cout << "Giocatore " << players[i]->getID() << " ha fatto 2 numeri diversi, quindi resta in prigione.\n";
                                players[i]->incrementTurniJail();
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                std::cout << "Fine " << turno << " del giocatore " << players[i]->getID() << ".\n";
                                std::cout << muro;
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                continue;
                            }
                        }
                    }
                }
            }

    // Se il giocatore non è in prigione o ne è appena uscito    
        // Tiro dei dadi
            // Controllo se ho già lanciato i dadi in prigione, in questo caso non li lancio
            if (toDoRoll)
            {
                tiro = getDiceRoll();
                std::cout << "Giocatore " << players[i]->getID() << " ha tirato i dadi e ha fatto " << tiro.first << " + " << tiro.second << " = " << (tiro.first + tiro.second) << ".\n";
                output += "Giocatore " + std::to_string(players[i]->getID()) + " ha tirato i dadi e ha fatto "+ std::to_string(tiro.first) + " + " + std::to_string(tiro.second) + " = " + std::to_string(tiro.first + tiro.second) + ".\n";
                std::this_thread::sleep_for(std::chrono::seconds(pausa));
            }
            
            // Se dopo 3 turni consecutivi il giocatore ha fatto nuovamente un doppio tiro, allora va diretto in prigione
            if ((players[i]->getNTurno() == 3) && (tiro.first == tiro.second))
            {
                players[i]->setJail(true);
                players[i]->resetNTurno();
                std::cout << "Giocatore " << players[i]->getID() << " ha ottenuto un numero doppio per 3 volte di fila, quindi (come da regolamento) va in prigione.\n";
                output += "Giocatore " + std::to_string(players[i]->getID()) + " ha ottenuto un numero doppio per 3 volte di fila, quindi (come da regolamento) va in prigione.\n";
                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                // Movimento nella casella Transito/Prigione
                players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                players[i]->goTo(T.prison);
                T.prison->addPlayer(players[i]->getID()); // Aggiunta al tabellone del giocatore (nella casella dove si è spostato)
                aggiornaSchermo(T,players,output);
                std::cout << "Fine " << turno << " del giocatore " << players[i]->getID() << ".\n";
                std::cout << muro;
                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                continue;
            }
            
    // Movimento sul tabellone
            int tmpMoney = players[i]->getMoney();  // Per intercettare il passaggio dal via e stamparlo
            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
            players[i]->move(tiro.first + tiro.second);
            players[i]->getPosition()->addPlayer(players[i]->getID()); // Aggiunta al tabellone del giocatore (nella casella dove si è spostato)
            aggiornaSchermo(T,players,output);
            if (players[i]->getPosition() == T.partenza)
            {
                std::cout << "Giocatore " << players[i]->getID() << " si e' mosso sul via e ha ritirato " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                output += "Giocatore " + std::to_string(players[i]->getID()) + " si e' mosso sul via e ha ritirato " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                std::this_thread::sleep_for(std::chrono::seconds(pausa));
            }
            else
            {
                if (!players[i]->isInJail())
                {
                    std::cout << "Giocatore " << players[i]->getID() << " si e' mosso su " << players[i]->getPosition()->getName() << ".\n";
                    output += "Giocatore " + std::to_string(players[i]->getID()) + " si e' mosso su " + players[i]->getPosition()->getName() + ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                }
        
                // Stampo il passaggio dal via (avvenuto nel move)
                if (players[i]->getMoney() > tmpMoney)
                {
                    std::cout << "Giocatore " << players[i]->getID() << " e' passato dal via e ha ritirato " <<  Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ".\n";
                    output += "Giocatore " + std::to_string(players[i]->getID()) + " e' passato dal via e ha ritirato " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                }
            }
// Verifico su che casella è il giocatore
            // Prima controllo se sono finito su una probabilità/imprevisto che potrebbe mandarmi da altre parti nel tabellone
            Casella_Prob_Impr *pos5 = dynamic_cast<Casella_Prob_Impr*>(players[i]->getPosition());
    // Casella Probabilità Imprevisti
            if (pos5)
            {
                std::random_device rd;
                std::mt19937 gen(rd());
                bool pay = false;
                bool deposit = false;
                int money = 0;
                int counterAlb = 0, counterCase = 0, pagato = 0, guadagno = 0;                
    // Imprevisti
                if (pos5->getNameOutput() == "?I?")
                {
                    std::uniform_int_distribution<int> distr(1,nScelteI);
                    tmpMoney = players[i]->getMoney();  // Per intercettare il passaggio dal via e stamparlo
                    int n = distr(gen);
                    // per simulare la pescata di una carta dal mazzetto, poi la carta si rimette in mezzo e si mescola.
                    switch (n)
                    {
                        case 1:
                            std::cout << "Fate tre passi indietro (con tanti auguri).\n";
                            output += "Fate tre passi indietro (con tanti auguri).\n";
                            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                            players[i]->moveBack(3);
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            aggiornaSchermo(T,players,output);
                            std::cout << "Giocatore " << players[i]->getID() << " si e' mosso su " << players[i]->getPosition()->getName() << ".\n";
                            output += "Giocatore " + std::to_string(players[i]->getID()) + " si e' mosso su " + players[i]->getPosition()->getName() + ".\n";
                            if (players[i]->getPosition())
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        break;
                        
                        case 2:
                            std::cout << "Andate fino a Parco Della Vittoria.\n";
                            output += "Andate fino a Parco Della Vittoria.\n";
                            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                            players[i]->goTo("Parco Della Vittoria");
                            T.partenza->getPrec()->addPlayer(players[i]->getID());
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            aggiornaSchermo(T,players,output);
                            if (players[i]->getMoney() > tmpMoney) // Stampo il passaggio dal via (avvenuto nel goTo)
                            {
                                std::cout << "Giocatore " << players[i]->getID() << " e' passato dal via e ha ritirato " <<  Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ".\n";
                                output += "Giocatore " + std::to_string(players[i]->getID()) + " e' passato dal via e ha ritirato " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            }
                        break;
                        
                        case 3:
                            std::cout << "Andate fino a Corso Ateneo (se passate dal Via ritirate " << Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ").\n";
                            output += "Andate fino a Corso Ateneo (se passate dal Via ritirate " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ").\n";
                            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                            players[i]->goTo("Corso Ateneo");
                            T.prison->getSucc()->getSucc()->getSucc()->addPlayer(players[i]->getID());
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            aggiornaSchermo(T,players,output);
                            if (players[i]->getMoney() > tmpMoney) // Stampo il passaggio dal via (avvenuto nel goTo)
                            {
                                std::cout << "Giocatore " << players[i]->getID() << " e' passato dal via e ha ritirato " <<  Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ".\n";
                                output += "Giocatore " + std::to_string(players[i]->getID()) + " e' passato dal via e ha ritirato " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            }
                        break;
                        
                        case 4:
                            for (int bob=0; bob < players[i]->_elenco_proprieta.size(); bob++)
                            {
                                if (players[i]->_elenco_proprieta[bob]->isAlbergo())
                                    counterAlb++;
                                if (players[i]->_elenco_proprieta[bob]->isCasa4())
                                    counterCase++;
                                if (players[i]->_elenco_proprieta[bob]->isCasa3())
                                    counterCase++;
                                if (players[i]->_elenco_proprieta[bob]->isCasa2())
                                    counterCase++;
                                if (players[i]->_elenco_proprieta[bob]->isCasa1())
                                    counterCase++;
                            }
                            std::cout << "Pagate per i contributi di miglioria stradale: 25 "<< Variabili::getValuta() << " per ogni casa, 100 "<< Variabili::getValuta() << " per ogni albergo.";
                            output += "Pagate per i contributi di miglioria stradale: 25 " + Variabili::getValuta() + " per ogni casa, 100 " + Variabili::getValuta() + " per ogni albergo.";
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            money = 25*counterCase + 100*counterAlb;
                            if (money > 0)
                            {
                                if (counterCase > 0)
                                {
                                    std::cout << "\nGiocatore " << players[i]->getID() << " ha " << counterCase << " case.\n";
                                    output += "\nGiocatore " + std::to_string(players[i]->getID()) + " possiede " + std::to_string(counterCase) + " case.\n";
                                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                }
                                if (counterAlb > 0) 
                                {
                                    std::cout << "\nGiocatore " << players[i]->getID() << " ha " << counterAlb << " alberghi.\n";
                                    output += "\nGiocatore " + std::to_string(players[i]->getID()) + " possiede " + std::to_string(counterAlb) + " alberghi.\n";
                                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                }
                            
                                std::cout << "\nGiocatore " << players[i]->getID() << " deve pagare " << counterCase << " x 25 + " << counterAlb << " x 100 = " << money << " " << Variabili::getValuta() << ".\n";
                                output += "\nGiocatore " + std::to_string(players[i]->getID()) + " deve pagare " + std::to_string(counterCase) + " x 25 + "  + std::to_string(counterAlb) + " x 100 = "  + std::to_string(money) + " " + Variabili::getValuta() + ".\n";
                                pay = true;
                            }
                            else
                            {
                                std::cout << "\nGiocatore " << players[i]->getID() << " non ha ancora costruito case, quindi non paga nulla.\n";
                            }
                        break;
                        
                        case 5:
                            std::cout << "Andate fino a Largo Colombo (se passate dal Via ritirate " << Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ").\n";
                            output += "Andate fino a Largo Colombo (se passate dal Via ritirate " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ").\n";
                            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                            players[i]->goTo("Largo Colombo");
                            T.parcheggio->getSucc()->getSucc()->getSucc()->getSucc()->addPlayer(players[i]->getID());
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            aggiornaSchermo(T,players,output);
                            if (players[i]->getMoney() > tmpMoney) // Stampo il passaggio dal via (avvenuto nel goTo)
                            {
                                std::cout << "Giocatore " << players[i]->getID() << " e' passato dal via e ha ritirato " <<  Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ".\n";
                                output += "Giocatore " + std::to_string(players[i]->getID()) + " e' passato dal via e ha ritirato " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            }
                        break;
                        
                        case 6:
                            std::cout << "Andate fino a Stazione Nord (se passate dal Via ritirate " << Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ").\n";
                            output += "Andate fino a Stazione Nord (se passate dal Via ritirate " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ").\n";
                            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                            players[i]->goTo("Stazione Nord");
                            T.parcheggio->getSucc()->getSucc()->getSucc()->getSucc()->getSucc()->addPlayer(players[i]->getID());
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            aggiornaSchermo(T,players,output);
                            if (players[i]->getMoney() > tmpMoney) // Stampo il passaggio dal via (avvenuto nel goTo)
                            {
                                std::cout << "Giocatore " << players[i]->getID() << " e' passato dal via e ha ritirato " <<  Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ".\n";
                                output += "Giocatore " + std::to_string(players[i]->getID()) + " e' passato dal via e ha ritirato " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            }
                        break;
                        
                        case 7:
                            std::cout << "Avanzate fino alla stazione ferroviaria piu vicina, se e' di un altro giocatore l'affitto viene raddoppiato.\n";
                            output += "Avanzate fino alla stazione ferroviaria piu vicina:, se e' di un altro giocatore l'affitto viene raddoppiato.\n";
                            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                            players[i]->moveTo(1);
                            affittoRaddoppiato = true;
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            aggiornaSchermo(T,players,output);
                            if (players[i]->getMoney() > tmpMoney) // Stampo il passaggio dal via (avvenuto nel moveTo)
                            {
                                std::cout << "Giocatore " << players[i]->getID() << " e' passato dal via e ha ritirato " <<  Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ".\n";
                                output += "Giocatore " + std::to_string(players[i]->getID()) + " e' passato dal via e ha ritirato " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            }
                        break;
                        
                        case 8:
                            std::cout << "Avanzate fino alla societa' piu vicina, se e' di un altro giocatore l'affitto viene raddoppiato.\n";
                            output += "Avanzate fino alla societa' piu vicina, se e' di un altro giocatore l'affitto viene raddoppiato.\n";
                            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                            players[i]->moveTo(2);
                            affittoRaddoppiato = true;
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            aggiornaSchermo(T,players,output);
                            if (players[i]->getMoney() > tmpMoney) // Stampo il passaggio dal via (avvenuto nel moveTo)
                            {
                                std::cout << "Giocatore " << players[i]->getID() << " e' passato dal via e ha ritirato " <<  Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ".\n";
                                output += "Giocatore " + std::to_string(players[i]->getID()) + " e' passato dal via e ha ritirato " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            }
                        break;
                        
                        case 9:
                            std::cout << "Premio per massimo esponente di bossita', sei troppo un boss: ricevi 500 " + Variabili::getValuta() + ".\n";
                            output += "Premio per massimo esponente di bossita', sei troppo un boss: ricevi 500 " + Variabili::getValuta() + ".\n";
                            deposit = true;
                            money = 500;
                        break;
                        
                        case 10:
                            std::cout << "Multa per eccesso di velocita': pagate 150 " + Variabili::getValuta() + ".\n";
                            output += "Multa per eccesso di velocita': pagate 150 " + Variabili::getValuta() + ".\n";
                            pay = true;
                            money = 150;
                        break;
                        
                        case 11:
                            std::cout << "Maturano le cedole dei vostri fondi immobiliari: incassate 150 " + Variabili::getValuta() + ".\n";
                            output += "Maturano le cedole dei vostri fondi immobiliari: incassate 150 " + Variabili::getValuta() + ".\n";
                            deposit = true;
                            money = 150;
                        break;
                        
                        case 12:
                            std::cout << "La banca vi paga un dividendo di 50 " + Variabili::getValuta() + ".\n";
                            output += "La banca vi paga un dividendo di 50 " + Variabili::getValuta() + ".\n";
                            deposit = true;
                            money = 50;
                        break;
                        
                        case 13:
                            std::cout << "Siete stati promossi alla presidenza del consiglio di amministrazione: pagate 50 " + Variabili::getValuta() + " ad ogni giocatore.\n";
                            output += "Siete stati promossi alla presidenza del consiglio di amministrazione: pagate 50 " + Variabili::getValuta() + " ad ogni giocatore.\n";
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            for (int bob = 0; bob < players.size(); bob++)
                            {
                                if (players[bob]->getID() != players[i]->getID())
                                {
                                    try
                                    {
                                        players[i]->Transaction(50, players[bob], &output);
                                    }
                                    catch (const Giocatore::Player_Lost& e)
                                    {
                                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                        output += "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
                                        players[i]->getPosition()->removePlayer(players[i]->getID());
                                    }
                                }
                            }
                            aggiornaSchermo(T,players,output);
                            if (players[i]->isInGame())
                                std::cout << "Giocatore " << players[i]->getID() << " ha pagato " << (players.size()-1)*50 << " " + Variabili::getValuta() + ".\n";
                        break;
                        
                        case 14:
                            std::cout << "Andate in prigione direttamente e senza passare dal via.\n";
                            output += "Andate in prigione direttamente e senza passare dal via.\n";
                            players[i]->setJail(true);
                            // Movimento nella casella Transito/Prigione
                            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                            players[i]->goTo(T.prison);
                            T.prison->addPlayer(players[i]->getID()); // Aggiunta al tabellone del giocatore (nella casella dove si è spostato)
                            aggiornaSchermo(T,players,output);
                        break;
                        
                        case 15:
                            std::cout << "Andate avanti fino al via e ritirate " << Variabili::moneyPassaggioVia << " " << Variabili::getValuta() + ".\n";
                            output += "Andate avanti fino al via e ritirate " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                            // Movimento nella casella Via
                            players[i]->deposit(Variabili::moneyPassaggioVia);
                            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                            players[i]->goTo("Via");
                            T.partenza->addPlayer(players[i]->getID()); // Aggiunta al tabellone del giocatore (nella casella dove si è spostato)
                            aggiornaSchermo(T,players,output);
                            std::cout << "Giocatore " << players[i]->getID() << " si e' mosso sul via e ha ritirato " <<  Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ".\n";
                            output += "Giocatore " + std::to_string(players[i]->getID()) + " si e' mosso sul via e ha ritirato " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        break;
                        
                        case 16:
                            std::cout << "Giocatore " << players[i]->getID() << " ha ottenuto la carta Uscita gratis di prigione.\n";
                            players[i]->setFreeExitPrisonImpr(true);
                            nScelteI=15;
                        break;
                    }
                }
    // Probabilità
                Casella_Prob_Impr* pos5bis = dynamic_cast<Casella_Prob_Impr*>(players[i]->getPosition());
                bool tornatoIndietroSuProb = false;
                if (pos5bis)
                    if (pos5bis->getNameOutput() == "?P?")
                        tornatoIndietroSuProb = true;
                if (tornatoIndietroSuProb || pos5->getNameOutput() == "?P?")
                {
                    std::uniform_int_distribution<int> distr(1,nScelteP);
                    int n = distr(gen);
                    // per simulare la pescata di una carta dal mazzetto, poi la carta si rimette in mezzo e si mescola.
                    switch (n)
                    {
                        case 1:
                            std::cout << "Pagate la parcella del dottore di 50 " + Variabili::getValuta() + ".\n";
                            output += "Pagate la parcella del dottore di 50 " + Variabili::getValuta() + ".\n";
                            pay = true;
                            money = 50;
                        break;
                        
                        case 2:
                            std::cout << "Pagate le rette scolastiche dei vostri figli: versate 50 " + Variabili::getValuta() + ".\n";
                            output += "Pagate le rette scolastiche dei vostri figli: versate 50 " + Variabili::getValuta() + ".\n";
                            pay = true;
                            money = 50;
                        break;
                        
                        case 3:
                            std::cout << "Pagate la retta ospedaliera di 100 " + Variabili::getValuta() + ".\n";
                            output += "Pagate la retta ospedaliera di 100 " + Variabili::getValuta() + ".\n";
                            pay = true;
                            money = 100;
                        break;
                        
                        case 4:
                            for (int bob=0; bob < players[i]->_elenco_proprieta.size(); bob++)
                            {
                                if (players[i]->_elenco_proprieta[bob]->isAlbergo())
                                    counterAlb++;
                                if (players[i]->_elenco_proprieta[bob]->isCasa4())
                                    counterCase++;
                                if (players[i]->_elenco_proprieta[bob]->isCasa3())
                                    counterCase++;
                                if (players[i]->_elenco_proprieta[bob]->isCasa2())
                                    counterCase++;
                                if (players[i]->_elenco_proprieta[bob]->isCasa1())
                                    counterCase++;
                            }
                            std::cout << "Pagate per i contributi di miglioria stradale: 40 "<< Variabili::getValuta() << " per ogni casa, 115 "<< Variabili::getValuta() << " per ogni albergo.";
                            output += "Pagate per i contributi di miglioria stradale: 40 " + Variabili::getValuta() + " per ogni casa, 115 " + Variabili::getValuta() + " per ogni albergo.";
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            money = 40*counterCase + 115*counterAlb;
                            if (money > 0)
                            {
                                if (counterCase > 0)
                                {
                                    std::cout << "\nGiocatore " << players[i]->getID() << " ha " << counterCase << " case.\n";
                                    output += "\nGiocatore " + std::to_string(players[i]->getID()) + " possiede " + std::to_string(counterCase) + " case.\n";
                                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                }
                                if (counterAlb > 0) 
                                {
                                    std::cout << "\nGiocatore " << players[i]->getID() << " ha " << counterAlb << " alberghi.\n";
                                    output += "\nGiocatore " + std::to_string(players[i]->getID()) + " possiede " + std::to_string(counterAlb) + " alberghi.\n";
                                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                }
                                std::cout << "\nGiocatore " << players[i]->getID() << " deve pagare " << counterCase << " x 40 + " << counterAlb << " x 115 = " << money << " " << Variabili::getValuta() << ".\n";
                                output += "\nGiocatore " + std::to_string(players[i]->getID()) + " deve pagare " + std::to_string(counterCase) + " x 40 + "  + std::to_string(counterAlb) + " x 115 = "  + std::to_string(money) + " " + Variabili::getValuta() + ".\n";
                                pay = true;
                            }
                            else
                            {
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                std::cout << "\nGiocatore " << players[i]->getID() << " non ha ancora costruito case, quindi non paga nulla.\n";
                            }
                        break;
                        
                        case 5:
                            std::cout << "La banca riconosce un errore nel vostro estratto conto: incassate 200 " + Variabili::getValuta() + ".\n";
                            output += "La banca riconosce un errore nel vostro estratto conto: incassate 200 " + Variabili::getValuta() + ".\n";
                            deposit = true;
                            money = 200;
                        break;
                        
                        case 6:
                            std::cout << "Vi viene rimborsata la tassa sui redditi: incassate 50 " + Variabili::getValuta() + ".\n";
                            output += "Vi viene rimborsata la tassa sui redditi: incassate 50 " + Variabili::getValuta() + ".\n";
                            deposit = true;
                            money = 50;
                        break;
                        
                        case 7:
                            std::cout << "Dalla vendita di uno stock di merci ricavate 50 " + Variabili::getValuta() + ".\n";
                            output += "Dalla vendita di uno stock di merci ricavate 50 " + Variabili::getValuta() + ".\n";
                            deposit = true;
                            money = 50;
                        break;
                        
                        case 8:
                            std::cout << "Avete vinto il secondo premio in un concorso di bellezza: incassate 100 " + Variabili::getValuta() + ".\n";
                            output += "Avete vinto il secondo premio in un concorso di bellezza: incassate 100 " + Variabili::getValuta() + ".\n";
                            deposit = true;
                            money = 100;
                        break;
                        
                        case 9:
                            std::cout << "Maturano le cedole delle vostre azioni: ricevete 100 " + Variabili::getValuta() + ".\n";
                            output += "Maturano le cedole delle vostre azioni: ricevete 100 " + Variabili::getValuta() + ".\n";
                            deposit = true;
                            money = 100;
                        break;
                        
                        case 10:
                            std::cout << "Ereditate 100 " + Variabili::getValuta() + " da un lontano zio.\n";
                            output +=  "Ereditate 100 " + Variabili::getValuta() + " da un lontano zio.\n";
                            deposit = true;
                            money = 100;
                        break;
                        
                        case 11:
                            std::cout << "Maturano gli interessi della vostra assicurazione sulla vita: incassate 100 " + Variabili::getValuta() + ".\n";
                            output += "Maturano gli interessi della vostra assicurazione sulla vita: incassate 100 " + Variabili::getValuta() + ".\n";
                            deposit = true;
                            money = 100;
                        break;
                        
                        case 12:
                            std::cout << "Ricevete 25 " + Variabili::getValuta() + " per la vostra consulenza.\n";
                            output += "Ricevete 25 " + Variabili::getValuta() + " per la vostra consulenza.\n";
                            deposit = true;
                            money = 25;
                        break;
                        
                        case 13:
                            std::cout << "E' il vostro compleanno: ogni giocatore vi regala 50 " + Variabili::getValuta() + ".\n";
                            output += "E' il vostro compleanno: ogni giocatore vi regala 50 " + Variabili::getValuta() + ".\n";
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            for (int bob = 0; bob < players.size(); bob++)
                            {
                                if (players[bob]->getID() != players[i]->getID())
                                {
                                    tmpMoney = players[bob]->getMoney();
                                    try
                                    {
                                        players[bob]->Transaction(50, players[i], &output);
                                    }
                                    catch (const Giocatore::Player_Lost& e)
                                    {
                                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                        output += "Giocatore " + std::to_string(players[bob]->getID()) + " e' andato in bancarotta e ha perso.\n";
                                        players[bob]->getPosition()->removePlayer(players[bob]->getID());
                                    }
                                    guadagno += tmpMoney - players[bob]->getMoney();
                                }
                            }
                            aggiornaSchermo(T,players,output);
                            std::cout << "Giocatore " << players[i]->getID() << " ha incassato " << guadagno << " " + Variabili::getValuta() + ".\n";
                        break;
                        
                        case 14:
                            std::cout << "Andate in prigione direttamente e senza passare dal via.\n";
                            output += "Andate in prigione direttamente e senza passare dal via.\n";
                            players[i]->setJail(true);
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            // Movimento nella casella Transito/Prigione
                            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                            players[i]->goTo(T.prison);
                            T.prison->addPlayer(players[i]->getID()); // Aggiunta al tabellone del giocatore (nella casella dove si è spostato)
                            aggiornaSchermo(T,players,output);
                        break;
                        
                        case 15:
                            std::cout << "Andate avanti fino al via e ritirate " << Variabili::moneyPassaggioVia << " " << Variabili::getValuta() + ".\n";
                            output += "Andate avanti fino al via e ritirate " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            // Movimento nella casella Via
                            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                            players[i]->goTo("Via");
                            T.partenza->addPlayer(players[i]->getID()); // Aggiunta al tabellone del giocatore (nella casella dove si è spostato)
                            aggiornaSchermo(T,players,output);
                            std::cout << "Giocatore " << players[i]->getID() << " si e' mosso sul via e ha ritirato " <<  Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ".\n";
                            output += "Giocatore " + std::to_string(players[i]->getID()) + " si e' mosso sul via e ha ritirato " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        break;
                        
                        case 16:
                            std::cout << "Giocatore " << players[i]->getID() << " ha ottenuto la carta Uscita gratis di prigione.\n";
                            players[i]->setFreeExitPrisonProb(true);
                            nScelteP = 15;
                        break;
                    }
                }
                // Ho stampato ciò che c'era scritto sull'imprevisto/probabilità (ora se c'è da pagare o depositare si effettua l'operazione, i movimenti sono gestiti caso per caso nello switch)
                if (pay)
                {
                    try
                    {
                        players[i]->Transaction(money, nullptr, &output);
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        aggiornaSchermo(T,players,output);
                        std::cout << "Giocatore " << players[i]->getID() << " ha pagato " << money << " " + Variabili::getValuta() + ".\n";
                    }
                    catch (const Giocatore::Player_Lost& e)
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        output += "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
                        players[i]->getPosition()->removePlayer(players[i]->getID());
                        aggiornaSchermo(T,players,output);
                    }
                }
                if (deposit)
                {
                    players[i]->deposit(money);
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    aggiornaSchermo(T,players,output);
                    std::cout << "Giocatore " << players[i]->getID() << " ha incassato " << money << " " + Variabili::getValuta() + ".\n";
                }
            }

            // Controllo su che casella è finito il giocatore (anche se il giocatore era su una casella imprevisti che lo ha spostato)
            Casella_Terreno *pos1 = dynamic_cast<Casella_Terreno *>(players[i]->getPosition());
            Casella_Stazione *pos2 = dynamic_cast<Casella_Stazione *>(players[i]->getPosition());
            Casella_Tasse *pos3 = dynamic_cast<Casella_Tasse *>(players[i]->getPosition());
            Casella_Societa *pos4 = dynamic_cast<Casella_Societa*>(players[i]->getPosition());
    // Casella terreno
            if (pos1)
            {
                // Se la casella laterale non è di nessuno 
                if (pos1->getProprietario() == nullptr)
                {
                    std::cout << "Terreno della banca.\n";
                    output += "Terreno della banca.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    
                    int prezzo = pos1->getPrezzoTerreno();  // per l'output
                    std::string stampami = "";
                    // Il giocatore sceglie se comprare
                    wantToBuy = players[i]->choice(&stampami);
                    output += stampami;
                    // Stampo la scelta del giocatore
                    if (dynamic_cast<Computer*>(players[i]))
                    {
                        std::cout << stampami;
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    }
                    if (wantToBuy)
                    {
        // Pagamento
                        players[i]->buy();
                        std::cout << "Giocatore " << players[i]->getID() << " ha pagato " << prezzo << " " << Variabili::getValuta() << " e ha acquistato " << pos1->getName() << ".\n";
                        output += "Giocatore " + std::to_string(players[i]->getID()) + " ha pagato " + std::to_string(prezzo) + " " + Variabili::getValuta() + " e ha acquistato " + pos1->getName() + ".\n";
                        
                        // Se la casella appena acquistata (l'ultima inserita in _elenco_proprieta) è l'ultima di una famiglia di caselle simili
                        z=0;    //Contatore di altre caselle della stessa famiglia
                        // Scansiono il vettore _elenco_proprieta del giocatore per vedere se ci sono caselle simili
                        for (int h=0; h < players[i]->_elenco_proprieta.size(); h++)
                        {
                            if (pos1->getFamily() == players[i]->_elenco_proprieta[h]->getFamily())
                                z++;
                        }
                        // z può essere 1 (nessuna copia trovata), 2 (va bene solo se sono marroni o blu), 3 (ne ho 3 uguali quindi 100% una family)
                        if ((((pos1->getFamily() == 'M') || (pos1->getFamily() == 'B')) && (z==2)) || (z==3))
                        {
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            output += "Giocatore " + std::to_string(players[i]->getID()) + " ora ha tutti i terreni di colore " + pos1->getColor() + ".\n";

                            // Aggiorno tutte le proprietà che la famiglia è al completo
                            for (int h=0; h < players[i]->_elenco_proprieta.size(); h++)
                            {
                                if (pos1->getFamily() == players[i]->_elenco_proprieta[h]->getFamily())
                                {
                                    players[i]->_elenco_proprieta[h]->setCanBuy(true);
                                    // e le aggiungo al vettore di proprietà dove il giocatore può decidere se acquistarci case/alberghi
                                    players[i]->pushInToBuild(players[i]->_elenco_proprieta[h]);
                                }
                            }
                        }
                        aggiornaSchermo(T,players,output);
                    }
                    else
                    {
                        sold = false;
                    }
                }
            // Se il proprietario sono io, ma non posso decidere di costruire nulla perche non ho tutte le caselle simili
                else if (pos1->getProprietario() == players[i])
                {
                    std::cout << "Terreno del giocatore " << players[i]->getID() << ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    std::cout << "Non succede nulla.\n";
                }
                // Terreno di un altro giocatore (devo pagare)
                else
                {
                    std::cout << "Terreno del giocatore " << pos1->getProprietario()->getID() << ".\n";
                    output += "Terreno del giocatore " + std::to_string(pos1->getProprietario()->getID()) + ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));

                    // Stampo la rendita, ovvero quanto il giocatore deve spendere
                    if (!pos1->canBuy() && !pos1->isCasa1())
                    {
                        std::cout << "La rendita solo terreno e' di " << pos1->getAffitto() << " " + Variabili::getValuta() + ".\n";
                        output += "La rendita solo terreno e' di " + std::to_string(pos1->getAffitto()) + " " + Variabili::getValuta() + ".\n";
                    }
                    if (pos1->canBuy() && !pos1->isCasa1())
                    {
                        std::cout << "La rendita solo terreno e' di " << pos1->getAffitto()/2 << " x 2 = " << pos1->getAffitto() << " " + Variabili::getValuta() << " (giocatore " << pos1->getProprietario()->getID() << " possiede tutti i terreni di colore " << pos1->getColor() << ").\n";
                        output += "La rendita solo terreno e' di " + std::to_string(pos1->getAffitto()/2) + " x 2 = " + std::to_string(pos1->getAffitto()) + " " + Variabili::getValuta() + " (giocatore " +  std::to_string(pos1->getProprietario()->getID()) + " possiede tutti i terreni di colore " + pos1->getColor() + ").\n";
                    }
                    if (pos1->isCasa1() && !pos1->isCasa2())
                    {
                        std::cout << "La rendita con una casa e' di " << pos1->getAffitto() << " " + Variabili::getValuta() + ".\n";
                        output += "La rendita con una casa e' di " +  std::to_string(pos1->getAffitto()) + " " + Variabili::getValuta() + ".\n";
                    }
                    if (pos1->isCasa2() && !pos1->isCasa3())
                    {
                        std::cout << "La rendita con due case e' di " << pos1->getAffitto() << " " + Variabili::getValuta() + ".\n";
                        output += "La rendita con due case e' di " +  std::to_string(pos1->getAffitto()) + " " + Variabili::getValuta() + ".\n";
                    }
                    if (pos1->isCasa3() && !pos1->isCasa4())
                    {
                        std::cout << "La rendita con tre case e' di " << pos1->getAffitto() << " " + Variabili::getValuta() + ".\n";
                        output += "La rendita con tre case e' di " +  std::to_string(pos1->getAffitto()) + " " + Variabili::getValuta() + ".\n";
                    }
                    if (pos1->isCasa4() && !pos1->isAlbergo())
                    {
                        std::cout << "La rendita con quattro case e' di " << pos1->getAffitto() << " " + Variabili::getValuta() + ".\n";
                        output += "La rendita con quattro case e' di " +  std::to_string(pos1->getAffitto()) + " " + Variabili::getValuta() + ".\n";
                    }
                    if (pos1->isAlbergo())
                    {
                        std::cout << "La rendita con un albergo e' di " << pos1->getAffitto() << " " << Variabili::getValuta() << ".\n";
                        output += "La rendita con un albergo e' di " +  std::to_string(pos1->getAffitto()) + " " + Variabili::getValuta() + ".\n";
                    }
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));

        // Pagamento
                    try
                    {
                        players[i]->Transaction(pos1->getAffitto(), pos1->getProprietario(), &output);
                        // Il metodo Transfert gestisce l'eccezione lanciata in caso di mancanza di soldi eliminando
                        // il giocatore (se deve pagare 10 e ha solo 5 paga 5, arriva a 0 euro, perde tutte le proprietà
                        // (che tornano senza proprietario, nuovamente acquistabili) e viene settato che ha perso, quando
                        // tutti i giocatori avranno finito il loro turno il giocatore verrà rimosso dal vettore player, vedi codice uscito dal ciclo for).
                        std::cout << "Giocatore " << players[i]->getID() << " ha pagato " << pos1->getAffitto() << " " << Variabili::getValuta() << " al giocatore " << pos1->getProprietario()->getID() << ".\n";
                        output += "Giocatore " + std::to_string(players[i]->getID()) + " ha pagato " + std::to_string(pos1->getAffitto()) + " " + Variabili::getValuta() + " al giocatore " + std::to_string(pos1->getProprietario()->getID()) + ".\n";
                    }
                    catch (const Giocatore::Player_Lost& e)
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        output += "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
                        players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                    }
                    aggiornaSchermo(T,players,output);
                }
            }
    // Casella stazione
            else if (pos2)
            {
                if (pos2->getProprietario() == nullptr) // si può comprare
                {
                    std::cout << "Stazione della banca.\n";
                    output += "Stazione della banca.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    
                    std::string stampami = "";
                    // Il giocatore sceglie se comprare
                    wantToBuy = players[i]->choice(&stampami);
                    output += stampami;
                    // Stampo la scelta del giocatore
                    if (dynamic_cast<Computer*>(players[i]))
                    {
                        std::cout << stampami;
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    }
                    if (wantToBuy)
                    {
        // Pagamento
                        players[i]->buy();
                        aggiornaSchermo(T,players,output);
                        std::cout << "Giocatore " << players[i]->getID() << " ha pagato " << pos2->getPrezzo() << " " << Variabili::getValuta() << " e ha acquistato " << pos2->getName() << ".\n";
                        if (players[i]->_elenco_proprieta_st.size() > 1)
                        {
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            int size = players[i]->_elenco_proprieta_st.size();
                            std::string s = std::to_string(size);
                            if (size == 4)
                                s = "tutte e 4 le";
                            std::cout << "Giocatore " << players[i]->getID() << " ora possiede " << s << " stazioni.\n";
                        }
                    }
                    else
                    {
                        sold = false;
                    }
                }
                else if (pos2->getProprietario() == players[i]) // Il giocatore ne è il proprierario
                {
                    std::cout << "Stazione del giocatore " << players[i]->getID() << ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    std::cout << "Non succede nulla.\n";
                }
                else    // Se non è nullptr e non è del giocatore, allora è di un altro giocatore e bisogna pagare
                {
                    std::cout << "Stazione del giocatore " << pos2->getProprietario()->getID() << ".\n";
                    output += "Stazione del giocatore " + std::to_string(pos2->getProprietario()->getID()) + ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));

                    // Vedo quante altre stazioni ha l'altro nel suo elenco di proprietà
                    int toPay = pos2->getAffitto();
                    int j;
                    for (j=1; j < pos2->getProprietario()->_elenco_proprieta_st.size(); j++)
                    {
                        toPay = toPay * 2;
                    }
                    int size = pos2->getProprietario()->_elenco_proprieta_st.size();
                    std::string staz = " stazione";
                    int k = 1; // Fattore moltiplicativo
                    for (int s=1; s < size; s++)
                    {
                        staz = " stazioni";
                        k = k * 2;
                    }
                    std::cout << "L'affitto e' di " << toPay <<  " " << Variabili::getValuta() << ".\n";
                    output +=  "L'affitto e' di " + std::to_string(toPay) + " " + Variabili::getValuta() + ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    if (affittoRaddoppiato)
                    {
                        std::cout << "L'imprevisto fa si che l'affitto si raddoppi: " << toPay << " x 2 = " << toPay*2 << " " + Variabili::getValuta() + ".\n";
                        output += "L'imprevisto fa si che l'affitto si raddoppi: " + std::to_string(toPay) + " x 2 = "  + std::to_string(toPay*2) + " " + Variabili::getValuta() + ".\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        toPay = toPay*2;
                    }
        // Pagamento
                    try
                    {
                        players[i]->Transaction(toPay, pos2->getProprietario(), &output);
                        // Il metodo Transfert gestisce l'eccezione lanciata in caso di mancanza di soldi eliminando
                        // il giocatore (se deve pagare 10 e ha solo 5 paga 5, arriva a 0 euro, perde tutte le proprietà
                        // (che tornano senza proprietario, nuovamente acquistabili) e viene settato che ha perso, quando
                        // tutti i giocatori avranno finito il loro turno il giocatore verrà rimosso dal vettore player, vedi codice uscito dal ciclo for).
                        std::cout << "Giocatore " << players[i]->getID() << " ha pagato un affitto di " << toPay << " " << Variabili::getValuta() << " al giocatore " << pos2->getProprietario()->getID() << ".\n";
                        output += "Giocatore " + std::to_string(players[i]->getID()) + " ha pagato un affitto di " + std::to_string(toPay) + " " + Variabili::getValuta() + " al giocatore " + std::to_string(pos2->getProprietario()->getID()) + ".\n";
                    }
                    catch (const Giocatore::Player_Lost& e)
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        output += "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
                        players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                    }
                    aggiornaSchermo(T,players,output);
                }
            }
    // Casella tasse
            else if (pos3)
            {
        // Pagamento
                try
                {
                    players[i]->Transaction(pos3->getTax(), nullptr, &output);  //nullptr è la banca
                    std::cout << "Giocatore " << players[i]->getID() << " ha pagato " << pos3->getTax() << " " << Variabili::getValuta() << " di tasse.\n";
                    output += "Giocatore " + std::to_string(players[i]->getID()) + " ha pagato " + std::to_string(pos3->getTax()) + " " + Variabili::getValuta() + " di tasse.\n";
                }
                catch (const Giocatore::Player_Lost& e)
                {
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    output += "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
                    players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                }
                aggiornaSchermo(T,players,output);
            }
    // Casella Società
            else if (pos4)
            {
                if (pos4->getProprietario() == nullptr) // si può comprare
                {
                    std::cout << "Societa' della banca.\n";
                    output += "Societa' della banca.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    
                    // Il giocatore sceglie se comprare
                    std::string stampami = "";
                    wantToBuy = players[i]->choice(&stampami);
                    output += stampami;
                    // Stampo la scelta del giocatore
                    if (dynamic_cast<Computer*>(players[i]))
                    {
                        std::cout << stampami;
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    }
                    if (wantToBuy)
                    {
        // Pagamento
                        players[i]->buy();
                        aggiornaSchermo(T,players,output);
                        std::cout << "Giocatore " << players[i]->getID() << " ha pagato " << pos4->getPrezzo() << " " << Variabili::getValuta() << " e ha acquistato " << pos4->getName() << ".\n";
                        if (players[i]->_elenco_proprieta_soc.size() == 2)
                        {
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            std::cout << "Giocatore " << players[i]->getID() << " ora possiede tutte e 2 le societa'.\n";
                        }
                    }
                    else
                    {
                        sold = false;
                    }
                }
                else if (pos4->getProprietario() == players[i]) // Il giocatore ne è il proprierario
                {
                    std::cout << "Societa' del giocatore " << players[i]->getID() << ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    std::cout << "Non succede nulla.\n";
                }
                else    // Se non è nullptr e non è del giocatore, allora è di un altro giocatore e bisogna pagare
                {
                    std::cout << "Societa' del giocatore " << pos4->getProprietario()->getID() << ".\n";
                    output += "Societa' del giocatore " + std::to_string(pos4->getProprietario()->getID()) + ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));

                    // Vedo quante altre società ha l'altro nel suo elenco di proprietà
                    int toPay = tiro.first + tiro.second;
                    if (pos4->getProprietario()->_elenco_proprieta_soc.size() == 1)
                    {
                        std::cout << "Affitto = tirodadi (" << toPay << ") x 4 (con 1 societa') = " << toPay*4 << " " + Variabili::getValuta() + ".\n";
                        output += "Affitto = tirodadi (" + std::to_string(toPay) + ") x 4 (con 1 societa') = " + std::to_string(toPay*4) + " " + Variabili::getValuta() + ".\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        toPay = toPay*4;
                    }
                    if (pos4->getProprietario()->_elenco_proprieta_soc.size() == 2)
                    {
                        std::cout << "Affitto = tirodadi (" << toPay << ") x 10 (con 2 societa') = " << toPay*10 << " " + Variabili::getValuta() + ".\n";
                        output += "Affitto = tirodadi (" + std::to_string(toPay) + ") x 10 (con 2 societa') = " + std::to_string(toPay*10) + " " + Variabili::getValuta() + ".\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        toPay = toPay*10;
                    }
                    if (affittoRaddoppiato)
                    {
                        std::cout << "L'imprevisto fa si che l'affitto si raddoppi: " << toPay << " x 2 = " << toPay*2 << " " + Variabili::getValuta() + ".\n";
                        output += "L'imprevisto fa si che l'affitto si raddoppi: " + std::to_string(toPay) + " x 2 = "  + std::to_string(toPay*2) + " " + Variabili::getValuta() + ".\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        toPay = toPay*2;
                    }
        // Pagamento
                    try
                    {
                        players[i]->Transaction(toPay, pos4->getProprietario(), &output);
                        // Il metodo Transfert gestisce l'eccezione lanciata in caso di mancanza di soldi eliminando
                        // il giocatore (se deve pagare 10 e ha solo 5 paga 5, arriva a 0 euro, perde tutte le proprietà
                        // (che tornano senza proprietario, nuovamente acquistabili) e viene settato che ha perso, quando
                        // tutti i giocatori avranno finito il loro turno il giocatore verrà rimosso dal vettore player, vedi codice uscito dal ciclo for).
                        std::cout << "Giocatore " << players[i]->getID() << " ha pagato un affitto di " << toPay << " " << Variabili::getValuta() << " al giocatore " << pos4->getProprietario()->getID() << ".\n";
                        output += "Giocatore " + std::to_string(players[i]->getID()) + " ha pagato un affitto di " + std::to_string(toPay) + " " + Variabili::getValuta() + " al giocatore " + std::to_string(pos4->getProprietario()->getID()) + ".\n";
                    }
                    catch (const Giocatore::Player_Lost& e)
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        output += "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
                        players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                    }
                    aggiornaSchermo(T,players,output);
                }
            }
    // Casella angolare
            else if (!pos5) // Se non è su una probabilità/imprevisto
            {
                if (players[i]->getPosition()->getNameOutput() == "ALT")
                {
                    std::cout << "Giocatore " << players[i]->getID() << " va in prigione.\n";
                    output += "Giocatore " + std::to_string(players[i]->getID()) + " va in prigione.\n";
                    players[i]->setJail(true);
                    // Movimento sul tabellone (nella casella Transito/Prigione)
                    players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                    players[i]->goTo(T.prison);
                    T.prison->addPlayer(players[i]->getID()); // Aggiunta al tabellone del giocatore (nella casella dove si è spostato)
                    aggiornaSchermo(T,players,output);
                }
                else if (!players[i]->isInJail())
                {
                    std::cout << "Non succede nulla.\n";
                }
            }
    
// Fine turno di un giocatore
            if (sold)
                std::this_thread::sleep_for(std::chrono::seconds(pausa));
            std::cout << "Fine " << turno << " del giocatore " << players[i]->getID() << ".\n";
            std::cout << muro;
            std::this_thread::sleep_for(std::chrono::seconds(pausa));
        
// Verifiche per capire se continuare a giocare
            // Verifica se il giocatore è ancora in gioco
            for (int k=0; k < players.size(); k++)
            {
                if (!players[k]->isInGame()) // Se il giocatore non è più in game
                {                            // lo tolgo dal vettore di players
                    players.erase(players.begin() + k);
                }
            }
            if (players.size() == 1)    // C'è 1 solo giocatore, il gioco si ferma e vince l'ultimo rimasto
            {
                winner.push_back(players[0]->getID()); // Per uscire dal while
                break;                        // Per uscire dal for
            }

// Asta in caso il giocatore non abbia voluto acquistare un terreno
            if(!sold)
            {
                std::string hey = "\n" + muro;
                aggiornaSchermo(T,players,hey);
                hey += "\nGiocatore " + std::to_string(players[i]->getID()) + " non ha acquistato " + players[i]->getPosition()->getName() + ", quindi inizia l'asta.";
                std::cout << "\nGiocatore " << players[i]->getID() << " non ha acquistato " << players[i]->getPosition()->getName() << ", quindi inizia l'asta.";
                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                std::vector<Giocatore *> playersAsta;
                bool noOneWants = true;
                int prezzo = 0;
                if (pos2)
                    prezzo = pos2->getPrezzo()/2;
                if (pos1)
                    prezzo = pos1->getPrezzoTerreno()/2;
                if (pos4)
                    prezzo = pos4->getPrezzo()/2;

                for (int w=0; w<players.size(); w++)
                {
                    if (players[i]->getID() != players[w]->getID()) // Il giocatore che non ha voluto comprare la casella non partecipa all'asta
                    {
                        if (players[w]->getMoney() >= prezzo + minimaOffertaAsta) // il giocatore partecipa all'asta se ha abbastanza soldi
                        {
                            playersAsta.push_back(players[w]);
                        }
                        else 
                        {
                            std::cout << "\nGiocatore " << players[w]->getID() << " non ha abbastanza "<< Variabili::getValuta() << " per partecipare all'asta.";
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        }
                    }
                }
                if (!playersAsta.empty())
                {
                    do
                    {
                        std::cout << "\nAll'asta partecipano i seguenti giocatori:";
                        hey += "\nAll'asta partecipano i seguenti giocatori:";
                        for (int z=0; z < playersAsta.size(); z++)
                        {
                            std::cout << " " << playersAsta[z]->getID();
                            hey += " " + std::to_string(playersAsta[z]->getID());
                        }
                        std::cout << ".\n";
                        hey += ".\n";
                        if (noOneWants)
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        std::cout << "Proprieta' in vendita: " << players[i]->getPosition()->getName() << ".\n";
                        hey += "Proprieta' in vendita: " + players[i]->getPosition()->getName() + ".\n";
                        if (noOneWants)
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        std::cout << "Prezzo attuale: " << prezzo << ".\n";
                        hey += "Prezzo attuale: " + std::to_string(prezzo) + ".\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        for (int w=0; w < playersAsta.size(); w++)
                        {
                            int prezzotmp=prezzo;
                            bool lessMoneyPossibile = playersAsta.size()==1 || noOneWants;  // Se sono l'ultimo o se è il primo turno, i computer rilanciano del minimo sindacale
                            if(playersAsta[w]->partecipaAsta(&prezzo, players[i]->getPosition(), lessMoneyPossibile, minimaOffertaAsta)) // Il giocatore decide se partecipare
                            {
                                noOneWants = false;
                                minimaOffertaAsta = minimaOffertaAsta + 10;
                                std::cout << "Giocatore " << playersAsta[w]->getID() << " rilancia di " << (prezzo-prezzotmp) << " " << Variabili::getValuta() << ".\n";
                                hey += "Giocatore " + std::to_string(playersAsta[w]->getID()) + " rilancia di " + std::to_string(prezzo-prezzotmp) +  " " + Variabili::getValuta() + ".\n";
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                std::cout << "Prezzo attuale: " << prezzo << ".\n";
                                hey += "Prezzo attuale: " + std::to_string(prezzo) + ".\n";
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            }
                            else
                            {
                                std::cout << "Giocatore " << playersAsta[w]->getID() << " abbandona l'asta.\n";
                                hey += "Giocatore " + std::to_string(playersAsta[w]->getID()) + " abbandona l'asta.\n";
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                playersAsta.erase(playersAsta.begin() + w);
                                if (noOneWants || playersAsta.size() != 1)
                                    w--;
                            }
                        }
                        if (playersAsta.size() >= 2)
                        {
                            hey ="\n" + muro;
                            aggiornaSchermo(T, players,hey);
                        }

                    }while (playersAsta.size() >= 2);
                    if (!noOneWants) // Se almeno un giocatore ha dimostrato interesse nell'acquistare il terreno
                    {                // Stampo chi ha acquistato la proprietà
                        std::cout << "Nell'asta e' rimasto solo il giocatore " << playersAsta[0]->getID() << ".\n";
                        hey += "Nell'asta e' rimasto solo il giocatore " + std::to_string(playersAsta[0]->getID()) + ".\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        minimaOffertaAsta = 10; // Reset per la prossima asta
                        try
                        {
                            playersAsta[0]->buy(prezzo, players[i]->getPosition());
                            std::cout << "Giocatore " << playersAsta[0]->getID() << " ha pagato " << prezzo << " " << Variabili::getValuta() << " e ha acquistato " << players[i]->getPosition()->getName() << ".\n";
                            hey += "Giocatore " + std::to_string(playersAsta[0]->getID()) + " ha pagato " + std::to_string(prezzo) + " " + Variabili::getValuta() + " e ha acquistato " + players[i]->getPosition()->getName() + ".\n";
                            if (playersAsta[0]->_elenco_proprieta_st.size() > 1 && pos2) // Se ho acquistato una casella stazione
                            {
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                int size = playersAsta[0]->_elenco_proprieta_st.size();
                                std::string s = std::to_string(size);
                                if (size == 4)
                                    s = "tutte e 4 le";
                                hey += "Giocatore " + std::to_string(playersAsta[0]->getID()) + " ora possiede " + s + " stazioni.\n";
                            }
                            if (playersAsta[0]->_elenco_proprieta_soc.size() == 2 && pos4) // Se ho acquistato una casella società
                            {
                                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                hey += "Giocatore " + std::to_string(playersAsta[0]->getID()) + " ora possiede tutte e 2 le societa'.\n";
                            }
                            if (pos1)   // Se ho acquistato una casella terreno
                            { // Se è l'ultima di una famiglia di caselle simili
                                z=0;    //Contatore di altre caselle della stessa famiglia
                                // Scansiono il vettore _elenco_proprieta del giocatore per vedere se ci sono caselle simili
                                for (int h=0; h < playersAsta[0]->_elenco_proprieta.size(); h++)
                                {
                                    if (pos1->getFamily() == playersAsta[0]->_elenco_proprieta[h]->getFamily())
                                        z++;
                                }
                                // z può essere 1 (nessuna copia trovata), 2 (va bene solo se sono marroni o blu), 3 (ne ho 3 uguali quindi 100% una family)
                                if ((((pos1->getFamily() == 'M') || (pos1->getFamily() == 'B')) && (z==2)) || (z==3))
                                {
                                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                    hey += "Giocatore " + std::to_string(playersAsta[0]->getID()) + " ora ha tutti i terreni di colore " + pos1->getColor() + ".\n";
                                    // Aggiorno tutte le proprietà che la famiglia è al completo
                                    for (int h=0; h < playersAsta[0]->_elenco_proprieta.size(); h++)
                                    {
                                        if (pos1->getFamily() == playersAsta[0]->_elenco_proprieta[h]->getFamily())
                                        {
                                            playersAsta[0]->_elenco_proprieta[h]->setCanBuy(true);
                                            // e le aggiungo al vettore di proprietà dove il giocatore può decidere se acquistarci case/alberghi
                                            playersAsta[0]->pushInToBuild(playersAsta[0]->_elenco_proprieta[h]);
                                        }
                                    }
                                }
                            }
                            aggiornaSchermo(T, players,hey);
                        }
                        catch (const Giocatore::Not_Enough_Money& e)    // non dovrebbe mai lanciarla, ma meglio gestirla ...
                        {
                            std::cout << "Tentativo di acquisto non riuscito, " << Variabili::getValuta() << " non sufficenti.\n";
                        }
                    }
                    else
                        std::cout << "Nessun giocatore ha mostrato interersse per l'acquisto.\n";
                }
                else
                    std::cout << "\nNessun giocatore ha abbastanza " << Variabili::getValuta() << " per partecipare all'asta.\n";
                    
                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                std::cout << "Fine asta.\n" << muro;
                std::this_thread::sleep_for(std::chrono::seconds(pausa));
            }

// Verifiche sul tiro dei dadi del giocatore una volta finito il suo turno (possibile turno extra)
            // Se il giocatore ha fatto un doppio numero tirando i dadi
            if (tiro.first == tiro.second)
            {
                players[i]->incrementNTurno();
                i--;    // in questo caso ha diritto ad un turno extra
            }
            else if (players[i]->getNTurno() > 1)   // Se è in un turno extra (secondo o terzo) e ha fatto un numero di dadi diverso
                players[i]->resetNTurno();  // Il turno del giocatore torna ad 1 e si passa al giocatore successivo
        }
    }

// Stampo chi ha vinto
    aggiornaSchermo(T,players,"\n" + muro + "\nGioco finito");
    if (j >= nMaxTurni)
       std::cout << " per raggiungimento limite massimo di turni";

    if (winner.size() == 1)
    {
        std::cout << ".\nHa vinto il giocatore " << players[0]->getID();
    }
    else    // if (winner.size() > 1), se non è 1 è per forza maggiore di uno, perchè quando esco dal game c'è sempre almeno un push
    {
        std::cout << ".\nVittoria a parimerito, hanno vinto i giocatori";
        for (int i=0; i < winner.size(); i++)
        {
            std::cout << " " << winner[i];
        }
        std::cout << ".";
    }
    if (winner.size() == 0) // Non dovrebbe succedere MAI
    {
        std::cout << ".\nVincitore non chiaro, qualcosa è andato storto nella chiusura del game.";
    }
    std::cout << "\n" << muro;

    return 0;
}

std::pair<int,int> getDiceRoll()
{
    // Inizializza il generatore di numeri casuali con un seed basato sull'orologio di sistema
    std::random_device rd;
    std::mt19937 gen(rd());

    // Definisci la distribuzione uniforme per generare numeri tra 1 e 6
    std::uniform_int_distribution<int> distr(1, Variabili::nFacceDado);

    return std::make_pair(distr(gen),distr(gen));
}

void startingOrder(std::vector<Giocatore *> &players)
{
    std::vector<int> lanci;
    for (int i = 0; i < players.size(); i++) // assegno a ogni elemento un lancio di dadi
    {
        int l;
        std::pair<int,int> tiro = getDiceRoll();
        l = tiro.first + tiro.second;
        std::cout << "Giocatore " << players[i]->getID() << " ha tirato i dadi e ha fatto " << tiro.first << " + " << tiro.second << " = " << (tiro.first + tiro.second) << ".\n";
        std::this_thread::sleep_for(std::chrono::seconds(pausa));
        lanci.push_back(l);
    }

    // utilizzo l'algoritmo bubble sort
    bool isOrder = false;
    while (!isOrder)
    {
        isOrder = true;
        for (int i = 1; i < lanci.size(); i++)
        {
            if (lanci[i - 1] < lanci[i])
            {
                int tmp = lanci[i - 1];
                lanci[i - 1] = lanci[i];
                lanci[i] = tmp;
                Giocatore *gTmp = players[i - 1];
                players[i - 1] = players[i];
                players[i] = gTmp;
                isOrder = false;
            }
        }
    }

    // Vado a Verificare se ci sono doppioni
    std::vector<std::vector<int>> indicidoppioni; // Matrice che conterrà gli indici per accedere ad eventuali giocatori di players con un tiro di dadi uguale
    std::vector<int> v;
    bool to_push = false;
    for (int i = 1; i < lanci.size(); i++)
    {
        if (lanci[i - 1] == lanci[i]) // Controllo se il giocatore i ed il precedente hanno fatto lo stesso numero di dadi
        {
            if (v.size() == 0) // Se il vettore è vuoto, allora devo inserire 2 elementi
            {
                v.push_back(i - 1);
            }

            v.push_back(i);
            
            if ((i == lanci.size() - 1) && (v.size() > 0)) // Per gestire anche il caso in cui almeno gli ultimi 2 giocatori hanno fatto lo stesso numero
            {
                to_push = true;
            }
        }
        else
        {
            if (v.size() > 0)
                to_push = true;
        }

        if (to_push)
        {
            indicidoppioni.push_back(v);
            v.clear();
            to_push = false;
        }
    }

    std::vector<Giocatore *> tmp;
    std::string output = "";
// Se c'erano dei giocatori che hanno fatto un numero di dadi =, ritirano i dadi (tramite ricorsione)
    for (int i = 0; i < indicidoppioni.size(); i++)
    {
        output += "I giocatori ";
        for (int k = 0; k < indicidoppioni[i].size(); k++)
        {
            output += std::to_string(players[indicidoppioni[i][k]]->getID()) + " ";
            tmp.push_back(players[indicidoppioni[i][k]]);
        }
        output += "hanno fatto lo stesso numero, quindi ritirano i dadi.\n";
        std::cout << output;
        std::this_thread::sleep_for(std::chrono::seconds(pausa));
        startingOrder(tmp);
        // Ora che il vettore tmp è ordinato aggiorno il vettore players
        for (int k = 0; k < indicidoppioni[i].size(); k++)
        {
            players[indicidoppioni[i][k]] = tmp[k];
        }
    }
}

void aggiornaSchermo(Tabellone t, std::vector<Giocatore *> p, std::string s)
{
    clearScreen();

    // Stampa tabellone
    std::cout << t;
 
    // Stampa lista giocatori
    for (int i = 0; i < p.size(); i++)
    {
        std::cout << *p[i];
        std::cout << "\nGiocatore " << p[i]->getID() << " ha: " << p[i]->getMoney() << " " << Variabili::getValuta() << ".\n";
    }

    // Ri-stampa eventi di gioco (già stampati precedentemente)
    std::cout << s;
}