### **DOCKER a W10- MING**

*   Fem una carpeta a l'escriptori amb nom **my\_docker**.
*   Dins de **my_docker** fem 4 carpetes: **nodered00**, **mosquitto**, **influxdb** i **grafana**.
*   Dins de **mosquitto** fem 3 carpetes: **config**, **data** i **log.** Dins de config, cal fer un fitxer de text pla (amb el bloc de notes), de nom **mosquitto.conf**, amb el següent contingut:

**allow\_anonymous true**  
**listener 1883**  
**persistence true**  
**persistence\_location /mosquitto/data/**  
**log\_dest file /mosquitto/log/mosquitto.log**

*   Dins d' **influxdb** fem 2 carpetes: **data** i **config**.
*   Ara instal·lem [Docker Desktop](https://www.docker.com/) per Windows.
*   Reiniciem Windows.
*   Si no està obert, iniciem **Docker Desktop**.
*   Obrim el **CMD** (Simbolo del sistema).
*   Suposem que a W10 som l'usuari **lora**.
*   Per instal·lar i iniciar el contenidor de node-red utilitzem aquest comandament: 

```text-plain
docker run -d -p 1880:1880 -v C:\Users\lora\Desktop\my_docker\nodered00:/data --restart unless-stopped --name mynodered00 nodered/node-red
```

*   Per instal·lar i iniciar el contenidor de mosquitto:

```text-plain
docker run -d -p 1883:1883 -v C:\Users\lora\Desktop\my_docker\mosquitto:/mosquitto/ --restart unless-stopped --name mymosquitto eclipse-mosquitto
```

*   Per influxdb (demana nou password 1a vegada):

```text-plain
docker run -d -p 8086:8086 -v C:\Users\lora\Desktop\my_docker\influxdb\data:/var/lib/influxdb2 -v C:\Users\lora\Desktop\my_docker\influxdb\config:/etc/influxdb2 --name=myinfluxdb --restart unless-stopped influxdb:2.7
```

*   Per Grafana, usuari i password per defecte (admin admin):

```text-plain
docker run -d  -p 3000:3000 -e "GF_AUTH_ANONYMOUS_ENABLED=true" --name=mygrafana --restart unless-stopped -v C:\Users\lora\Desktop\my_docker\grafana:/var/lib/grafana grafana/grafana
```

