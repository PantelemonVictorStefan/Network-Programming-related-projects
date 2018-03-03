Cerinta:
Sa se dezvolte o aplicatie client/server care sa permita schimbul de mesaje intre utilizatori care sunt conectati si sa ofere functionalitatea trimiterii mesajelor si catre utilizatorii offline, acestora din urma aparandu-le mesajele atunci cand se vor conecta la server. De asemenea, utilizatorii vor avea posibilitatea de a trimite un raspuns (reply) in mod specific la anumite mesaje primite. Aplicatia va oferi si istoricul conversatiilor pentru si cu fiecare utilizator in parte. 

Descriere:

O aplicatie de tip messenger care permite schimbul de mesaje intre utilizatori, acestia putand sa primeasca mesaje si cand sunt offline si putand sa le citeasca mai tarziu.
Prezinta o functie de register, un modul de autentificare, un sistem de notificare si un modul de schimb de mesaje.
Serverul este de tip concurent cu fork() si apeluri blocante (o provocare pentru acest gen de aplicatie, recomandata fiind multiplexarea).
