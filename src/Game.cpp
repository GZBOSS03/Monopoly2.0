#include "../include/Human.h"
#include "../include/Computer.h"
#include "../include/Tabellone.h"

std::pair<int,int> getDiceRoll();                   // restituisce la somme di due numeri casuali tra 1 e 6
void startingOrder(std::vector<Giocatore *> &players); // ordina i players in base ai lanci dei dadi
void aggiornaSchermo(Tabellone, std::vector<Giocatore *>, std::string);
void clearScreen() { system("cls"); }
int tiriIniziali = 0;
int pausa = 2;  // Pausa che il programma fa in output tra una stampa e la successiva

// Mode passata come argomento da terminale
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
        if (j==20)
        {
            pausa = 2;
        }
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
                    // Se il giocatore è in prigione e ci ha passato già nMaxTurniPrigione turni
                    if (players[i]->getTurniJail() == Variabili::nMaxTurniPrigione)
                    {
                        players[i]->setJail(false);
                        std::cout << "Giocatore " << players[i]->getID() << " ha passato " << Variabili::nMaxTurniPrigione << " turni in prigione, quindi esce pagando " + std::to_string(Variabili::moneyUscitaPrigione) + " " + Variabili::getValuta() + " e tira i dadi per muoversi.\n";
                        output += "Giocatore " + std::to_string(players[i]->getID()) + " ha passato " + std::to_string(Variabili::nMaxTurniPrigione) + " turni in prigione, quindi esce pagando " + std::to_string(Variabili::moneyUscitaPrigione) + " " + Variabili::getValuta() + " e tira i dadi per muoversi.\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        try
                        {
                            players[i]->Transaction(50, nullptr, &output);
                        }
                        catch (const Giocatore::Player_Lost& e)
                        {
                            std::cout << e.s;
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            output += e.s + "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
                            T.prison->removePlayer(players[i]->getID());
                        }
                        aggiornaSchermo(T,players,output);
                    }
                    else
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
            std::cout << "Giocatore " << players[i]->getID() << " si e' mosso su " << players[i]->getPosition()->getName() << ".\n";
            output += "Giocatore " + std::to_string(players[i]->getID()) + " si e' mosso su " + players[i]->getPosition()->getName() + ".\n";
        
            // Stampo il passaggio dal via (avvenuto nel move)
            if (players[i]->getMoney() > tmpMoney)
            {
                std::this_thread::sleep_for(std::chrono::seconds(pausa));
                std::cout << "Giocatore " << players[i]->getID() << " e' passato dal via e ha ritirato " <<  Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ".\n";
                output += "Giocatore " + std::to_string(players[i]->getID()) + " e' passato dal via e ha ritirato " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
            }
            std::this_thread::sleep_for(std::chrono::seconds(pausa));

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
    // Probabilità
                if (pos5->getNameOutput() == "?P?")
                {
                    std::uniform_int_distribution<int> distr(1,nScelteP);
                    int n = distr(gen);
                    // per simulare la pescata di una carta dal mazzetto, poi la carta si rimette in mezzo e si mescola.
                    switch (n)
                    {
                        case 1:
                            std::cout << "Ricevete la parcella del dottore: pagate 50 " + Variabili::getValuta() + ".\n";
                            output += "Ricevete la parcella del dottore: pagate 50 " + Variabili::getValuta() + ".\n";
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
                                std::cout << " Giocatore " << players[i]->getID() << " possiede " << counterCase << " case e " << counterAlb << " alberghi, quindi deve pagare " << counterCase << "x40 + " << counterAlb << "x115 = " << money << " " << Variabili::getValuta() << ".\n";
                                output += " Giocatore " + std::to_string(players[i]->getID()) + " possiede " + std::to_string(counterCase) + " case e " + std::to_string(counterAlb) + " alberghi, quindi deve pagare " + std::to_string(counterCase) + "x40 + "  + std::to_string(counterAlb) + "x115 = "  + std::to_string(money) + " " + Variabili::getValuta() + ".\n";
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
                                        std::cout << e.s;
                                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                        output += e.s + "Giocatore " + std::to_string(players[bob]->getID()) + " e' andato in bancarotta e ha perso.\n";
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
                            std::cout << "Giocatore " << players[i]->getID() << " si e' mosso su " << players[i]->getPosition()->getName() << ".\n";
                        break;
                        
                        case 15:
                            std::cout << "Andate avanti fino al via e ritirate " << Variabili::moneyPassaggioVia << " " << Variabili::getValuta() + ".\n";
                            output += "Andate avanti fino al via e ritirate " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ".\n";
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            // Movimento nella casella Via
                            players[i]->getPosition()->removePlayer(players[i]->getID()); // Rimozione dal tabellone del giocatore dalla casella vecchia
                            players[i]->goTo(T.partenza);
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
                    if (pay)
                    {
                        try
                        {
                            players[i]->Transaction(money, nullptr, &output);
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            aggiornaSchermo(T,players,output);
                            std::cout << "Giocatore " << players[i]->getID() << " ha pagato " << money << ".\n";
                        }
                        catch (const Giocatore::Player_Lost& e)
                        {
                            std::cout << e.s;
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            output += e.s + "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
                            players[i]->getPosition()->removePlayer(players[i]->getID());
                            aggiornaSchermo(T,players,output);
                        }
                    }
                    if (deposit)
                    {
                        players[i]->deposit(money);
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        aggiornaSchermo(T,players,output);
                        std::cout << "Giocatore " << players[i]->getID() << " ha incassato " << money << ".\n";
                    }
                }
    // Imprevisti
                else
                {
                    std::uniform_int_distribution<int> distr(1,nScelteI);
                    int n = distr(gen);
                    tmpMoney = players[i]->getMoney();  // Per intercettare il passaggio dal via e stamparlo
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
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        break;
                        
                        case 2:
                            std::cout << "Andate fino a Parco Della Vittoria (se passate dal Via ritirate " << Variabili::moneyPassaggioVia << " " << Variabili::getValuta() << ").\n";
                            output += "Andate fino a Parco Della Vittoria (se passate dal Via ritirate " + std::to_string(Variabili::moneyPassaggioVia) + " " + Variabili::getValuta() + ").\n";
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
                                std::cout << " Giocatore " << players[i]->getID() << " possiede " << counterCase << " case e " << counterAlb << " alberghi, quindi deve pagare " << counterCase << "x25 + " << counterAlb << "x100 = " << money << " " << Variabili::getValuta() << ".\n";
                                output += " Giocatore " + std::to_string(players[i]->getID()) + " possiede " + std::to_string(counterCase) + " case e " + std::to_string(counterAlb) + " alberghi, quindi deve pagare " + std::to_string(counterCase) + "x25 + "  + std::to_string(counterAlb) + "x100 = "  + std::to_string(money) + " " + Variabili::getValuta() + ".\n";
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
                            std::cout << "Avanzate fino alla stazione ferroviaria piu vicina:\nse e' libera potete acquistarla dalla banca;\nse e' di un altro giocatore dovete pagargli il doppio di cio' che gli spetta normalmente.\n";
                            output += "Avanzate fino alla stazione ferroviaria piu vicina:\nse e' libera potete acquistarla dalla banca;\nse e' di un altro giocatore dovete pagargli il doppio di cio' che gli spetta normalmente.\n";
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
                            std::cout << "Avanzate fino alla societa' piu vicina:\nse e' libera potete acquistarla dalla banca;\nse e' di un altro giocatore dovete pagargli il doppio di cio' che gli spetta normalmente.\n";
                            output += "Avanzate fino alla societa' piu vicina:\nse e' libera potete acquistarla dalla banca;\nse e' di un altro giocatore dovete pagargli il doppio di cio' che gli spetta normalmente.\n";
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
                                        std::cout << e.s;
                                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                                        output += e.s + "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
                                        players[i]->getPosition()->removePlayer(players[i]->getID());
                                    }
                                }
                            }
                            aggiornaSchermo(T,players,output);
                            if (players[i]->isInGame())
                                std::cout << "Giocatore " << players[i]->getID() << " ha pagato 150 " + Variabili::getValuta() + ".\n";
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
                            players[i]->goTo(T.partenza);
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
                    if (pay)
                    {
                        try
                        {
                            players[i]->Transaction(money, nullptr, &output);
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            aggiornaSchermo(T,players,output);
                            std::cout << "Giocatore " << players[i]->getID() << " ha pagato " << money << ".\n";
                        }
                        catch (const Giocatore::Player_Lost& e)
                        {
                            std::cout << e.s;
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            output += e.s + "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
                            players[i]->getPosition()->removePlayer(players[i]->getID());
                            aggiornaSchermo(T,players,output);
                        }
                    }
                    if (deposit)
                    {
                        players[i]->deposit(money);
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        aggiornaSchermo(T,players,output);
                        std::cout << "Giocatore " << players[i]->getID() << " ha incassato " << money << ".\n";
                    }
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
                // Se il giocatore-iesimo è il proprietario e c'è un albergo, il giocatore non può fare nulla
                if ((pos1->getProprietario() == players[i]) && (pos1->isAlbergo()))
                {
                    std::cout << "Terreno del giocatore " << players[i]->getID() << ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    std::cout << "Giocatore " << players[i]->getID() << " ha gia' costruito tutto il possibile.\n";
                }
                // Se la casella laterale non è di nessuno 
                // o è del giocatore, è senza albergo (ci si può ancora costruire) e il giocatore ha tutte le caselle di quel colore
                else if ((pos1->getProprietario() == nullptr) || ((pos1->getProprietario() == players[i]) && (!pos1->isAlbergo()) && (pos1->canBuy())))
                {
                    std::string prop = "della banca";
                    if (pos1->getProprietario()) 
                        prop = "del giocatore " + std::to_string(pos1->getProprietario()->getID());
                    std::cout << "Terreno " << prop << ".\n";
                    output += "Terreno " + prop + ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    
                    int prezzo = pos1->getPrezzo();  // per l'output
                    std::string stampami = "";
                    // Il giocatore sceglie se comprare
                    bool wantToBuy = players[i]->choice(&stampami);
                    output += stampami;
                    // Stampo la scelta del giocatore
                    if (dynamic_cast<Computer*>(players[i]))
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        std::cout << stampami;
                    }
                    if (wantToBuy)
                    {
        // Pagamento
                        players[i]->buy();
                        if (pos1->isCasa1())    // c'è una casa dopo l'acquisto, quindi dovro stampare ...
                        {
                            if (pos1->isAlbergo()) // se dopo l'acquisto c'è un albergo
                            {
                                output += "Giocatore " + std::to_string(players[i]->getID()) + " ha pagato " + std::to_string(prezzo) + " " + Variabili::getValuta() + " e ha acquistato un albergo in tutte le proprieta' di colore " + pos1->getColor() + ".\n";
                            }
                            else  // se dopo l'acquisto non c'è un albergo, allora c'è una casa
                            {
                                output += "Giocatore " + std::to_string(players[i]->getID()) + " ha pagato " + std::to_string(prezzo) + " " + Variabili::getValuta() + " e ha acquistato una casa in tutte le proprieta' di colore " + pos1->getColor() + ".\n";
                            }
                        }
                        else
                        {
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
                                        players[i]->_elenco_proprieta[h]->setCanBuy(true);
                                }
                            }
                        }
                        aggiornaSchermo(T,players,output);
                    }
                    else
                    {
                        if(pos1->getProprietario() == nullptr)
                            sold = false;
                    }
                }
            // Se il proprietario sono io, ma non posso decidere di costruire nulla perche non ho tutte le caselle simili
                else if ((pos1->getProprietario() == players[i]) && (!pos1->isAlbergo()) && (!pos1->canBuy()))
                {
                    std::cout << "Terreno del giocatore " << players[i]->getID() << ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    std::cout << "Giocatore " << players[i]->getID() << " non ha ancora tutti i terreni di colore " << pos1->getColor() << ", quindi non puo' costruire case.\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    std::cout << "Non succede nulla.\n";
                }
                // Terreno di un altro giocatore (devo pagare)
                else if(pos1->getProprietario() != players[i] && pos1->getProprietario() != nullptr) // questo if non servirebbe per la catena di if/else, ma meglio metterlo
                {
                    std::cout << "Terreno del giocatore " << pos1->getProprietario()->getID() << ".\n";
                    output += "Terreno del giocatore " + std::to_string(players[i]->getID()) + ".\n";
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));

                    // Stampo la rendita, ovvero quanto il giocatore deve spendere
                    if (!pos1->canBuy())
                    {
                        std::cout << "La rendita solo terreno e' di " << pos1->getAffitto() << ".\n";
                        output += "La rendita solo terreno e' di " +  std::to_string(pos1->getAffitto()) + ".\n";
                    }
                    if (pos1->canBuy() && !pos1->isCasa1())
                    {
                        
                        std::cout << "La rendita solo terreno e' di " << pos1->getAffitto()/2 << " x 2 = " << pos1->getAffitto() << " (giocatore " << pos1->getProprietario()->getID() << " possiede tutti i terreni di colore " << pos1->getColor() << ").\n";
                        output += "La rendita solo terreno e' di " + std::to_string(pos1->getAffitto()/2) + " x 2 = " + std::to_string(pos1->getAffitto()) + " (giocatore " +  std::to_string(pos1->getProprietario()->getID()) + " possiede tutti i terreni di colore " + pos1->getColor() + ").\n";
                    }
                    if (pos1->isCasa1() && !pos1->isCasa2())
                    {
                        std::cout << "La rendita con una casa e' di " << pos1->getAffitto() << ".\n";
                        output += "La rendita con una casa e' di " +  std::to_string(pos1->getAffitto()) + ".\n";
                    }
                    if (pos1->isCasa2() && !pos1->isCasa3())
                    {
                        std::cout << "La rendita con due case e' di " << pos1->getAffitto() << ".\n";
                        output += "La rendita con due case e' di " +  std::to_string(pos1->getAffitto()) + ".\n";
                    }
                    if (pos1->isCasa3() && !pos1->isCasa4())
                    {
                        std::cout << "La rendita con tre case e' di " << pos1->getAffitto() << ".\n";
                        output += "La rendita con tre case e' di " +  std::to_string(pos1->getAffitto()) + ".\n";
                    }
                    if (pos1->isCasa4() && !pos1->isAlbergo())
                    {
                        std::cout << "La rendita con quattro case e' di " << pos1->getAffitto() << ".\n";
                        output += "La rendita con quattro case e' di " +  std::to_string(pos1->getAffitto()) + ".\n";
                    }
                    if (pos1->isAlbergo())
                    {
                        std::cout << "La rendita con un albergo e' di " << pos1->getAffitto() << ".\n";
                        output += "La rendita con un albergo e' di " +  std::to_string(pos1->getAffitto()) + ".\n";
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
                        std::cout << e.s;
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        output += e.s + "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
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
                    bool wantToBuy = players[i]->choice(&stampami);
                    output += stampami;
                    // Stampo la scelta del giocatore
                    if (dynamic_cast<Computer*>(players[i]))
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        std::cout << stampami;
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
                    if (pos2->getProprietario()->_elenco_proprieta_st.size()>1)
                    {
                        int size = pos2->getProprietario()->_elenco_proprieta_st.size();
                        std::string staz = " stazione";
                        int k = 1; // Fattore moltiplicativo
                        for (int s=1; s < size; s++)
                        {
                            staz = " stazioni";
                            k = k * 2;
                        }
                        std::cout << "L'affitto con " << size << staz << " e' di " << pos2->getAffitto() << " x " << k << " = " << toPay << ".\n";
                        output +=  "L'affitto con " + std::to_string(size) + staz + " e' di " + std::to_string(pos2->getAffitto()) + " x " + std::to_string(k) + " = " + std::to_string(toPay) + ".\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    }
                    if (affittoRaddoppiato)
                    {
                        std::cout << "L'imprevisto fa si che l'affitto si raddoppi: " << toPay << " x 2 = " << toPay*2 << ".\n";
                        output += "L'imprevisto fa si che l'affitto si raddoppi: " + std::to_string(toPay) + " x 2 = "  + std::to_string(toPay*2) + ".\n";
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
                        std::cout << e.s;
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        output += e.s + "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
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
                    std::cout << e.s;
                    std::this_thread::sleep_for(std::chrono::seconds(pausa));
                    output += e.s + "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
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
                    bool wantToBuy = players[i]->choice(&stampami);
                    output += stampami;
                    // Stampo la scelta del giocatore
                    if (dynamic_cast<Computer*>(players[i]))
                    {
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        std::cout << stampami;
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

                    // Vedo quante altre stazioni ha l'altro nel suo elenco di proprietà
                    int toPay = tiro.first + tiro.second;
                    if (pos4->getProprietario()->_elenco_proprieta_soc.size() == 1)
                    {
                        std::cout << "Affitto = tirodadi(" << toPay << ") x 4 (con 1 societa') = " << toPay*4 << ".\n";
                        output += "Affitto = tirodadi(" + std::to_string(toPay) + ") x 4 (con 1 societa') = " + std::to_string(toPay*4) + ".\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        toPay = toPay*4;
                    }
                    if (pos4->getProprietario()->_elenco_proprieta_soc.size() == 2)
                    {
                        std::cout << "Affitto = tirodadi(" << toPay << ") x 10 (con 2 societa') = " << toPay*10 << ".\n";
                        output += "Affitto = tirodadi(" + std::to_string(toPay) + ") x 10 (con 2 societa') = " + std::to_string(toPay*10) + ".\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        toPay = toPay*10;
                    }
                    if (affittoRaddoppiato)
                    {
                        std::cout << "L'imprevisto fa si che l'affitto si raddoppi: " << toPay << " x 2 = " << toPay*2 << ".\n";
                        output += "L'imprevisto fa si che l'affitto si raddoppi: " + std::to_string(toPay) + " x 2 = "  + std::to_string(toPay*2) + ".\n";
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
                        std::cout << e.s;
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        output += e.s + "Giocatore " + std::to_string(players[i]->getID()) + " e' andato in bancarotta e ha perso.\n";
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
                std::string hey ="\n" + muro;
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
                    prezzo = pos1->getPrezzo()/2;
                if (pos4)
                    prezzo = pos4->getPrezzo()/2;
                // Il giocatore che ha rifiutato di comprare non può partecipare all'asta, inoltre un giocatore partecipa all'asta se ha abbastanza soldi
                for (int w=0; w<players.size(); w++)
                {
                    if (players[i]->getID() != players[w]->getID() && players[w]->getMoney() > prezzo + minimaOffertaAsta)
                    {
                        playersAsta.push_back(players[w]);
                    }
                }
                if (!playersAsta.empty())
                {
                    do
                    {
                        std::cout << "\nAsta in corso\n";
                        std::cout << "All'asta partecipano i seguenti giocatori:";
                        hey += "\nAsta in corso\nAll'asta partecipano i seguenti giocatori:";
                        for (int z=0; z < playersAsta.size(); z++)
                        {
                            std::cout << " " << playersAsta[z]->getID();
                            hey += " " + std::to_string(playersAsta[z]->getID());
                        }
                        std::cout << ".\n";
                        hey += ".\n";
                        std::this_thread::sleep_for(std::chrono::seconds(pausa));
                        std::cout << "Proprieta' in vendita: " << players[i]->getPosition()->getName() << ".\n";
                        hey += "Proprieta' in vendita: " + players[i]->getPosition()->getName() + ".\n";
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
                            aggiornaSchermo(T, players,hey);
                            std::cout << "Giocatore " << playersAsta[0]->getID() << " ha pagato " << prezzo << " " << Variabili::getValuta() << " e ha acquistato " << players[i]->getPosition()->getName() << ".\n";
                        }
                        catch (const Giocatore::Not_Enough_Money& e)    // non dovrebbe mai lanciarla, ma meglio gestirla ...
                        {
                            std::cout << "Tentativo di acquisto non riuscito, " << Variabili::getValuta() << " non sufficenti.\n";
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            std::cout << "Non succede nulla.\n";
                        }
                        if (playersAsta[0]->_elenco_proprieta_st.size() > 1 && pos2)
                        {
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            int size = playersAsta[0]->_elenco_proprieta_st.size();
                            std::string s = std::to_string(size);
                            if (size == 4)
                                s = "tutte e 4 le";
                            std::cout << "Giocatore " << playersAsta[0]->getID() << " ora possiede " << s << " stazioni.\n";
                        }
                        if (playersAsta[0]->_elenco_proprieta_soc.size() == 2 && pos4)
                        {
                            std::this_thread::sleep_for(std::chrono::seconds(pausa));
                            std::cout << "Giocatore " << players[i]->getID() << " ora possiede tutte e 2 le societa'.\n";
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
                                std::cout << "Giocatore " << playersAsta[0]->getID() << " ora ha tutti i terreni di colore " << pos1->getColor() << ".\n";
                                output += "Giocatore " + std::to_string(playersAsta[0]->getID()) + " ora ha tutti i terreni di colore " + pos1->getColor() + ".\n";

                                // Aggiorno tutte le proprietà che la famiglia è al completo
                                for (int h=0; h < playersAsta[0]->_elenco_proprieta.size(); h++)
                                {
                                    if (pos1->getFamily() == playersAsta[0]->_elenco_proprieta[h]->getFamily())
                                        playersAsta[0]->_elenco_proprieta[h]->setCanBuy(true);
                                }
                            }
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