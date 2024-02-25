#ifndef VARIABILI_H
#define VARIABILI_H

#include <string>

// Cambiando i valori di questa struct si può personalizzare il gioco

struct Variabili
{
    public:
    // Personalizzabili
    static const std::string getValuta() {return "euro";}
    static const int contoAllaRovescitaDa = 3;
    static const int turnoSbloccoAste = 15; // For the future

    // Preferibilmete da non modificare
    static const int nFacceDado = 6;
    static const int moneyPassaggioVia = 200;
    static const int moneyUscitaPrigione = 50;
    static const int initialMoney = 1500;
    // Da non modificare
    // Variabili per IA dei computer
    static const int probabilitaSiFirstAcquisto = 25;
    static const int probabilitaSiExitJail = 50;
    static const int maxMoneyAsta = 3; // Se una proprietà costa 100 il computer in un asta non spenderà mai più di maxMoneyAsta*100 (a meno che non sia davvero interessato)
    // Regole
    static const int nDadi = 2;
    static const int nMaxTurniPrigione = 3;
    // Per l'output
    static const int dimMaxCasella = 4 + 11;   // Siccome il numero di giocatori varia tra 2 e 6, tengo 4 per avere un buon output
    static const int dimMaxOutput = dimMaxCasella * 11;  // Numero di caselle per colonna (dim x del tabellone)
    /*
    Per stampare il carattere di inizio casella "|" + " "                                       (2)
    + il tipo di casella Ex: StN                                                                (3)
    + il numero di giocatori sopra alla casella (massimo nGiocatori, caso raro ma possibile)    (0-nGiocatori)
    + eventuali case/albergo                                                                    (0-4)
    + carattere di chiusura cella " " + "|"                                                     (2)
    Aggiungo qualche spazio a destra e sinistra per avere un output del tabellone più omogeneo  (1-2)
    */

};

#endif  // VARIABILI_H