# Istruzioni per lo showcase

## Guida all'uso classico dell'applicazione

Per un uso classico, aprire l'applicazione e importare la libreria. La libreria principale è quella chiamata qui `main-library.json`.

È possibile filtrare e ordinare i media attraverso i filtri che si trovano nella barra laterale a sinistra. I filtri sono cumulativi, perciò un media, per essere trovato, deve soddisfare _tutti_ i filtri selezionati.

Inoltre, è possibile ricercare i media in modalità fuzzy-find attraverso un motore di ricerca ad-hoc. *Tutti i campi dei media* (perlomeno quelli ragionevoli, non di certo l'id) *sono ricercabili*, anche quelli del particolare tipo di media.

È possibile eliminare un media attraverso un click destro -> elimina, e aggiungere un media attraverso il `+` in alto a destra.

## Funzionalità - Import-export delle thumbnails

All'esportazione, vengono esportate solamente le immagini di copertina che rimandano a risorse online, non quelle locali. 

Dunque, è possibile "esportare" (in realtà _copiare_) tutte le thumbnails locali verso una singola cartella. Sourcevault chiamerà queste immagini con l'id del media a cui sono associate. Poi, è possibile importarle da una cartella formata dal suddetto processo. Grazie a questa funzionalità è possibile simulare un export totale esportando sia libreria che immagini, seppur separatamente.

È possibile importare alcune delle thumbnails della libreria principale dalla cartella `main-library-thumbnails`. Altre sono online, e non hanno perciò bisogno di essere importate.

Una soluzione più funzionale sarebbe l'export in formato `.zip` contenente sia libreria che immagini. Tuttavia, Qt non supporta il formato zip nativamente e ciò mi ha fatto desistere.

## Funzionalità - merge della libreria

È possibile provare la funzionalità di merge della libreria importando `try-merge-with-me.json`. Questa libreria contiene alcuni media già presenti in `main-library`, ed altri assenti che verranno dunque aggiunti alla libreria. I media già presenti in `main-library` non saranno né intaccati né copiati.

## Funzionalità - parsing JSON robusto

Nella cartella `parsing-showcase` viene fornito un insieme di file JSON che Sourcevault considera _mal formati_. Provando ad importarli, è possibile vedere in azione il motore di parsing dell'applicazione e i suoi riscontri precisi in caso di errore.