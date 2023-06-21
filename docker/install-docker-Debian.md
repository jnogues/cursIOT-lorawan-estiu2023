1. Entreu a clouding.io i us doneu d'alta.
2. Feu servir 'oferta d'un VPS gratis per un mes.
3. Trieu Debian 12.
4. Entreu per primera vegada per ssh amb `ssh root@xx.xx.xx.xx`. Cal introduir nou password (p.ex. Curs2023#).
5. Feu `apt update` i `apt upgrade`.
6. Ara crearem un nou usuari `apt adduser lora`. Responeu al que us demana, alerta amb el password (p.ex. Curs2023#).
7. Ara afegim usuari **lora** a sudoers `adduser lora sudo`.
8. Reconfigurem zona horària `dpkg-reconfigure tzdata`.
9. Sortim amb `exit`.
10. Tornem a entrar amb `ssh lora@xx.xx.xx.xx`.
11. Fem `sudo apt update` i `sudo apt upgrade`.
12. Ara ja anem a per la instal·lacio de docker.
13. 'sudo apt-get update'
14. `sudo apt-get install ca-certificates curl gnupg`
15. `sudo install -m 0755 -d /etc/apt/keyrings`
16. `curl -fsSL https://download.docker.com/linux/debian/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg`
17. `sudo chmod a+r /etc/apt/keyrings/docker.gpg`
18. `echo "deb [arch="$(dpkg --print-architecture)" signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/debian "$(. /etc/os-release && echo "$VERSION_CODENAME")" stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null `
20. `sudo apt-get update`
21. `sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin`
22. Per saber si va bé: `sudo docker run hello-world`
23. Ara per tal d'executar docker sense privilegis: `sudo groupadd docker`i `sudo usermod -aG docker $USER`
24. Cal sortir amb `exit` i tornar a entrar amb `ssh lora@xx.xx.xx.xx`
25. Probem `docker run hello-world`, si va, tot OK.
26. Ara instal·lem docker-compose.
27. `sudo apt-get update`
28. `sudo apt-get install docker-compose-plugin`
29. Verifiquem la versió `docker compose version`
30. Perquè docker comenci al boot: `sudo systemctl enable docker.service` i `sudo systemctl enable containerd.service`
31. JA HO TENIM!!!!!!!
32. 