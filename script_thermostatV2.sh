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

cp $INTERFACE_FOLDER/consigne.csv .
sudo ./thermostatV2 |
while read -r DATE JOUR HEURE NOMBRE1 NOMBRE2 NOMBRE3 NOMBRE4 NOMBRE5 TEXTE
do
	#echo "$DATE" "$JOUR" "$HEURE" "$NOMBRE1" "$NOMBRE2" "$NOMBRE3" "$NOMBRE4" "$NOMBRE5" "$TEXTE"
	sqlite3  $EXEC_FOLDER/meteosat.db "insert into meteo values('$DATE','$JOUR','$HEURE','$NOMBRE1','$NOMBRE2','$NOMBRE3','$NOMBRE4','$NOMBRE5','$TEXTE');"
done
sudo kill -9 `pgrep fbi`
sudo fbi -T 2 -d /dev/fb1 -noverbose -a out.jpg
cp out.jpg $INTERFACE_FOLDER/
cp LFOP.TXT $INTERFACE_FOLDER/
cp meteosat.db $INTERFACE_FOLDER/
#send order to huzzah board
sudo ./clientThermo 192.168.1.117
