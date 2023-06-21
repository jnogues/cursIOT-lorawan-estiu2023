1. Entreu a clouding.io i us doneu d'alta.
2. Feu servir 'oferta d'un VPS gratis per un mes.
3. Trieu Debian 12.
4. Entreu per primera vegada per ssh amb `ssh root@xx.xx.xx.xx`. Cal introduir nou password (p.ex. Curs2023#).
5. Feu `apt update` i `apt upgrade`.
6. Ara crearem un nou usuari `apt adduser lora`. Responeu al que us demana, alerta amb el password (p.ex. Curs2023#).
7. Ara afegim usuari **lora** a sudoers `adduser lora sudo`.
8. Reconfigurem zona horària `dpkg-reconfigure tzdata`.
9. Sortim amb `exit`.
10. Tornem a entrar amb `lora@xx.xx.xx.xx`.
11. Fem `sudo apt update` i `sudo apt upgrade`.
12.  
