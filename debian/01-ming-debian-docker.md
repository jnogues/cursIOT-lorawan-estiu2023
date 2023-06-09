# Com instal·lar MING amb docker a linux DEBIAN 12 de Clouding.io
## Creació de directoris
1. Entrem al servidor `ssh lora@xx.xx.xx.xx`
2. Fem un directori per docker: `mkdir /home/lora/my_docker`
3. Anem a my_docker: `cd my_docker`
4. Fem `mkdir nodered00` , `mkdir mosquitto` , `mkdir influxdb` i `mkdir grafana`
5. Anem a mosquitto: `cd mosquitto`
6. Fem `mkdir config` , `mkdir data` i `mkdir log`.
7. Anem a config `cd config`
8. Fem un fitxer de text amb `nano mosquitto.conf`. Contingut del fitxer:

**allow\_anonymous true**  
**listener 1883**  
**persistence true**  
**persistence\_location /mosquitto/data/**  
**log\_dest file /mosquitto/log/mosquitto.log**

9. Guardem amb `CTRL O` i `CTRL X`
10. Anem a influxdb `cd /home/lora/my_docker/influxdb`
11. Fem 2 carpetes: `mkdir data` i `mkdir config`
12. Tornem a la carpeta d'usuari `cd`
13. Donem permissos `sudo chown -R 1000:1000 /home/lora/my_docker/nodered00`
## Obertura de ports del firewall
14. Ara cal obrir els ports del tallafocs del Clouding: 1880 a 1882 (http), 1883 (TCP), 8086 (http), 3000 (http), 8000(TCP), 9000(http), 9443(TCP), o bé els obrim tots alhora, del 1 al 65535: [Cómo crear una regla de firewall](https://help.clouding.io/hc/es/articles/360013590619-C%C3%B3mo-crear-una-regla-de-firewall?source=search&auth_token=eyJhbGciOiJIUzI1NiJ9.eyJhY2NvdW50X2lkIjo5MTkwNjIsInVzZXJfaWQiOjg4OTE1Mjg2ODIwMTIsInRpY2tldF9pZCI6MTM3ODk0LCJjaGFubmVsX2lkIjo2MywidHlwZSI6IlNFQVJDSCIsImV4cCI6MTY4OTkzOTAwM30._PUWPiBGeIBxDHqkOe13fSWbpmeo2PIiIE_FTvIBONs)
## Portainer
15. Creem un volum per docker: `docker volume create portainer_data`
16. Iniciem portainer, demana un password de 12 caràcters per usuari admin (p.ex Curslora2023#):
    
`docker run -d -p 8000:8000 -p 9000:9000 -p 9443:9443 --name=myportainer --restart=always -v /var/run/docker.sock:/var/run/docker.sock -v portainer_data:/data portainer/portainer-ce`

## Node-red
17. Iniciem node-red:

`docker run -d -p 1880:1880 -v /home/lora/my_docker/nodered00:/data --restart unless-stopped --name mynodered00 nodered/node-red`

## Mosquitto
16. Iniciem mosquitto:
    
`docker run -d -p 1883:1883 -v /home/lora/my_docker/mosquitto:/mosquitto/ --restart unless-stopped --name mymosquitto eclipse-mosquitto`
    
# Influxdb
17. Iniciem influxdb:

`docker run -d -p 8086:8086 -v /home/lora/my_docker/influxdb/data:/var/lib/influxdb2 -v /home/lora/my_docker/influxdb/config:/etc/influxdb2 --name=myinfluxdb --restart unless-stopped influxdb:2.7`

## Grafana
18. Iniciem grafana:

`docker run -d  --user '1001' -p 3000:3000 -e "GF_AUTH_ANONYMOUS_ENABLED=true" --name=mygrafana --restart unless-stopped -v /home/lora/my_docker/grafana:/var/lib/grafana grafana/grafana`

