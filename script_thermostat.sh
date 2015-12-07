#!/bin/sh

EXEC_FOLDER="/path/to/folder/where/exec/runs"
INTERFACE_FOLDER="/path/to/folder/where/export/import/data"
cd $EXEC_FOLDER

if [ -f "LFOP.TXT" ];then
	echo "Le fichier existe ! on le supprime";
	rm LFOP.TXT*
else
	echo "Le fichier n'existe pas !"
fi

#http://weather.noaa.gov/international.html
wget http://weather.noaa.gov/pub/data/observations/metar/decoded/LFOP.TXT

if [ -f "LFOP.TXT" ];then
	echo "fichier récupéré !";
	
else
	echo "impossible de chopper le truc !"
fi

cp $INTERFACE_FOLDER/consigne_chambre.csv .
sudo ./thermostat_chambre |
while read -r DATE JOUR HEURE NOMBRE1 NOMBRE2 NOMBRE3 NOMBRE4 NOMBRE5 TEXTE
do
	sqlite3  $EXEC_FOLDER/meteosat2.db "insert into meteo values('$DATE','$JOUR','$HEURE','$NOMBRE1','$NOMBRE2','$NOMBRE3','$NOMBRE4','$NOMBRE5','$TEXTE');"
done
cp out2.jpg $INTERFACE_FOLDER/
cp meteosat2.db $INTERFACE_FOLDER/
