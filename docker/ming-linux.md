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
14. Ara cal obrir els ports del tallafocs del Clouding: 1880 a 1882 (http), 1883 (All TCP), 8086 (http), 3000 (http)
15. 15. Iniciem node-red:

`docker run -d -p 1880:1880 -v /home/lora/my_docker/nodered00:/data --restart unless-stopped --name mynodered00 nodered/node-red`

14. Iniciem mosquitto:
    
`docker run -d -p 1883:1883 -v /home/lora/my_docker/mosquitto:/mosquitto/ --restart unless-stopped --name mymosquitto eclipse-mosquitto`
    
16. Iniciem influxdb:

`docker run -d -p 8086:8086 -v /home/lora/my_docker/influxdb/data:/var/lib/influxdb2 -v /home/lora/my_docker/influxdb/config:/etc/influxdb2 --name=myinfluxdb --restart unless-stopped influxdb:2.7`

17. `docker run -d  --user '1001' -p 3000:3000 -e "GF_AUTH_ANONYMOUS_ENABLED=true" --name=mygrafana --restart unless-stopped -v /home/lora/my_docker/grafana:/var/lib/grafana grafana/grafana`
   
18. 
