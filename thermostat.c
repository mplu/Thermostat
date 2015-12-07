#include "includes.h"

int main (void)
{
	float temptemperature,consigne,consignedenuit;
	time_t secondes;
	struct tm instant;
	int heure, minute, jour,jourmois,mois,annee,i,ordre_chauffe=0;
	//char Table_bin[j_jour_size][50];
	s_noaa webdata;
	char command[TAILLE_MAX] = {0};
	char etat_chauffage[TAILLE_MAX] = {0};
	char *tab_jour_semaine[] = {"Lundi","Mardi","Mercredi","Jeudi","Vendredi","Samedi","Dimanche"};
    char *tab_mois[] = {"Janvier","Février","Mars","Avril","Mai","Juin","Juillet","Août","Septembre","Octobre","Novembre","Décembre"};
	
	
	char **Table_bin = (char **)malloc(sizeof(char*)*j_jour_size);
	char *tab2 = (char *)malloc(sizeof(char)*50*j_jour_size);
	for(i = 0 ; i < j_jour_size ; i++){
		Table_bin[i] = &tab2[i*50];
	}
	
	
	
	/* Recupérer le jour de la semaine (0 = dimanche) et l'heure */
	time(&secondes);
	instant=*localtime(&secondes);
	jour = instant.tm_wday;
	heure = instant.tm_hour;
	minute = instant.tm_min;
	jourmois = instant.tm_mday;
	mois = instant.tm_mon;
	annee = instant.tm_year;
	
	//remise en accord des numero de jours
	if (jour == 0)
	{
		jour = 6;
	}
	else
	{
		jour --;
	}
    
	/* Récupérer la température */
	GetTemperatureFromProbe(FILE_SONDE1,&temptemperature);
	
	/* Lecture du fichier de consigne */
	GetConsigneFromFile(FILE_CONSIGNE,Table_bin,&consigne,&consignedenuit);
	
	/* Doit on chauffer ? */
	// chercher index consigne
	i = heure * 2;
	if(minute >= 30)
	{
		i++;
	}
	
	if((Table_bin[(e_jour)jour][i] == 1) && (temptemperature < consigne))
	{
		ordre_chauffe = RAD_ON;
	}else
	{
		ordre_chauffe = RAD_OFF;
	}
	
	
	/* Pilotage du radiateur */
	wiringPiSetup () ;
	pinMode (THERMO_PIN, OUTPUT) ;
	
	if(ordre_chauffe == RAD_ON)
	{
		digitalWrite (THERMO_PIN,  LOW) ;
		//printf("mode jour\n");
	}else
	{
		if(temptemperature < consignedenuit)
		{
			digitalWrite (THERMO_PIN, LOW) ;
			ordre_chauffe = RAD_ON_FORCED;
			//printf("mode nuit qui chauffe\n");
		}else
		{
			digitalWrite (THERMO_PIN, HIGH) ;
			//printf("mode nuit\n");
		}
	}
	
	GetDataFromNOAA(FILE_INFO,&webdata);
	
	switch(ordre_chauffe)
	{
		case RAD_OFF:
			sprintf(etat_chauffage,"%s",ETEINT);
			break;
		case RAD_ON:
			sprintf(etat_chauffage,"%s",MARCHE);
			break;
		case RAD_ON_FORCED:
			sprintf(etat_chauffage,"%s",MARCHE_FORCE);
			break;
		default :
			sprintf(etat_chauffage,"%s",INCONNU);
			break;
	}
	
	sprintf(command,"convert fond.bmp \
					-pointsize 20 -draw \"fill white  text 15,25 '%s %d %s' \" \
					-pointsize 40 -draw \"fill white  text 100,70 '%dh%02d' \" \
					-pointsize 55 -draw \"fill white  text 20,140 '%.1f°C' \" \
					-pointsize 15 -draw \"fill white  text 220,120 'Chauffage' \" \
					-pointsize 15 -draw \"fill white  text 220,140 '%s' \" \
					-pointsize 17 -draw \"fill white  text 10,205 '%d°C / HR : %d%% / Vent : %dkm/h' \" \
					-pointsize 17 -draw \"fill white  text 10,230 '(%s - %s UTC)' \" \
					out.jpg"
					,tab_jour_semaine[jour],jourmois,tab_mois[mois]
					,heure,minute
					,temptemperature
					,etat_chauffage
					,webdata.temperature,webdata.humidite,webdata.vent
					,webdata.date,webdata.heure);
	
	system(command);
	
	//printf("%04d-%02d-%02d %d %d:%02d / ",annee+1900,mois+1,jourmois,jour,heure,minute);
	//printf("mesure : %.2f / consigne : %.2f => ",temptemperature,consigne);
	//printf("%s %s %d %d %d\n", webdata.date,webdata.heure,);
	if((minute%15==0))
	{
		printf("%04d-%02d-%02d\t%d\t%d:%02d\t%.2f\t%.2f\t%d\t%d\t%d\t%s\n",annee+1900,mois+1,jourmois,jour,heure,minute,consigne,temptemperature,webdata.temperature,webdata.humidite,webdata.vent,etat_chauffage);
	}
	
	/*sprintf(command,"sh -c \"echo 508 > /sys/class/gpio/export\"");
	system(command);
	sprintf(command,"sh -c \"echo 'out' > /sys/class/gpio/gpio508/direction\"");
	system(command);*/
	if((heure<23)&&(heure>=6))
	{
		sprintf(command,"sh -c \"echo '1' > /sys/class/gpio/gpio508/value\"");
	}else
	{
		sprintf(command,"sh -c \"echo '0' > /sys/class/gpio/gpio508/value\"");
	}
	system(command);
	
	return 0 ;
}
