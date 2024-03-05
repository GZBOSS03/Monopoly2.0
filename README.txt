Gioco del monopoly personalizzato GZ: in questa versione non sono implementati scambi di proprietà.
Tralascando questa piccola inezia il gioco è perfettamente identico al classico gioco del monopoly,
con le stesse regole (con qualche modifichina). Qui vi elenco qualche info importante del gioco:
- ad inizio gioco viene chiesto il numero di giocatori che giocherà la partita e successivamente bisognerà specificare
  per ogni giocatore se è un giocatore umano (H per Human) o se è un bot (C per Computer)
- la pausa di attesa tra una stampa e la successiva è settata ad 1 secondo, 
  se il programma va troppo veloce bisogna settarla a 2 (in Variabili.h)
- se al contrario il gioco vi sembra troppo lento ho proposto ad inizio di ogni partita un numero di turni 
  iniziali da fare più veloci del normale, senza pausa (il gioco interagisce con l'utente solo quando deve 
  farlo, per il resto va avanti come un treno)
- quando si hanno tutte le proprietà di uno stesso colore si sblocca la sottolineatura, per i terreni 
  significa che ci si può costruire e (come da regolamento ufficiale le case devono essere costruite 
  in modo omogeneo, perciò è possibile che nel corso di una partita alcuni terreni non risultino più sottolineati,
  questo perchè li siete momentaneamente apposto e non potete costruirci finche non si va in pari)
- miglioria rispetto alle regole ufficiali: non ci sono limiti di case/alberghi possibili da costruire 
  (non ci sono ad ex 28 case e poi basta, se ne possono costruire finche se ne ha la possibilità)
- i giocatori computer generano sia risposte casuali sia risposte intelligenti
  * si potrebbe lavorare si più sulla loro parziale IA
- i giocatori non sono costretti a dover ipotecare per forza le case in modo omogeneo 
  (non so se nelle regole ufficiali ci sia scritto da qualche parte, a me non sembrava di averlo visto)
  In ogni caso in questa versione è possibile costruire 4 case su ogni proprieta di colore rosso (ad esempio)
  e poi ipotecare solo le case di una proprietà, lasciandone una con 0 case e le altre con 4 case)
- quando un giocatore deve ipotecare dei terreni prima è obbligato ad ipotecare eventuali case e poi può ipotecare i terreni
- quando un giocatore ipoteca il terreno, questo viene semplicemente restituito alla banca, senza aste o robe varie
- il programma dialoga con l'utente prima che parta il gioco e con eventuali giocatori Human per capire le loro scelte
  in tutti gli altri casi (tiro di dadi, pagamenti, movimenti sul tabellone, ecc.) il programma va avanti autonomamente
