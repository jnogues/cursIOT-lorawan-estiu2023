# Configuració inicial de Debian, instal·lació de docker i docker-compose.
## Configuració de Debian
1. Entreu a [clouding.io](https://clouding.io/) i us doneu d'alta.
2. Feu servir l'oferta d'un VPS gratis per un mes.
3. Trieu Debian 12, 1 core, 10GB de disc.
4. Entreu per primera vegada per ssh amb `ssh root@xx.xx.xx.xx`. Cal introduir nou password (p.ex. Curs2023#). Amb W10 ho podem fer pel CMD, powershell o amb [mobaXterm](https://mobaxterm.mobatek.net/download.html).
5. Feu `apt update` i `apt upgrade`.
6. Ara crearem un nou usuari `apt adduser lora`. Responeu al que us demana, alerta amb el password (p.ex. Curs2023#).
7. Ara afegim usuari **lora** a sudoers `adduser lora sudo`.
8. Reconfigurem zona horària `dpkg-reconfigure tzdata`.
9. Sortim amb `exit`.
10. Tornem a entrar amb `ssh lora@xx.xx.xx.xx`.
11. Fem `sudo apt update` i `sudo apt upgrade`.
## Instal·lació de docker, via ràpida
12. Ara ja anem a per la instal·lacio de docker.
13. Tenim l'opció **ràpida**, que es aquesta:
14. `curl -fsSL https://get.docker.com -o get-docker.sh`
15. `sudo sh get-docker.sh`
## Instal·lació de docker, via lenta
16. Si ho volem fer a ma, pas a pas:
17. `sudo apt-get update`
18. `sudo apt-get install ca-certificates curl gnupg`
19. `sudo install -m 0755 -d /etc/apt/keyrings`
20. `curl -fsSL https://download.docker.com/linux/debian/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg`
21. `sudo chmod a+r /etc/apt/keyrings/docker.gpg`
22. `echo "deb [arch="$(dpkg --print-architecture)" signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/debian "$(. /etc/os-release && echo "$VERSION_CODENAME")" stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null `
23. `sudo apt-get update`
24. `sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin`
## Post-instal·lació
25. Per saber si va bé: `sudo docker run hello-world`
26. Per saber la versió: `docker --version`
27. Ara per tal d'executar docker sense privilegis: `sudo groupadd docker`i `sudo usermod -aG docker $USER`
28. Cal sortir amb `exit` i tornar a entrar amb `ssh lora@xx.xx.xx.xx`
29. Probem `docker run hello-world`, si va, tot OK.
30. Perquè docker comenci al boot: `sudo systemctl enable docker.service` i `sudo systemctl enable containerd.service`
## Instal·lació de docker-compose
30. En principi docker-compose ja hauria d'estar instal·lat. Verifiquem-ho amb `docker compose version`
31. JA HO TENIM!!!!!!!

