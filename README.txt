Gioco del monopoly personalizzato GZ: in questa versione non sono implementati scambi di proprietà.
Tralascando questa piccola inezia il gioco è perfettamente identico al classico gioco del monopoly,
con le stesse regole (+ qualche modifichina). Qui vi elenco qualche info importante del gioco:
- ad inizio gioco viene chiesto il numero di giocatori che giocherà la partita e successivamente bisognerà specificare
  per ogni giocatore se è un giocatore umano (H per Human) o se è un bot (C per Computer)
- la pausa di attesa tra una stampa e la successiva è settata ad 1 secondo, 
  se il programma va troppo veloce bisogna settarla a 2 (in Variabili.h)
- se al contrario il gioco vi sembra troppo lento ho proposto ad inizio di ogni partita un numero di turni 
  iniziali da fare più veloci del normale, senza pausa  tra un output e il successivo, se non per dialoghi con
  giocatori Human o quando i giocatori ipotecano delle proprietà (evento "importante" per cui il gioo rallenta, by GZ)
- in tutti gli altri casi (tiro di dadi, pagamenti, movimenti sul tabellone, ecc.) il programma va avanti autonomamente
  (se la pausa è settata ad 1, questi passaggi vengono stampati ad uno ad uno con relative pause, con pausa = 0 invece,
  come nei turni velocizzati iniziali, non si ha il tempo di leggere tranne in caso di scelta di un giocatore Human)
- quando si hanno tutte le proprietà di uno stesso colore si sblocca la sottolineatura, per i terreni 
  significa che ci si può costruire e (come da regolamento ufficiale le case devono essere costruite 
  in modo omogeneo, perciò è possibile che nel corso di una partita alcuni terreni non risultino più sottolineati,
  questo perchè li siete momentaneamente apposto e non potete costruirci finche non si va in pari)
- i giocatori devono anche ipotecare le case in modo omogeneo: supponiamo vhe il giocatore i abbia costruito 4 case su 
  ogni terreno di colore rosso e poi supponiamo che debba pagare 800 euro e debba ipotecare delle case,
  il giocatore non può ipotecare solo le case di una proprietà, perchè devono sempre essere distribuite in modo omogeneo
- miglioria rispetto alle regole ufficiali: non ci sono limiti di case/alberghi possibili da costruire 
  (non ci sono ad ex 28 case e poi basta, se ne possono costruire finche se ne ha la possibilità)
- i giocatori computer generano sia risposte casuali sia risposte intelligenti
  (si potrebbe lavorare di più sulla loro parziale IA)
- quando un giocatore deve ipotecare dei terreni se è un bot ipoteca prima le società, poi le stazioni, poi eventuali case 
  e infine i terreni
- quando un giocatore ipoteca il terreno, questo viene semplicemente restituito alla banca, senza aste o robe varie
- quando un giocatore ottiene una carta uscita gratis di prigione essa viene visualizzata sotto forma di asterisco
  accanto alla scritta del nome: "Giocatore i*:"