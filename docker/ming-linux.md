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

9. Guardem amb ^CTRL O` i `CTRL X`
10. Anem a influxdb `cd /home/lora/my_docker/influxdb`
11. Fem 2 carpetes: `mkdir data` i `mkdir config`
    
