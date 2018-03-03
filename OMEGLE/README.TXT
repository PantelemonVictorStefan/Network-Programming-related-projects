Descriere:

Proiect personal asemanator aplicatiei Omegle, acesta conecteaza 2 utilizatori si creeaza o "camera" unde acestia schimba mesaje.
Serverul este concurent cu fork(). Acesta asteapta ca 2 clienti sa se conecteze, urmand apoi sa creeze procesele fii care se vor ocupa de schimbul de mesaje.
Deoarece aplicatia foloseste apeluri blocante este nevoie si de un proces fiu la nivel de client responsabil de read si afisarea mesajelor.
